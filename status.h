#ifndef __STATUS_H__
#define __STATUS_H__
#include <string.h>
#include <stdio.h>
// Funções i2c e pthread retornam o código de erro ao invés de setar errno. As macros são para tornar o código mais curto
// Mostra __msg e o a string de erro __errno
#define status_perror(__msg, __errno)	do {if ((__errno)) {char __errmsg[50]; fprintf(stderr, "%s: %s\n", __msg, strerror_r(__errno, __errmsg, sizeof(__errmsg)));}} while (0)
// Executa cmd. Se retornar erro, chama status_perror e retorna da função
#define status_try(cmd, errmsg)			do {int __errno = (cmd); if (__errno) {status_perror(errmsg, __errno); return;}} while (0)
// Executa cmd. Se houver erro, chama abort()
#define status_abort(cmd, errmsg)		do {int __errno = (cmd); if (__errno) {status_perror(errmsg, __errno); abort();}} while (0)
#endif
