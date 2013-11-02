dnl $Id$
dnl config.m4 for extension zh2py

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(zh2py, for zh2py support,
dnl Make sure that the comment is aligned:
dnl [  --with-zh2py             Include zh2py support])

dnl Otherwise use enable:

 PHP_ARG_ENABLE(zh2py, whether to enable zh2py support,
 [  --enable-zh2py           Enable zh2py support])

if test "$PHP_ZH2PY" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-zh2py -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/zh2py.h"  # you most likely want to change this
  dnl if test -r $PHP_ZH2PY/$SEARCH_FOR; then # path given as parameter
  dnl   ZH2PY_DIR=$PHP_ZH2PY
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for zh2py files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ZH2PY_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ZH2PY_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the zh2py distribution])
  dnl fi

  dnl # --with-zh2py -> add include path
  dnl PHP_ADD_INCLUDE($ZH2PY_DIR/include)

  dnl # --with-zh2py -> check for lib and symbol presence
  dnl LIBNAME=zh2py # you may want to change this
  dnl LIBSYMBOL=zh2py # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ZH2PY_DIR/lib, ZH2PY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ZH2PYLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong zh2py lib version or lib not found])
  dnl ],[
  dnl   -L$ZH2PY_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ZH2PY_SHARED_LIBADD)

  PHP_NEW_EXTENSION(zh2py, zh2py.c, $ext_shared)
fi
