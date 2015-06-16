PIXY=-lpixyusb -lusb-1.0 -lboost_thread -lboost_system -lboost_chrono -lstdc++

bin_PROGRAMS = udp_recv_hc_sr04 udp_recv_bigodes udp_recv_hmc5883l udp_recv_bussola_cell \
	udp_recv_gps_cell udp_send main pixy_test azimuth distance compass_diff \
	gpsd_client udp_sendr_led controle color_follow udp_recv_contato azimuth_new

udp_recv_hc_sr04_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_hc_sr04.c

udp_recv_bigodes_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_bigodes.c

udp_recv_hmc5883l_SOURCES = src/lib/udp_receiver.c src/lib/compass.c src/bin/udp_recv/udp_recv_hmc5883l.c
udp_recv_hmc5883l_LDADD = -lm

udp_recv_bussola_cell_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_bussola_cell.c

udp_recv_gps_cell_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_gps_cell.c

udp_recv_contato_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_contato.c

udp_send_SOURCES = src/bin/utils/udp_send.c

pixy_test_SOURCES = src/lib/thread_spawn.c src/lib/pixy_cam.c src/bin/utils/pixy_test.c
pixy_test_LDADD = $(PIXY) -lpthread

azimuth_SOURCES = src/lib/gps_coord.c src/bin/utils/azimuth.c
azimuth_LDADD = -lm

azimuth_new_SOURCES = src/lib/GPS.cc src/bin/utils/azimuth_new.cc
azimuth_new_LDADD = -lm -lgps

distance_SOURCES = src/lib/gps_coord.c src/bin/utils/distance.c
distance_LDADD = -lm

main_SOURCES = src/lib/file_lock.c src/lib/compass.c src/lib/serial.c \
	src/lib/thread_spawn.c src/lib/thread_motors.c src/lib/thread_gps.c \
	src/lib/i2c.c src/lib/gps_coord.c src/lib/thread_hmc5883l.c src/lib/pid.c \
	src/lib/udp_receiver.c src/lib/thread_sonar.c src/lib/joystick.c \
	src/lib/pixy_cam.c src/bin/main.c src/lib/leds.c src/lib/udp_sender.c \
	src/lib/thread_proximity.c
main_LDADD = -lpthread -lm $(PIXY)

controle_SOURCES = src/bin/controle.c src/lib/thread_spawn.c \
	src/lib/thread_motors.c src/lib/serial.c
controle_LDADD = -lpthread

compass_diff_SOURCES = src/lib/compass.c src/bin/utils/compass_diff.c
compass_diff_LDADD = -lm

gpsd_client_SOURCES = src/lib/thread_gpsd_client.c src/lib/thread_spawn.c \
	src/bin/utils/gpsd_client.c
gpsd_client_LDADD = -lgps -lpthread

udp_sendr_led_SOURCES = src/bin/udp_sendr/udp_sendr_led.c src/lib/udp_sender.c src/lib/leds.c \
						src/lib/thread_spawn.c
udp_sendr_led_LDADD = -lpthread

color_follow_SOURCES = src/lib/file_lock.c src/lib/compass.c src/lib/serial.c \
	src/lib/thread_spawn.c src/lib/thread_motors.c src/lib/thread_gps.c \
	src/lib/i2c.c src/lib/gps_coord.c src/lib/thread_hmc5883l.c src/lib/pid.c \
	src/lib/udp_receiver.c src/lib/thread_sonar.c src/lib/joystick.c \
	src/lib/pixy_cam.c src/lib/leds.c src/lib/udp_sender.c \
	src/lib/thread_proximity.c src/bin/color_follow.c
color_follow_LDADD = -lpthread $(PIXY) -lm

OPTIMIZE = -O3 -flto
AM_CFLAGS = -Wall -Wextra -I. -I$(srcdir)/include -I$(srcdir)/src/lib \
	-D_GNU_SOURCE -D_XOPEN_SOURCE=600 $(OPTIMIZE)
AM_CXXFLAGS = $(AM_CFLAGS)

.PHONY: scripts

scripts:
	cp -r $(srcdir)/scripts/* .
