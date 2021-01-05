/*
   +----------------------------------------------------------------------+
   | Copyright (c) The PHP Group                                          |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Go Kudo <zeriyoshi@gmail.com>                               |
   +----------------------------------------------------------------------+
*/

#include "php.h"
#include "zend_bitset.h"
#include "php_rng.h"

#include "zend_exceptions.h"

#include "rng_xorshift128plus.h"
#include "rng_xorshift128plus_arginfo.h"

#include "rng_randominterface.h"

PHPAPI zend_class_entry *ce_RNG_XorShift128Plus;

static zend_object_handlers XorShift128Plus_handlers;

static uint64_t splitmix64(uint64_t *seed)
{
	uint64_t r;
	r = (*seed += UINT64_C(0x9e3779b97f4a7c15));
	r = (r ^ (r >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
	r = (r ^ (r >> 27)) * UINT64_C(0x94d049bb133111eb);
	return (r ^ (r >> 31));
}

static uint64_t next(php_rng *rng)
{
	uint64_t s0, s1, r;

	rng_RNG_XorShift128Plus_obj *obj = ((rng_RNG_XorShift128Plus_obj*)rng->obj);

	s1 = obj->s[0];
	s0 = obj->s[1];
	r = s0 + s1;
	obj->s[0] = s0;
	s1 ^= s1 << 23;
	obj->s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);

	return r;
}

static zend_object *rng_object_new(zend_class_entry *ce)
{
	rng_RNG_XorShift128Plus_obj *obj = (rng_RNG_XorShift128Plus_obj*)ecalloc(1, sizeof(rng_RNG_XorShift128Plus_obj) + zend_object_properties_size(ce));
	php_rng *rng = php_rng_initialize(next, obj);
	obj->rng = rng;
	zend_object_std_init(&obj->std, ce);
	object_properties_init(&obj->std, ce);
	obj->std.handlers = &XorShift128Plus_handlers;
	
	return &obj->std;
}

static void free_object_storage(zend_object *object)
{
	rng_RNG_XorShift128Plus_obj *obj = RNG_XorShift128Plus_from_obj(object);
	zend_object_std_dtor(&obj->std);
	if (obj->rng != NULL) {
		efree(obj->rng);
	}
}

PHP_METHOD(RNG_XorShift128Plus, __construct)
{
	zend_long seed;
	uint64_t s;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(seed);
	ZEND_PARSE_PARAMETERS_END();

	rng_RNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	s = (uint64_t) seed;
	obj->s[0] = splitmix64(&s);
	obj->s[1] = splitmix64(&s);
}

PHP_METHOD(RNG_XorShift128Plus, next)
{
	zend_long min, max;
	rng_RNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());
	
	if (ZEND_NUM_ARGS() == 0) {
		RETURN_LONG(php_rng_next(obj->rng));
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
	  Z_PARAM_LONG(min)
	  Z_PARAM_LONG(max)
	ZEND_PARSE_PARAMETERS_END();

	if (UNEXPECTED(max < min)) {
		zend_argument_value_error(2, "must be greater than or equal to argument #1 ($min)");
		RETURN_THROWS();
	}

	RETURN_LONG(php_rng_range(obj->rng, min, max));
}

PHP_METHOD(RNG_XorShift128Plus, arrayShuffle)
{
	zval *array;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY_EX(array, 0, 1)
	ZEND_PARSE_PARAMETERS_END();

	rng_RNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	php_rng_array_shuffle(obj->rng, array);

	RETURN_TRUE;
}

PHP_METHOD(RNG_XorShift128Plus, stringShuffle)
{
	zend_string *arg;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(arg)
	ZEND_PARSE_PARAMETERS_END();

	RETVAL_STRINGL(ZSTR_VAL(arg), ZSTR_LEN(arg));

	rng_RNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	if (Z_STRLEN_P(return_value) > 1) {
		php_rng_string_shuffle(obj->rng, Z_STRVAL_P(return_value), (zend_long) Z_STRLEN_P(return_value));
	}
}

PHP_METHOD(RNG_XorShift128Plus, arrayRandom)
{
	zval *input;
	zend_long num_req = 1;
	zend_string *string_key;
	zend_ulong num_key;
	int i;
	int num_avail;
	zend_bitset bitset;
	int negative_bitset = 0;
	uint32_t bitset_len;
	ALLOCA_FLAG(use_heap)

	rng_RNG_XorShift128Plus_obj *obj = Z_XorShift128Plus_P(getThis());

	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_ARRAY(input)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(num_req)
	ZEND_PARSE_PARAMETERS_END();

	num_avail = zend_hash_num_elements(Z_ARRVAL_P(input));

	if (num_avail == 0) {
		zend_argument_value_error(1, "cannot be empty");
		RETURN_THROWS();
	}

	if (num_req == 1) {
		HashTable *ht = Z_ARRVAL_P(input);

		if ((uint32_t)num_avail < ht->nNumUsed - (ht->nNumUsed>>1)) {
			/* If less than 1/2 of elements are used, don't sample. Instead search for a
			 * specific offset using linear scan. */
			zend_long i = 0, randval = php_rng_range(obj->rng, 0, num_avail - 1);
			ZEND_HASH_FOREACH_KEY(Z_ARRVAL_P(input), num_key, string_key) {
				if (i == randval) {
					if (string_key) {
						RETURN_STR_COPY(string_key);
					} else {
						RETURN_LONG(num_key);
					}
				}
				i++;
			} ZEND_HASH_FOREACH_END();
		}

		/* Sample random buckets until we hit one that is not empty.
		 * The worst case probability of hitting an empty element is 1-1/2. The worst case
		 * probability of hitting N empty elements in a row is (1-1/2)**N.
		 * For N=10 this becomes smaller than 0.1%. */
		do {
			zend_long randval = php_rng_range(obj->rng, 0, ht->nNumUsed - 1);
			Bucket *bucket = &ht->arData[randval];
			if (!Z_ISUNDEF(bucket->val)) {
				if (bucket->key) {
					RETURN_STR_COPY(bucket->key);
				} else {
					RETURN_LONG(bucket->h);
				}
			}
		} while (1);
	}

	if (num_req <= 0 || num_req > num_avail) {
		zend_argument_value_error(2, "must be between 1 and the number of elements in argument #1 ($array)");
		RETURN_THROWS();
	}

	/* Make the return value an array only if we need to pass back more than one result. */
	array_init_size(return_value, (uint32_t)num_req);
	if (num_req > (num_avail >> 1)) {
		negative_bitset = 1;
		num_req = num_avail - num_req;
	}

	bitset_len = zend_bitset_len(num_avail);
	bitset = ZEND_BITSET_ALLOCA(bitset_len, use_heap);
	zend_bitset_clear(bitset, bitset_len);

	i = num_req;
	while (i) {
		zend_long randval = php_rng_range(obj->rng, 0, num_avail - 1);
		if (!zend_bitset_in(bitset, randval)) {
			zend_bitset_incl(bitset, randval);
			i--;
		}
	}
	/* i = 0; */

	zend_hash_real_init_packed(Z_ARRVAL_P(return_value));
	ZEND_HASH_FILL_PACKED(Z_ARRVAL_P(return_value)) {
		/* We can't use zend_hash_index_find()
		 * because the array may have string keys or gaps. */
		ZEND_HASH_FOREACH_KEY(Z_ARRVAL_P(input), num_key, string_key) {
			if (zend_bitset_in(bitset, i) ^ negative_bitset) {
				if (string_key) {
					ZEND_HASH_FILL_SET_STR_COPY(string_key);
				} else {
					ZEND_HASH_FILL_SET_LONG(num_key);
				}
				ZEND_HASH_FILL_NEXT();
			}
			i++;
		} ZEND_HASH_FOREACH_END();
	} ZEND_HASH_FILL_END();

	free_alloca(bitset, use_heap);
}

PHP_METHOD(RNG_XorShift128Plus, __serialize)
{
	rng_RNG_XorShift128Plus_obj *intern = Z_XorShift128Plus_P(ZEND_THIS);
	zval tmp;
	int i;

	if (zend_parse_parameters_none() == FAILURE) {
		RETURN_THROWS();
	}

	array_init(return_value);
	
	/* state */
	for (i = 0; i < 2; i++) {
		ZVAL_STR(&tmp, zend_strpprintf(0, "%" PRIu64, intern->s[i]));
		zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &tmp);
	}

	/* members */
	ZVAL_ARR(&tmp, zend_std_get_properties(&intern->std));
	Z_TRY_ADDREF(tmp);
	zend_hash_next_index_insert(Z_ARRVAL_P(return_value), &tmp);
}

PHP_METHOD(RNG_XorShift128Plus, __unserialize)
{
	rng_RNG_XorShift128Plus_obj *intern = Z_XorShift128Plus_P(ZEND_THIS);
	HashTable *data;
	zval *tmp, *members_zv;
	int i;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "h", &data) == FAILURE) {
		RETURN_THROWS();
	}

	for (i = 0; i < 2; i++) {
		tmp = zend_hash_index_find(data, i);
		if (Z_TYPE_P(tmp) != IS_STRING) {
			zend_throw_exception(NULL, "Incomplete or ill-formed serialization data", 0);
			RETURN_THROWS();
		}

		intern->s[i] = strtoull(ZSTR_VAL(Z_STR_P(tmp)), NULL, 10);
	}
	members_zv = zend_hash_index_find(data, 2);
	if (!members_zv || Z_TYPE_P(members_zv) != IS_ARRAY) {
		zend_throw_exception(NULL, "Incomplete or ill-formed serialization data", 0);
		RETURN_THROWS();
	}

	object_properties_load(&intern->std, Z_ARRVAL_P(members_zv));
}

PHP_MINIT_FUNCTION(rng_xorshift128plus)
{
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "RNG\\XorShift128Plus", class_RNG_XorShift128Plus_methods);
	ce_RNG_XorShift128Plus = zend_register_internal_class(&ce);
	zend_class_implements(ce_RNG_XorShift128Plus, 1, rng_ce_RNG_RandomInterface);
	ce_RNG_XorShift128Plus->create_object = rng_object_new;
	memcpy(&XorShift128Plus_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	XorShift128Plus_handlers.offset = XtOffsetOf(rng_RNG_XorShift128Plus_obj, std);
	XorShift128Plus_handlers.free_obj = free_object_storage;

	return SUCCESS;
}
