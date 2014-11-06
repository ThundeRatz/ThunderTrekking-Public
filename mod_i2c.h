#include <pthreads.h>
#define MOTORS_I2C_ADDR			0x69
//extern pthread_mutex_t mod_i2c;
void *mod_i2c_create(__attribute__((unused)) void *args);
