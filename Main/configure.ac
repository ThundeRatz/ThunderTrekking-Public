AC_INIT([Trekking], [0.1], [tiago.shibata@gmail.com])
: ${CFLAGS=""} # Usaremos mais otimizações (configuradas no Makefile.am)
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC(gcc)
AC_PROG_CXX(g++)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
])

AX_CHECK_COMPILE_FLAG([-fdiagnostics-color=auto],
    [CFLAGS="$CFLAGS -fdiagnostics-color=auto"])

AC_OUTPUT
