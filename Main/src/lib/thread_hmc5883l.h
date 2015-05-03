#ifndef __THREAD_HMC5883L_H__
#define __THREAD_HMC5883L_H__

extern double direcao_atual;

void __attribute__((noreturn)) *hmc5883l_thread(__attribute__((unused)) void *ignored);

#endif
