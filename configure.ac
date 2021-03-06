#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([vitweet], [0.01], [gabomagno@gmail.com])
AM_INIT_AUTOMAKE
LT_PREREQ([2.4])
LT_INIT([dlopen])
AC_CONFIG_SRCDIR([src/vitweet.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O

AM_SILENT_RULES([yes])

# Checks for libraries.
 
if test -z "$PKG_CONFIG"; then
   AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
fi
if test "$PKG_CONFIG" = "no" ; then
   echo "*** pkg-config not found, can't build with gtk 2.0"
fi


PKG_CHECK_MODULES([GTK], [gtk+-3.0])

PKG_CHECK_MODULES([WEBKIT], [webkitgtk-3.0])

PKG_CHECK_MODULES([GLIB], [glib-2.0])

PKG_CHECK_MODULES([XML], [libxml-2.0])

AC_SEARCH_LIBS([oauth_sign_url2], [oauth])


# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h oauth.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([strdup])


AC_CONFIG_FILES([Makefile
                 include/Makefile
                 libgsocial/Makefile
                 src/Makefile])

AC_OUTPUT

echo \
"-------------------------------------------------

 ${PACKAGE_NAME} Version ${PACKAGE_VERSION}

 Prefix: '${prefix}'.
 Compiler: '${CC} ${CFLAGS} ${CPPFLAGS}'

 Package features:
    Webkit support

 Now type 'make @<:@<target>@:>@'
   where the optional <target> is:
     all                - build all binaries
     install            - install everything

--------------------------------------------------"

