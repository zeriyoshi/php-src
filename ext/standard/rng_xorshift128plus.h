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
#ifndef _RNG_XORSHIFT128PLUS_H
#define _RNG_XORSHIFT128PLUS_H

#include "php.h"
#include "php_rng.h"

extern PHPAPI zend_class_entry *rng_ce_RNG_XorShift128Plus;

typedef struct _rng_RNG_XorShift128Plus_obj {
	uint64_t s[2];
	php_rng *rng;
	zend_object std;
} rng_RNG_XorShift128Plus_obj;

static inline rng_RNG_XorShift128Plus_obj *RNG_XorShift128Plus_from_obj(zend_object *obj) {
	return (rng_RNG_XorShift128Plus_obj*)((char*)(obj) - XtOffsetOf(rng_RNG_XorShift128Plus_obj, std));
}

#define Z_XorShift128Plus_P(zval) RNG_XorShift128Plus_from_obj(Z_OBJ_P(zval))

PHP_MINIT_FUNCTION(rng_xorshift128plus);

#endif
