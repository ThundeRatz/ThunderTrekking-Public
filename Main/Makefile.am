PIXY=-lpixyusb -lusb-1.0 -lboost_thread -lboost_system -lboost_chrono -lstdc++

bin_PROGRAMS = udp_recv_hc_sr04 udp_recv_bigodes udp_recv_hmc5883l udp_recv_bussola_cell \
	udp_recv_gps_cell udp_send main pixy_test azimuth distance compass_diff \
	gpsd_client udp_sendr_led controle

udp_recv_hc_sr04_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_hc_sr04.c

udp_recv_bigodes_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_bigodes.c

udp_recv_hmc5883l_SOURCES = src/lib/udp_receiver.c src/lib/compass.c src/bin/udp_recv/udp_recv_hmc5883l.c
udp_recv_hmc5883l_LDADD = -lm

udp_recv_bussola_cell_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_bussola_cell.c

udp_recv_gps_cell_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_gps_cell.c

udp_send_SOURCES = src/bin/utils/udp_send.c

pixy_test_SOURCES = src/lib/thread_spawn.c src/lib/pixy_cam.c src/bin/utils/pixy_test.c
pixy_test_LDADD = $(PIXY) -lpthread

azimuth_SOURCES = src/lib/gps_coord.c src/bin/utils/azimuth.c
azimuth_LDADD = -lm

distance_SOURCES = src/lib/gps_coord.c src/bin/utils/distance.c
distance_LDADD = -lm

main_SOURCES = src/lib/file_lock.c src/lib/compass.c src/lib/serial.c \
	src/lib/thread_spawn.c src/lib/thread_motors.c src/lib/thread_gps.c \
	src/lib/i2c.c src/lib/gps_coord.c src/lib/thread_hmc5883l.c src/lib/pid.c \
	src/lib/udp_receiver.c src/lib/thread_sonar.c src/lib/joystick.c \
	src/lib/pixy_cam.c src/bin/main.c
main_LDADD = -lpthread -lm $(PIXY)

controle_SOURCES = src/bin/controle.c src/lib/thread_spawn.c \
	src/lib/thread_motors.c src/lib/serial.c
controle_LDADD = -lpthread

compass_diff_SOURCES = src/lib/compass.c src/bin/utils/compass_diff.c
compass_diff_LDADD = -lm

gpsd_client_SOURCES = src/lib/thread_gpsd_client.c src/lib/thread_spawn.c \
	src/bin/utils/gpsd_client.c
gpsd_client_LDADD = -lgps -lpthread

udp_sendr_led_SOURCES = src/bin/udp_sendr/udp_sendr_led.c src/lib/udp_sender.c

OPTIMIZE = -O3 -flto
AM_CFLAGS = -Wall -Wextra -I. -I$(srcdir)/include -I$(srcdir)/src/lib \
	-D_GNU_SOURCE -D_XOPEN_SOURCE=600 $(OPTIMIZE)

.PHONY: scripts

scripts:
	cp -r $(srcdir)/scripts/* .
