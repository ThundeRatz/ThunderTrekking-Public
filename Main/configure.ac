AC_INIT([Trekking], [0.1], [tiago.shibata@gmail.com])
: ${CXXFLAGS="-Wall -Wextra -std=c++14"}
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CXX(g++)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
])

AX_CHECK_COMPILE_FLAG([-fdiagnostics-color=auto],
    [CXXFLAGS="$CXXFLAGS -fdiagnostics-color=auto"])

AC_OUTPUT
