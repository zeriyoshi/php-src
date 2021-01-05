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

#include "rng_randominterface.h"
#include "rng_randominterface_arginfo.h"

PHPAPI zend_class_entry *rng_ce_RNG_RandomInterface;

PHP_MINIT_FUNCTION(rng_randominterface)
{
    zend_class_entry ce;

    INIT_CLASS_ENTRY(ce, "RNG\\RandomInterface", class_RNG_RandomInterface_methods);
    rng_ce_RNG_RandomInterface = zend_register_internal_interface(&ce);
    zend_class_implements(rng_ce_RNG_RandomInterface, 1, rng_ce_RNG_RNGInterface);
    
    return SUCCESS;
}
