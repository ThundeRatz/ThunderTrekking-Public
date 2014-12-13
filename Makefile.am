bin_PROGRAMS = i2c_features leds_control hmc5883l nmea compass_dump trekking # calibra_bussola
trekking_SOURCES = main.c motors.c file_lock.c joystick.c mod_i2c.c i2c.c compass.c init.c hook.c cont_array.c
i2c_features_SOURCES = i2c.c i2c_features.c
compass_dump_SOURCES = compass_dump.c compass.c mod_i2c.c i2c.c
leds_control_SOURCES = leds_control.c mod_i2c.c i2c.c
# calibra_bussola_SOURCES = calibra_bussola.c mod_i2c.c i2c.c joystick.c compass.c file_lock.c motors.c
nmea_SOURCES = serial.c serial_nmea.c
hmc5883l_SOURCES = hmc5883l_test.c hmc5883l.c mod_i2c.c i2c.c file_lock.c compass.c

OPTIMIZE = -O0 -g -flto #-DDEBUG
LDADD = -lpthread -lm 
AM_CFLAGS = -Wall -Wextra $(OPTIMIZE)
