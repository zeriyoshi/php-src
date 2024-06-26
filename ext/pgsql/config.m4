PHP_ARG_WITH([pgsql],
  [for PostgreSQL support],
  [AS_HELP_STRING([[--with-pgsql[=DIR]]],
    [Include PostgreSQL support. DIR is the PostgreSQL base install directory or
    the path to pg_config])])

if test "$PHP_PGSQL" != "no"; then
  PHP_EXPAND_PATH($PGSQL_INCLUDE, PGSQL_INCLUDE)

  dnl pg_config is still the default way to retrieve build options
  dnl pkgconfig support was only introduced in 9.3

  AC_MSG_CHECKING(for pg_config)
  for i in $PHP_PGSQL $PHP_PGSQL/bin /usr/local/pgsql/bin /usr/local/bin /usr/bin ""; do
	if test -x $i/pg_config; then
      PG_CONFIG="$i/pg_config"
      break;
    fi
  done

  if test -n "$PG_CONFIG"; then
    AC_MSG_RESULT([$PG_CONFIG])
    PGSQL_INCLUDE=`$PG_CONFIG --includedir`
    PGSQL_LIBDIR=`$PG_CONFIG --libdir`
  else
    AC_MSG_RESULT(not found)
    if test "$PHP_PGSQL" = "yes"; then
      PGSQL_SEARCH_PATHS="/usr /usr/local /usr/local/pgsql"
    else
      PGSQL_SEARCH_PATHS=$PHP_PGSQL
    fi

    for i in $PGSQL_SEARCH_PATHS; do
      for j in include include/pgsql include/postgres include/postgresql ""; do
        if test -r "$i/$j/libpq-fe.h"; then
          PGSQL_INC_BASE=$i
          PGSQL_INCLUDE=$i/$j
        fi
      done

      for j in lib $PHP_LIBDIR/pgsql $PHP_LIBDIR/postgres $PHP_LIBDIR/postgresql ""; do
        if test -f "$i/$j/libpq.so" || test -f "$i/$j/libpq.a"; then
          PGSQL_LIBDIR=$i/$j
        fi
      done
    done
  fi

  if test -z "$PGSQL_INCLUDE"; then
    AC_MSG_ERROR(Cannot find libpq-fe.h. Please specify correct PostgreSQL installation path)
  fi

  if test -z "$PGSQL_LIBDIR"; then
    AC_MSG_ERROR(Cannot find libpq.so. Please specify correct PostgreSQL installation path)
  fi

  if test -z "$PGSQL_INCLUDE" && test -z "$PGSQL_LIBDIR"; then
    AC_MSG_ERROR([Unable to find libpq anywhere under $PGSQL_SEARCH_PATHS])
  fi

  AC_DEFINE(HAVE_PGSQL,1,[Whether to build PostgreSQL support or not])
  old_LIBS=$LIBS
  old_LDFLAGS=$LDFLAGS
  LDFLAGS="-L$PGSQL_LIBDIR $LDFLAGS"
  old_CFLAGS=$CFLAGS
  CFLAGS="$CFLAGS -I$PGSQL_INCLUDE"
  AC_CHECK_LIB(pq, PQlibVersion,, AC_MSG_ERROR([Unable to build the PostgreSQL extension: at least libpq 9.1 is required]))
  AC_CHECK_LIB(pq, lo_truncate64, AC_DEFINE(HAVE_PG_LO64,1,[PostgreSQL 9.3 or later]))
  AC_CHECK_LIB(pq, PQsetErrorContextVisibility, AC_DEFINE(HAVE_PG_CONTEXT_VISIBILITY,1,[PostgreSQL 9.6 or later]))
  AC_CHECK_LIB(pq, PQresultMemorySize, AC_DEFINE(HAVE_PG_RESULT_MEMORY_SIZE,1,[PostgreSQL 12 or later]))
  AC_CHECK_LIB(pq, PQchangePassword, AC_DEFINE(HAVE_PG_CHANGE_PASSWORD,1,[PostgreSQL 17 or later]))
  AC_CHECK_LIB(pq, PQsocketPoll, AC_DEFINE(HAVE_PG_SOCKET_POLL,1,[PostgreSQL 17 or later]))
  AC_CHECK_LIB(pq, PQsetChunkedRowsMode, AC_DEFINE(HAVE_PG_SET_CHUNKED_ROWS_SIZE,1,[PostgreSQL 17 or later]))

  dnl Available since PostgreSQL 12.
  AC_CACHE_CHECK([if PGVerbosity enum has PQERRORS_SQLSTATE],
    [php_cv_enum_pgverbosity_pqerrors_sqlstate],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <libpq-fe.h>],
      [PGVerbosity e = PQERRORS_SQLSTATE; (void)e;])],
      [php_cv_enum_pgverbosity_pqerrors_sqlstate=yes],
      [php_cv_enum_pgverbosity_pqerrors_sqlstate=no])])
  AS_VAR_IF([php_cv_enum_pgverbosity_pqerrors_sqlstate], [yes],
    [AC_DEFINE([HAVE_PQERRORS_SQLSTATE], [1],
      [Define to 1 if PGVerbosity enum has PQERRORS_SQLSTATE.])])

  LIBS=$old_LIBS
  LDFLAGS=$old_LDFLAGS
  CFLAGS=$old_CFLAGS

  PHP_ADD_LIBRARY_WITH_PATH(pq, $PGSQL_LIBDIR, PGSQL_SHARED_LIBADD)
  PHP_SUBST(PGSQL_SHARED_LIBADD)

  PHP_ADD_INCLUDE($PGSQL_INCLUDE)

  PHP_NEW_EXTENSION(pgsql, pgsql.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
  PHP_ADD_EXTENSION_DEP(pgsql, pcre)
fi
