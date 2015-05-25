AC_INIT([Trekking], [0.1], [tiago.shibata@gmail.com])
: ${CFLAGS=""} # Usaremos mais otimizações (configuradas no Makefile.am)
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC(gcc)
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    Makefile
])

AX_CHECK_COMPILE_FLAG([-fdiagnostics-color=auto],
    [CFLAGS="$CFLAGS -fdiagnostics-color=auto"])

AX_CHECK_COMPILE_FLAG([-dumpversion],
    [ax_gcc_version_option=yes],
    [ax_gcc_version_option=no])

AS_IF([test "x$GCC" = "xyes"],[
    AS_IF([test "x$ax_gcc_version_option" != "xno"],[
        AC_CACHE_CHECK([gcc version],[ax_cv_gcc_version],[
            ax_cv_gcc_version="`$CC -dumpversion`"
            AS_IF([test "x$ax_cv_gcc_version" = "x"],[
                ax_cv_gcc_version=""
            ])
        ])
        GCC_VERSION=$ax_cv_gcc_version
    ])
])
AC_SUBST([GCC_VERSION])

AC_OUTPUT
