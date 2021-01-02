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
#include "php_rng.h"

#include "rng_rnginterface.h"
#include "rng_rnginterface_arginfo.h"

PHP_RNG_API zend_class_entry *rng_ce_RNG_RNGInterface;

PHP_MINIT_FUNCTION(rng_rnginterface) /* {{{ */
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "RNG\\RNGInterface", class_RNG_RNGInterface_methods);
    rng_ce_RNG_RNGInterface = zend_register_internal_interface(&ce);

    return SUCCESS;
} /* }}} */
