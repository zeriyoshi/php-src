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
   | Authors: Go Kudo <zeriyoshi@gmail.com                                |
   +----------------------------------------------------------------------+
*/

#ifndef PHP_RNG_H
#define PHP_RNG_H

#include "php.h"

# ifdef PHP_WIN32
#  ifdef PHP_RNG_EXPORTS
#   define PHP_RNG_API __declspec(dllexport)
#  else
#   define PHP_RNG_API
#  endif
# elif defined(__GNUC__) && __GNUC__ >= 4
#  define PHP_RNG_API __attribute__ ((visibility("default")))
# else
#  define PHP_RNG_API
# endif

typedef struct _php_rng php_rng;
typedef struct _php_rng {
	uint64_t (*next)(php_rng*);
	void *obj;
} php_rng;

PHP_MINIT_FUNCTION(rng);

PHP_RNG_API php_rng *php_rng_initialize(uint64_t (*next)(php_rng*), void*);
PHP_RNG_API zend_long php_rng_next(php_rng*);
PHP_RNG_API zend_long php_rng_range(php_rng*, zend_long, zend_long);
PHP_RNG_API void php_rng_array_shuffle(php_rng*, zval*);
PHP_RNG_API void php_rng_string_shuffle(php_rng*, char*, zend_long);

#endif
