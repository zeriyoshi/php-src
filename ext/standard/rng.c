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

#include <stdlib.h>

#include "php.h"
#include "php_rng.h"

#include "rng_rnginterface.h"
#include "rng_randominterface.h"
#include "rng_xorshift128plus.h"

PHP_RNG_API php_rng* php_rng_initialize(uint64_t (*next)(php_rng*), void *obj)
{
	/* Initializing common RNG struct */
	php_rng *rng = (php_rng*)ecalloc(1, sizeof(php_rng));
	rng->next = next;
	rng->obj = obj;

	return rng;
}

PHP_RNG_API zend_long php_rng_next(php_rng *rng)
{
	uint64_t next = rng->next(rng);

#if ZEND_ULONG_MAX > UINT32_MAX
	return (zend_long) (next >> 1);
#endif
	return (zend_long) (((uint32_t) next) >> 1);
}

PHP_RNG_API zend_long php_rng_range(php_rng *rng, zend_long min, zend_long max)
{
	/* The implementation is stolen from php_mt_rand_range() */
	zend_ulong umax = max - min;
	zend_ulong result, limit;

	result = rng->next(rng);

	/* Special case where no modulus is required */
	if (UNEXPECTED(umax == ZEND_ULONG_MAX)) {
		return result;
	}

	/* Powers of two are not biased */
	if ((umax & (umax - 1)) == 0) {
		return result & (umax - 1);
	}

	/* Ceiling under which ZEND_ULONG_MAX % max == 0 */
	limit = ZEND_ULONG_MAX - (ZEND_ULONG_MAX % umax) - 1;

	/* Discard numbers over the limit to avoid modulo bias */
	while (UNEXPECTED(result > limit)) {
		result = rng->next(rng);
	}

	return (zend_long) (result % umax) + min;
}

PHP_RNG_API void php_rng_array_shuffle(php_rng *rng, zval *array)
{
	/* The implementation is stolen from php_array_data_shuffle() */
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
			rnd_idx = php_rng_range(rng, 0, n_left);
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
			rnd_idx = php_rng_range(rng, 0, n_left);
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

PHP_RNG_API void php_rng_string_shuffle(php_rng *rng, char *str, zend_long len)
{
	/* The implementation is stolen from php_string_shuffle() */

	zend_long n_elems, rnd_idx, n_left;
	char temp;
	n_elems = len;

	if (n_elems <= 1) {
		return;
	}

	n_left = n_elems;

	while (--n_left) {
		rnd_idx = php_rng_range(rng, 0, n_left);
		if (rnd_idx != n_left) {
			temp = str[n_left];
			str[n_left] = str[rnd_idx];
			str[rnd_idx] = temp;
		}
	}
}

PHP_MINIT_FUNCTION(rng)
{
	PHP_MINIT(rng_rnginterface)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(rng_randominterface)(INIT_FUNC_ARGS_PASSTHRU);
   	PHP_MINIT(rng_xorshift128plus)(INIT_FUNC_ARGS_PASSTHRU);
	return SUCCESS;
}
