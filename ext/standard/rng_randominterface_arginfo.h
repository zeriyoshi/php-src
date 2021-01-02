/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b0aa88c061dc483a40e9436896e7e615b48d03d5 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_RNG_RandomInterface_arrayShuffle, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_RNG_RandomInterface_stringShuffle, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_RNG_RandomInterface_arrayRandom, 0, 1, MAY_BE_LONG|MAY_BE_STRING|MAY_BE_ARRAY)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, num, IS_LONG, 0, "1")
ZEND_END_ARG_INFO()




static const zend_function_entry class_RNG_RandomInterface_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(RNG_RandomInterface, arrayShuffle, arginfo_class_RNG_RandomInterface_arrayShuffle, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(RNG_RandomInterface, stringShuffle, arginfo_class_RNG_RandomInterface_stringShuffle, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_ABSTRACT_ME_WITH_FLAGS(RNG_RandomInterface, arrayRandom, arginfo_class_RNG_RandomInterface_arrayRandom, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};
