bin_PROGRAMS = sensor_server
sensor_server_SOURCES = lib/gpio_dma.c lib/hmc5883l.c lib/thread_bigodes.c lib/thread_hmc5883l.c lib/thread_spawn.c lib/udp_receiver.c lib/udp_sender.c lib/thread_hc_sr04.c lib/i2c.c bin/sensor_server.c

OPTIMIZE = -O3 -flto
LDADD = -lpthread -lm
AM_CFLAGS = -Wall -Wextra -I$(srcdir)/include -I$(srcdir)/lib -fdiagnostics-color=auto -D_GNU_SOURCE $(OPTIMIZE)
