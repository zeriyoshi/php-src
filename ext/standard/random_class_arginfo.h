/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 20accb7e441c69f98048ed03f862f65bdf578a42 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_RandomNumberGenerator_generate, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Random_getAlgos, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Random_getAlgoInfo, 0, 1, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, algo, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_class_Random___construct, 0, 0, 0)
	ZEND_ARG_OBJ_TYPE_MASK(0, algo, RandomNumberGenerator, MAY_BE_STRING, "RANDOM_XORSHIFT128PLUS")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, seed, IS_LONG, 1, "null")
ZEND_END_ARG_INFO()

#define arginfo_class_Random_nextInt arginfo_class_RandomNumberGenerator_generate

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Random_getInt, 0, 2, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, min, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, max, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Random_getBytes, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Random_shuffleArray, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, array, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Random_shuffleString, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_class_Random___serialize arginfo_class_Random_getAlgos

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_class_Random___unserialize, 0, 1, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, data, IS_ARRAY, 0)
ZEND_END_ARG_INFO()


ZEND_METHOD(Random, getAlgos);
ZEND_METHOD(Random, getAlgoInfo);
ZEND_METHOD(Random, __construct);
ZEND_METHOD(Random, nextInt);
ZEND_METHOD(Random, getInt);
ZEND_METHOD(Random, getBytes);
ZEND_METHOD(Random, shuffleArray);
ZEND_METHOD(Random, shuffleString);
ZEND_METHOD(Random, __serialize);
ZEND_METHOD(Random, __unserialize);


static const zend_function_entry class_RandomNumberGenerator_methods[] = {
	ZEND_ABSTRACT_ME_WITH_FLAGS(RandomNumberGenerator, generate, arginfo_class_RandomNumberGenerator_generate, ZEND_ACC_PUBLIC|ZEND_ACC_ABSTRACT)
	ZEND_FE_END
};


static const zend_function_entry class_Random_methods[] = {
	ZEND_ME(Random, getAlgos, arginfo_class_Random_getAlgos, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Random, getAlgoInfo, arginfo_class_Random_getAlgoInfo, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	ZEND_ME(Random, __construct, arginfo_class_Random___construct, ZEND_ACC_PUBLIC)
	ZEND_ME(Random, nextInt, arginfo_class_Random_nextInt, ZEND_ACC_PUBLIC)
	ZEND_ME(Random, getInt, arginfo_class_Random_getInt, ZEND_ACC_PUBLIC)
	ZEND_ME(Random, getBytes, arginfo_class_Random_getBytes, ZEND_ACC_PUBLIC)
	ZEND_ME(Random, shuffleArray, arginfo_class_Random_shuffleArray, ZEND_ACC_PUBLIC)
	ZEND_ME(Random, shuffleString, arginfo_class_Random_shuffleString, ZEND_ACC_PUBLIC)
	ZEND_ME(Random, __serialize, arginfo_class_Random___serialize, ZEND_ACC_PUBLIC)
	ZEND_ME(Random, __unserialize, arginfo_class_Random___unserialize, ZEND_ACC_PUBLIC)
	ZEND_FE_END
};

static zend_class_entry *register_class_RandomNumberGenerator(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "RandomNumberGenerator", class_RandomNumberGenerator_methods);
	class_entry = zend_register_internal_interface(&ce);

	return class_entry;
}

static zend_class_entry *register_class_Random(void)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "Random", class_Random_methods);
	class_entry = zend_register_internal_class_ex(&ce, NULL);
	class_entry->ce_flags |= ZEND_ACC_FINAL;

	zend_string *property_rng_class_RandomNumberGenerator = zend_string_init("RandomNumberGenerator", sizeof("RandomNumberGenerator")-1, 1);
	zval property_rng_default_value;
	ZVAL_UNDEF(&property_rng_default_value);
	zend_string *property_rng_name = zend_string_init("rng", sizeof("rng") - 1, 1);
	zend_declare_typed_property(class_entry, property_rng_name, &property_rng_default_value, ZEND_ACC_PRIVATE, NULL, (zend_type) ZEND_TYPE_INIT_CLASS(property_rng_class_RandomNumberGenerator, 0, MAY_BE_NULL));
	zend_string_release(property_rng_name);

	return class_entry;
}
