bin_PROGRAMS = sensor_server GPIO_test

sensor_server_SOURCES = lib/thread_leds.c lib/gpio_dma.c lib/hmc5883l.c lib/thread_bigodes.c lib/thread_hmc5883l.c lib/thread_spawn.c lib/udp_receiver.c lib/udp_sender.c lib/thread_hc_sr04.c lib/i2c.c bin/sensor_server.c

GPIO_test_SOURCES = bin/utils/GPIO_test.cc lib/GPIO.cc

OPTIMIZE = -O3 -flto
LDADD = -lpthread -lm
AM_CFLAGS = -Wall -Wextra -I$(srcdir)/include -I$(srcdir)/lib -fdiagnostics-color=auto -std=c11 -D_GNU_SOURCE -g
AM_CXXFLAGS = -Wall -Wextra -I$(srcdir)/include -I$(srcdir)/lib -fdiagnostics-color=auto -std=c++11 -D_GNU_SOURCE -g
