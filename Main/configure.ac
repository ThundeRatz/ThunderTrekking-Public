AC_INIT([Trekking], [0.1], [tiago.shibata@gmail.com])
: ${CXXFLAGS="-Wall -Wextra -I. -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -std=c++14"}
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CXX(g++)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
])

AX_CHECK_COMPILE_FLAG([-fdiagnostics-color=auto],
    [CXXFLAGS="$CFLAGS -fdiagnostics-color=auto"])

AC_OUTPUT
