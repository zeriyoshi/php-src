dnl
dnl Check for strtoull
dnl
AC_CHECK_FUNCS(strtoull)

dnl
dnl Setup extension
dnl
PHP_NEW_EXTENSION(random, csprng.c mt_rand.c rand.c random.c , , , -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
PHP_INSTALL_HEADERS([ext/random/])
