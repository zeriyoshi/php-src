/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 754941c0796f23cbb80f6754bad505ede371386c */

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_RNG_XorShift128Plus___construct, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, seed, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_RNG_XorShift128Plus_next, 0, 0, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_RNG_XorShift128Plus_arrayShuffle, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(1, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_RNG_XorShift128Plus_stringShuffle, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_class_RNG_XorShift128Plus_arrayRandom, 0, 1, MAY_BE_LONG|MAY_BE_STRING|MAY_BE_ARRAY)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, num, IS_LONG, 0, "1")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_RNG_XorShift128Plus___serialize, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_RNG_XorShift128Plus___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(RNG_XorShift128Plus, __construct);
ZEND_METHOD(RNG_XorShift128Plus, next);
ZEND_METHOD(RNG_XorShift128Plus, arrayShuffle);
ZEND_METHOD(RNG_XorShift128Plus, stringShuffle);
ZEND_METHOD(RNG_XorShift128Plus, arrayRandom);
ZEND_METHOD(RNG_XorShift128Plus, __serialize);
ZEND_METHOD(RNG_XorShift128Plus, __unserialize);


static const zend_function_entry class_RNG_XorShift128Plus_methods[] = {
	ZEND_ME(RNG_XorShift128Plus, __construct, arginfo_class_RNG_XorShift128Plus___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(RNG_XorShift128Plus, next, arginfo_class_RNG_XorShift128Plus_next, ZEND_ACC_PUBLIC)
	ZEND_ME(RNG_XorShift128Plus, arrayShuffle, arginfo_class_RNG_XorShift128Plus_arrayShuffle, ZEND_ACC_PUBLIC)
	ZEND_ME(RNG_XorShift128Plus, stringShuffle, arginfo_class_RNG_XorShift128Plus_stringShuffle, ZEND_ACC_PUBLIC)
	ZEND_ME(RNG_XorShift128Plus, arrayRandom, arginfo_class_RNG_XorShift128Plus_arrayRandom, ZEND_ACC_PUBLIC)
	ZEND_ME(RNG_XorShift128Plus, __serialize, arginfo_class_RNG_XorShift128Plus___serialize, ZEND_ACC_PUBLIC)
	ZEND_ME(RNG_XorShift128Plus, __unserialize, arginfo_class_RNG_XorShift128Plus___unserialize, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};
