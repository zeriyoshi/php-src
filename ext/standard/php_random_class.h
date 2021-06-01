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

#ifndef PHP_RANDOM_CLASS_H
#define PHP_RANDOM_CLASS_H

PHP_MINIT_FUNCTION(random_class);
PHP_MSHUTDOWN_FUNCTION(random_class);

extern PHPAPI zend_class_entry *php_random_class_ce_RandomNumberGenerator;
extern PHPAPI zend_class_entry *php_random_class_ce_Random;

typedef struct _php_random_class_algo {
	const char* ident;
	const size_t generate_size;
	const size_t state_size;
	uint64_t (*next)(void *state);
	void (*seed)(void *state, const zend_long seed);
	int (*serialize)(void *state, zval *return_value);
	int (*unserialize)(void *state, HashTable *data);
} php_random_class_algo;

typedef struct _php_random_class {
	const php_random_class_algo *algo;
	void *state;
	zend_object std;
} php_random_class;

static inline php_random_class *random_class_from_obj(zend_object *obj) {
	return (php_random_class *)((char *)(obj) - XtOffsetOf(php_random_class, std));
}

#define Z_RANDOM_CLASS_P(zval) random_class_from_obj(Z_OBJ_P(zval))

#define PHP_RANDOM_CLASS_UNSERIALIZE_PADDING 2

PHPAPI int php_random_class_algo_register(const php_random_class_algo *algo);
PHPAPI void php_random_class_algo_unregister(const char *ident);
PHPAPI const php_random_class_algo* php_random_class_algo_find(const zend_string *ident);
PHPAPI uint64_t php_random_class_next(php_random_class *random_class);
PHPAPI zend_long php_random_class_range(php_random_class *random_class, zend_long min, zend_long max);
PHPAPI void php_random_class_array_data_shuffle(php_random_class *random_class, zval *array);
PHPAPI void php_random_class_string_shuffle(php_random_class *random_class, char *str, zend_long len);

#endif
