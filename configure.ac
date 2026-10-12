m4_define([VIVIANITE_VERSION], [m4_esyscmd_s([cat VERSION])])

AC_INIT([vivianite],[VIVIANITE_VERSION])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_OBJC
AC_PROG_INSTALL

PKG_CHECK_MODULES(
    [DEPS],
    [freetype2 sdl3 sdl3-ttf]
)

AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
                    [Enable debug mode and warnings.])],
    [enable_debug=$enableval],
    [enable_debug=yes]
)

AM_CONDITIONAL([DEBUG], [test "x$enable_debug" = "xyes"])

AC_CONFIG_FILES([
    Makefile
])

AC_OUTPUT
