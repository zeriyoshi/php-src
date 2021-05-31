/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | https://www.php.net/license/3_01.txt                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Go Kudo <zeriyoshi@gmail.com>                                |
   +----------------------------------------------------------------------+
*/

#include <stdlib.h>

#include "php.h"
#include "info.h"

#include "php_random.h"
#include "php_random_class.h"
#include "random_class_arginfo.h"

#include "zend_exceptions.h"

static zend_array php_random_class_algos;
static zend_class_entry *php_random_class_ce;
static zend_object_handlers php_random_class_object_handlers;

static uint32_t range32(php_random_class *random_class, zend_object *obj, uint32_t umax) {
	uint32_t result, limit;

	result = php_random_class_next(random_class, obj);

	/* Special case where no modulus is required */
	if (UNEXPECTED(umax == UINT32_MAX)) {
		return result;
	}

	/* Increment the max so the range is inclusive of max */
	umax++;

	/* Powers of two are not biased */
	if ((umax & (umax - 1)) == 0) {
		return result & (umax - 1);
	}

	/* Ceiling under which UINT32_MAX % max == 0 */
	limit = UINT32_MAX - (UINT32_MAX % umax) - 1;

	/* Discard numbers over the limit to avoid modulo bias */
	while (UNEXPECTED(result > limit)) {
		result = php_random_class_next(random_class, obj);
	}

	return result % umax;
}

#if ZEND_ULONG_MAX > UINT32_MAX
static uint64_t range64(php_random_class *random_class, zend_object *obj, uint64_t umax) {
	uint64_t result, limit;

	result = php_random_class_next(random_class, obj);
	if (random_class->algo && random_class->algo->bytes == 32) {
		result = (result << 32) | php_random_class_next(random_class, obj);
	}

	/* Special case where no modulus is required */
	if (UNEXPECTED(umax == UINT64_MAX)) {
		return result;
	}

	/* Increment the max so the range is inclusive of max */
	umax++;

	/* Powers of two are not biased */
	if ((umax & (umax - 1)) == 0) {
		return result & (umax - 1);
	}

	/* Ceiling under which UINT64_MAX % max == 0 */
	limit = UINT64_MAX - (UINT64_MAX % umax) - 1;

	/* Discard numbers over the limit to avoid modulo bias */
	while (UNEXPECTED(result > limit)) {
		result = php_random_class_next(random_class, obj);
		if (random_class->algo && random_class->algo->bytes == 32) {
			result = (result << 32) | php_random_class_next(random_class, obj);
		}
	}

	return result % umax;
}
#endif

/* {{{ */
int php_random_class_algo_register(const php_random_class_algo *algo)
{
	zval zalgo;
	ZVAL_PTR(&zalgo, (php_random_class_algo*)algo);
	if (zend_hash_str_add(&php_random_class_algos, algo->ident, strlen(algo->ident), &zalgo)) {
		return SUCCESS;
	}
	return FAILURE;
}
/* }}} */

/* {{{ */
void php_random_class_algo_unregister(const char *ident)
{
	zend_hash_str_del(&php_random_class_algos, ident, strlen(ident));
}
/* }}} */

/* {{{ */
const php_random_class_algo* php_random_class_algo_find(const zend_string *ident)
{
	zval *tmp;

	if (!ident) {
		return NULL;
	}

	tmp = zend_hash_find(&php_random_class_algos, (zend_string*)ident);
	if (!tmp || (Z_TYPE_P(tmp) != IS_PTR)) {
		return NULL;
	}

	return Z_PTR_P(tmp);
}
/* }}} */

/* {{{ */
uint64_t php_random_class_next(php_random_class *random_class, zend_object *obj)
{
	zend_long ret;
	zend_function *function;
	zval function_name, retval;
	
	if (random_class->algo) {
		return random_class->algo->next(random_class->state);
	}

	/* call user implementation. */
	ZVAL_STRING(&function_name, "next");
	function = zend_hash_find_ptr(&obj->ce->function_table, Z_STR(function_name));
	zval_ptr_dtor(&function_name);

	zend_call_known_instance_method_with_0_params(function, obj, &retval);
	ret = Z_LVAL(retval);

	return (uint64_t) ret;
}
/* }}} */

/* {{{ */
zend_long php_random_class_range(php_random_class *random_class, zend_object *obj, zend_long min, zend_long max)
{
	zend_ulong umax = max - min;

#if ZEND_ULONG_MAX > UINT32_MAX
	if (umax > UINT32_MAX) {
		return (zend_long) (range64(random_class, obj, umax) + min);
	}
#endif

	return (zend_long) (range32(random_class, obj, umax) + min);
}
/* }}} */

/* {{{ */
void php_random_class_array_data_shuffle(php_random_class *random_class, zend_object *obj, zval *array)
{
	uint32_t idx, j, n_elems;
	Bucket *p, temp;
	HashTable *hash;
	zend_long rnd_idx;
	uint32_t n_left;

	n_elems = zend_hash_num_elements(Z_ARRVAL_P(array));

	if (n_elems < 1) {
		return;
	}

	hash = Z_ARRVAL_P(array);
	n_left = n_elems;

	if (EXPECTED(!HT_HAS_ITERATORS(hash))) {
		if (hash->nNumUsed != hash->nNumOfElements) {
			for (j = 0, idx = 0; idx < hash->nNumUsed; idx++) {
				p = hash->arData + idx;
				if (Z_TYPE(p->val) == IS_UNDEF) continue;
				if (j != idx) {
					hash->arData[j] = *p;
				}
				j++;
			}
		}
		while (--n_left) {
			rnd_idx = php_random_class_range(random_class, obj, 0, n_left);
			if (rnd_idx != n_left) {
				temp = hash->arData[n_left];
				hash->arData[n_left] = hash->arData[rnd_idx];
				hash->arData[rnd_idx] = temp;
			}
		}
	} else {
		uint32_t iter_pos = zend_hash_iterators_lower_pos(hash, 0);

		if (hash->nNumUsed != hash->nNumOfElements) {
			for (j = 0, idx = 0; idx < hash->nNumUsed; idx++) {
				p = hash->arData + idx;
				if (Z_TYPE(p->val) == IS_UNDEF) continue;
				if (j != idx) {
					hash->arData[j] = *p;
					if (idx == iter_pos) {
						zend_hash_iterators_update(hash, idx, j);
						iter_pos = zend_hash_iterators_lower_pos(hash, iter_pos + 1);
					}
				}
				j++;
			}
		}
		while (--n_left) {
			rnd_idx = php_random_class_range(random_class, obj, 0, n_left);
			if (rnd_idx != n_left) {
				temp = hash->arData[n_left];
				hash->arData[n_left] = hash->arData[rnd_idx];
				hash->arData[rnd_idx] = temp;
				zend_hash_iterators_update(hash, (uint32_t)rnd_idx, n_left);
			}
		}
	}
	hash->nNumUsed = n_elems;
	hash->nInternalPointer = 0;

	for (j = 0; j < n_elems; j++) {
		p = hash->arData + j;
		if (p->key) {
			zend_string_release_ex(p->key, 0);
		}
		p->h = j;
		p->key = NULL;
	}
	hash->nNextFreeElement = n_elems;
	if (!(HT_FLAGS(hash) & HASH_FLAG_PACKED)) {
		zend_hash_to_packed(hash);
	}
}
/* }}} */

/* {{{ */
void php_random_class_string_shuffle(php_random_class *random_class, zend_object *obj, char *str, zend_long len)
{
	zend_long n_elems, rnd_idx, n_left;
	char temp;
	n_elems = len;

	if (n_elems <= 1) {
		return;
	}

	n_left = n_elems;

	while (--n_left) {
		rnd_idx = php_random_class_range(random_class, obj, 0, n_left);
		if (rnd_idx != n_left) {
			temp = str[n_left];
			str[n_left] = str[rnd_idx];
			str[rnd_idx] = temp;
		}
	}
}
/* }}} */

static zend_object *php_random_class_new(zend_class_entry *ce) {
	php_random_class *random_class = zend_object_alloc(sizeof(php_random_class), ce);
	zend_object_std_init(&random_class->std, ce);
	object_properties_init(&random_class->std, ce);
	random_class->std.handlers = &php_random_class_object_handlers;

	return &random_class->std;
}

static void php_random_class_free_obj(zend_object *object) {
	php_random_class *random_class = random_class_from_obj(object);
	if (random_class->state) {
		efree(random_class->state);
	}
	zend_object_std_dtor(&random_class->std);
}

static zend_object *php_random_class_clone_obj(zend_object *object) {
	zend_object *new_obj;
	php_random_class *new, *old;

	new_obj = php_random_class_new(object->ce);

	zend_objects_clone_members(new_obj, object);

	new = random_class_from_obj(new_obj);
	old = random_class_from_obj(object);
	
	if (old->algo) {
		new->algo = old->algo;
		if (old->algo->init && old->algo->state_size && old->state) {
			new->state = old->algo->init();
			memcpy(new->state, old->state, old->algo->state_size);
		}
	}

	return new_obj;
}

/* XorShift128+ BEGIN */
#define PHP_RANDOM_CLASS_ALGO_XORSHIFT128PLUS "xorshift128plus"

typedef struct _xorshift128plus_state {
	uint64_t s[2];
} xorshift128plus_state;

static inline uint64_t xorshift128plus_splitmix64(uint64_t *seed) {
	uint64_t r;
	r = (*seed += UINT64_C(0x9e3779b97f4a7c15));
	r = (r ^ (r >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
	r = (r ^ (r >> 27)) * UINT64_C(0x94d049bb133111eb);
	return (r ^ (r >> 31));
}

static uint64_t xorshift128plus_next(void *state) {
	xorshift128plus_state *s = (xorshift128plus_state*) state;
	uint64_t s0, s1, r;

	s1 = s->s[0];
	s0 = s->s[1];
	r = s0 + s1;
	s->s[0] = s0;
	s1 ^= s1 << 23;
	s->s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);

	return r;
}

static void* xorshift128plus_init(void) {
	return ecalloc(1, sizeof(xorshift128plus_state));
}

static void xorshift128plus_seed(void *state, const zend_long seed) {
	xorshift128plus_state *s = (xorshift128plus_state*) state;
	uint64_t se = (uint64_t) seed;

	s->s[0] = xorshift128plus_splitmix64(&se);
	s->s[1] = xorshift128plus_splitmix64(&se);
}

static int xorshift128plus_serialize(void *state, zval *return_value) {
	xorshift128plus_state *s = (xorshift128plus_state*) state;
	zval tmp;
	int i;

	for (i = 0; i < 2; i++) {
		ZVAL_STR(&tmp, zend_strpprintf(0, "%" PRIu64, s->s[i]));
		zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &tmp);
	}

	return SUCCESS;
}

static int xorshift128plus_unserialize(void *state, HashTable *data) {
	xorshift128plus_state *s = (xorshift128plus_state*) state;
	zval *tmp;
	int i;

	for (i = 0; i < 2; i++) {
		tmp = zend_hash_index_find(data, i + PHP_RANDOM_CLASS_UNSERIALIZE_PADDING);
		if (!tmp || Z_TYPE_P(tmp) != IS_STRING) {
			return FAILURE;
		}

		s->s[i] = strtoull(ZSTR_VAL(Z_STR_P(tmp)), NULL, 10);
	}

	return SUCCESS;
}

const php_random_class_algo php_random_class_algo_xorshift128plus = {
	PHP_RANDOM_CLASS_ALGO_XORSHIFT128PLUS,
	sizeof(uint64_t),
	sizeof(xorshift128plus_state),
	xorshift128plus_next,
	xorshift128plus_init,
	xorshift128plus_seed,
	xorshift128plus_serialize,
	xorshift128plus_unserialize
};
/* XorShift128+ END */

/* MT19937 BEGIN */
#define PHP_RANDOM_CLASS_ALGO_MT19937 "mt19937"

#define MT19937_N             (624)
#define MT19937_M             (397)

#define MT19937_hiBit(u)      ((u) & 0x80000000U)
#define MT19937_loBit(u)      ((u) & 0x00000001U)
#define MT19937_loBits(u)     ((u) & 0x7FFFFFFFU)
#define MT19937_MixBits(u, v) (MT19937_hiBit(u)|MT19937_loBits(v))
#define MT19937_twist(m,u,v)  (m ^ (MT19937_MixBits(u,v)>>1) ^ ((uint32_t)(-(int32_t)(MT19937_loBit(v))) & 0x9908b0dfU))

typedef struct _mt19937_state {
	uint32_t s[MT19937_N + 1];
	int i;
} mt19937_state;

static inline void mt19937_reload(mt19937_state *state) {
	uint32_t *p = state->s;
	int i;

	for (i = MT19937_N - MT19937_M; i--; ++p) {
		*p = MT19937_twist(p[MT19937_M], p[0], p[1]);
	}
	for (i = MT19937_M; --i; ++p) {
		*p = MT19937_twist(p[MT19937_M - MT19937_N], p[0], p[1]);
	}
	*p = MT19937_twist(p[MT19937_M - MT19937_N], p[0], state->s[0]);

	state->i = 0;
}

static uint64_t mt19937_next(void *state) {
	mt19937_state *s = (mt19937_state *) state;
	uint32_t s1;

	if (s->i >= MT19937_N) {
		mt19937_reload(s);
	}

	s1 = s->s[s->i++];
	s1 ^= (s1 >> 11);
	s1 ^= (s1 << 7) & 0x9d2c5680U;
	s1 ^= (s1 << 15) & 0xefc60000U;
	return ( s1 ^ (s1 >> 18) );
}

static void* mt19937_init(void) {
	return ecalloc(1, sizeof(mt19937_state));
}

static void mt19937_seed(void *state, const zend_long seed) {
	mt19937_state *s = (mt19937_state*) state;

	s->s[0] = seed & 0xffffffffU;
	for (s->i = 1; s->i < MT19937_N; s->i++) {
		s->s[s->i] = (1812433253U * (s->s[s->i - 1] ^ (s->s[s->i - 1] >> 30)) + s->i) & 0xffffffffU;
	}
	mt19937_reload(s);
}

static int mt19937_serialize(void *state, zval *return_value) {
	mt19937_state *s = (mt19937_state *) state;
	zval tmp;
	int i;

	for (i = 0; i < MT19937_N + 1; i++) {
		ZVAL_LONG(&tmp, s->s[i]);
		zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &tmp);
	}
	ZVAL_LONG(&tmp, s->i);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &tmp);

	return SUCCESS;
}

static int mt19937_unserialize(void *state, HashTable *data) {
	mt19937_state *s = (mt19937_state *) state;
	zval *tmp;
	int i;

	for (i = 0; i < MT19937_N + 1; i++) {
		tmp = zend_hash_index_find(data, i + 2);
		if (!tmp || Z_TYPE_P(tmp) != IS_LONG) {
			return FAILURE;
		}

		s->s[i] = Z_LVAL_P(tmp);
	}
	tmp = zend_hash_index_find(data, PHP_RANDOM_CLASS_UNSERIALIZE_PADDING + MT19937_N + 1);
	if (Z_TYPE_P(tmp) != IS_LONG) {
		return FAILURE;
	}
	s->i = Z_LVAL_P(tmp);

	return SUCCESS;
}

const php_random_class_algo php_random_class_algo_mt19937 = {
	PHP_RANDOM_CLASS_ALGO_MT19937,
	sizeof(uint32_t),
	sizeof(mt19937_state),
	mt19937_next,
	mt19937_init,
	mt19937_seed,
	mt19937_serialize,
	mt19937_unserialize
};
/* MT19937 END */

/* secure BEGIN */
#define PHP_RANDOM_CLASS_ALGO_SECURE "secure"

static uint64_t secure_next(void *state) {
	uint64_t ret;

	php_random_bytes_silent(&ret, sizeof(uint64_t));

	return ret;
}

const php_random_class_algo php_random_class_algo_secure = {
	PHP_RANDOM_CLASS_ALGO_SECURE,
	sizeof(uint64_t),
	0,
	secure_next,
	NULL,
	NULL,
	NULL,
	NULL
};
/* secure END */

/* user BEGIN */
#define PHP_RANDOM_CLASS_ALGO_USER_DEFINED "user"
/* user END */

/* {{{ PHP_INI */
PHP_INI_BEGIN()
	STD_PHP_INI_BOOLEAN("random.ignore_generated_size_exceeded", "0", PHP_INI_ALL, OnUpdateBool, random_class_ignore_generated_size_exceeded, php_core_globals, core_globals)
PHP_INI_END()
/* }}} */

/* {{{ */
PHP_METHOD(Random, getAlgos)
{
	zend_string *algo;

	ZEND_PARSE_PARAMETERS_NONE();

	array_init(return_value);
	ZEND_HASH_FOREACH_STR_KEY(&php_random_class_algos, algo) {
		add_next_index_str(return_value, zend_string_copy(algo));
	} ZEND_HASH_FOREACH_END();
}
/* }}} */

/* {{{ */
PHP_METHOD(Random, getAlgoInfo)
{
	zend_string *algo_str;
	const php_random_class_algo *algo;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(algo_str)
	ZEND_PARSE_PARAMETERS_END();

	algo = php_random_class_algo_find(algo_str);
	if (!algo) {
		RETURN_NULL();
	}

	array_init(return_value);
	add_assoc_string(return_value, "algo", algo->ident);
	add_assoc_bool(return_value, "seedable", algo->seed);
	add_assoc_bool(return_value, "serializable", algo->serialize && algo->unserialize);
}
/* }}} */

/* {{{ */
PHP_METHOD(Random, __construct)
{
	zend_string *algo_str = NULL;
	zend_long seed;
	bool seed_is_null = 1;
	const php_random_class_algo *algo;

	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(algo_str)
		Z_PARAM_LONG_OR_NULL(seed, seed_is_null)
	ZEND_PARSE_PARAMETERS_END();

	algo =
		algo_str == NULL
			? &php_random_class_algo_xorshift128plus
			: php_random_class_algo_find(algo_str);

	if (algo) {
		php_random_class *random_class = Z_RANDOM_CLASS_P(ZEND_THIS);
		random_class->algo = algo;

		if (!algo->seed && !seed_is_null) {
			zend_argument_value_error(2, "algorithm does not support seed with value");
			RETURN_THROWS();
		}

		if (algo->init) {
			random_class->state = algo->init();
			if (algo->seed) {
				if (seed_is_null) {
					seed = php_random_bytes_silent(&seed, sizeof(zend_long));
				}

				algo->seed(random_class->state, seed);
			}
		}
	} else if (! zend_string_equals_literal(algo_str, PHP_RANDOM_CLASS_ALGO_USER_DEFINED)) {
		zend_argument_value_error(1, "must be a valid random number generator algorithm");
		RETURN_THROWS();
	} else if (Z_OBJCE_P(ZEND_THIS)->type == ZEND_INTERNAL_CLASS) {
		zend_throw_exception(NULL, "User defined algorithm must be inherited", 0);
		RETURN_THROWS();
	}
}
/* }}} */

/* {{{ */
PHP_METHOD(Random, nextInt)
{
	php_random_class *random_class = Z_RANDOM_CLASS_P(ZEND_THIS);
	uint64_t ret;

	ZEND_PARSE_PARAMETERS_NONE();

	ret = php_random_class_next(random_class, Z_OBJ_P(ZEND_THIS));
	if (random_class->algo) {
		if (random_class->algo->bytes > sizeof(zend_long)) {
			if (PG(random_class_ignore_generated_size_exceeded)) {
				ret = (zend_ulong) ret;
			} else {
				zend_throw_exception(NULL, "Generated random number size exceeded", 0);
				RETURN_THROWS();
			}
		}
		/* right shift for future machine compatibility. */
		ret = ret >> 1;
	}

	RETURN_LONG((zend_long) ret);
}
/* }}} */

/* {{{ */
PHP_METHOD(Random, getInt)
{
	php_random_class *random_class = Z_RANDOM_CLASS_P(ZEND_THIS);
	zend_long min, max;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(min)
		Z_PARAM_LONG(max)
	ZEND_PARSE_PARAMETERS_END();

	if (UNEXPECTED(max < min)) {
		zend_argument_value_error(2, "must be greater than or equal to argument #1 ($min)");
		RETURN_THROWS();
	}

	RETURN_LONG(php_random_class_range(random_class, Z_OBJ_P(ZEND_THIS), min, max));
}
/* }}} */

/* {{{ */
PHP_METHOD(Random, getBytes)
{
	php_random_class *random_class = Z_RANDOM_CLASS_P(ZEND_THIS);
	zend_string *ret;
	zend_long size;
	uint64_t buf;
	uint8_t *bytes;
	size_t generated_bytes = 0;
	int i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(size)
	ZEND_PARSE_PARAMETERS_END();

	if (size < 1) {
		zend_argument_value_error(1, "must be greater than 0");
		RETURN_THROWS();
	}

	ret = zend_string_alloc(size, 0);

	while (generated_bytes <= size) {
		buf = php_random_class_next(random_class, Z_OBJ_P(ZEND_THIS));
		if (random_class->algo && random_class->algo->bytes == sizeof(uint32_t)) {
			buf = (buf << 32) | php_random_class_next(random_class, Z_OBJ_P(ZEND_THIS));
		}
		bytes = (uint8_t *) &buf;
		for (i = 0; i < (sizeof(uint64_t) / sizeof(uint8_t)); i ++) {
			ZSTR_VAL(ret)[generated_bytes + i] = bytes[i];
			
			if ((generated_bytes + i) >= size) {
				ZSTR_VAL(ret)[size] = '\0';
				RETURN_STR(ret);
			}
		}
		generated_bytes += (sizeof(uint64_t) / sizeof(uint8_t));
	}
}
/* }}} */

/* {{{ */
PHP_METHOD(Random, shuffleArray)
{
	php_random_class *random_class = Z_RANDOM_CLASS_P(ZEND_THIS);
	zval *array;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(array)
	ZEND_PARSE_PARAMETERS_END();

	ZVAL_DUP(return_value, array);

	php_random_class_array_data_shuffle(random_class, Z_OBJ_P(ZEND_THIS), return_value);
}
/* }}} */

/* {{{ */
PHP_METHOD(Random, shuffleString)
{
	php_random_class *random_class = Z_RANDOM_CLASS_P(ZEND_THIS);
	zend_string *string;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(string)
	ZEND_PARSE_PARAMETERS_END();

	RETVAL_STRINGL(ZSTR_VAL(string), ZSTR_LEN(string));
	if (Z_STRLEN_P(return_value) > 1) {
		php_random_class_string_shuffle(random_class, Z_OBJ_P(ZEND_THIS), Z_STRVAL_P(return_value), (zend_long) Z_STRLEN_P(return_value));
	}
}
/* }}} */

/* {{{ */
PHP_METHOD(Random, __serialize)
{
	php_random_class *intern = Z_RANDOM_CLASS_P(ZEND_THIS);
	zval tmp;

	ZEND_PARSE_PARAMETERS_NONE();

	if (Z_OBJCE_P(ZEND_THIS)->ce_flags & ZEND_ACC_ANON_CLASS) {
		zend_throw_exception(NULL, "Anonymous class serialization is not allowed", 0);
		RETURN_THROWS();
	}

	if (intern->algo && (!intern->algo->serialize || !intern->algo->unserialize)) {
		zend_throw_exception(NULL, "Algorithm does not support serialization", 0);
		RETURN_THROWS();
	}

	array_init(return_value);

	/* members */
	ZVAL_ARR(&tmp, zend_std_get_properties(&intern->std));
	Z_TRY_ADDREF(tmp);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &tmp);

	/* algo */
	ZVAL_STRING(&tmp, intern->algo ? intern->algo->ident : PHP_RANDOM_CLASS_ALGO_USER_DEFINED);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &tmp);

	/* state */
	if (intern->algo && intern->algo->serialize) {
		intern->algo->serialize(intern->state, return_value);		
	}
}
/* }}} */

/* {{{ */
PHP_METHOD(Random, __unserialize)
{
	php_random_class *intern = Z_RANDOM_CLASS_P(ZEND_THIS);
	const php_random_class_algo *algo;
	HashTable *data;
	zval *tmp, *members_zv;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "h", &data) == FAILURE) {
		RETURN_THROWS();
	}

	/* members */
	members_zv = zend_hash_index_find(data, 0);
	if (!members_zv || Z_TYPE_P(members_zv) != IS_ARRAY) {
		zend_throw_exception(NULL, "Incomplete or ill-formed serialization data", 0);
		RETURN_THROWS();
	}
	object_properties_load(&intern->std, Z_ARRVAL_P(members_zv));

	/* state */
	tmp = zend_hash_index_find(data, 1);
	if (! zend_string_equals_literal(Z_STR_P(tmp), PHP_RANDOM_CLASS_ALGO_USER_DEFINED)) {
		algo = php_random_class_algo_find(Z_STR_P(tmp));
		if (!algo) {
			zend_throw_exception(NULL, "Algorithm does not registered", 0);
			RETURN_THROWS();
		}

		intern->algo = algo;

		if (algo->init) {
			intern->state = algo->init();
		}

		if (!algo->serialize || !algo->unserialize) {
			zend_throw_exception(NULL, "Algorithm does not support serialization", 0);
			RETURN_THROWS();
		}

		if (algo->unserialize(intern->state, data) == FAILURE) {
			zend_throw_exception(NULL, "Unserialize failed", 0);
			RETURN_THROWS();
		}
	}
}
/* }}} */

/* {{{ */
PHP_METHOD(Random, next)
{
	zend_throw_exception(NULL, "Must be override the method next(): int", 0);
	RETURN_THROWS();
}
/* }}} */

PHP_MINIT_FUNCTION(random_class)
{
	zend_class_entry ce;

	REGISTER_INI_ENTRIES();

	zend_hash_init(&php_random_class_algos, 1, NULL, ZVAL_PTR_DTOR, 1);

	/* XorShift128+ */
	if (FAILURE == php_random_class_algo_register(&php_random_class_algo_xorshift128plus)) { return FAILURE; }
	REGISTER_STRING_CONSTANT("RANDOM_XORSHIFT128PLUS", php_random_class_algo_xorshift128plus.ident, CONST_CS | CONST_PERSISTENT);

	/* MT19937 */
	if (FAILURE == php_random_class_algo_register(&php_random_class_algo_mt19937)) { return FAILURE; }
	REGISTER_STRING_CONSTANT("RANDOM_MT19937", php_random_class_algo_mt19937.ident, CONST_CS | CONST_PERSISTENT);

	/* secure */
	if (FAILURE == php_random_class_algo_register(&php_random_class_algo_secure)) { return FAILURE; }
	REGISTER_STRING_CONSTANT("RANDOM_SECURE", php_random_class_algo_secure.ident, CONST_CS | CONST_PERSISTENT);

	/* user */
	REGISTER_STRING_CONSTANT("RANDOM_USER", PHP_RANDOM_CLASS_ALGO_USER_DEFINED, CONST_CS | CONST_PERSISTENT);

	INIT_CLASS_ENTRY(ce, "Random", class_Random_methods);
	php_random_class_ce = zend_register_internal_class(&ce);
	php_random_class_ce->create_object = php_random_class_new;
	memcpy(&php_random_class_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	php_random_class_object_handlers.offset = XtOffsetOf(php_random_class, std);
	php_random_class_object_handlers.free_obj = php_random_class_free_obj;
	php_random_class_object_handlers.clone_obj = php_random_class_clone_obj;

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(random_class)
{
	UNREGISTER_INI_ENTRIES();

	zend_hash_destroy(&php_random_class_algos);
	return SUCCESS;
}

PHP_MINFO_FUNCTION(random_class)
{
	DISPLAY_INI_ENTRIES();
}
