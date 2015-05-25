bin_PROGRAMS = udp_recv_hc_sr04 udp_recv_bigodes udp_recv_hmc5883l udp_recv_bussola_cell \
	udp_recv_gps_cell udp_send controle main pixy_test azimuth distance compass_diff \
	gpsd_client
udp_recv_hc_sr04_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_hc_sr04.c
udp_recv_bigodes_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_bigodes.c
udp_recv_hmc5883l_SOURCES = src/lib/udp_receiver.c src/lib/compass.c src/bin/udp_recv/udp_recv_hmc5883l.c
udp_recv_bussola_cell_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_bussola_cell.c
udp_recv_gps_cell_SOURCES = src/lib/udp_receiver.c src/bin/udp_recv/udp_recv_gps_cell.c
udp_send_SOURCES = src/bin/utils/udp_send.c
pixy_test_SOURCES = src/lib/thread_spawn.c src/lib/pixy_cam.c src/bin/utils/pixy_test.c
azimuth_SOURCES = src/lib/gps_coord.c src/bin/utils/azimuth.c
distance_SOURCES = src/lib/gps_coord.c src/bin/utils/distance.c
controle_SOURCES = src/lib/file_lock.c src/lib/joystick.c src/lib/compass.c \
	src/lib/serial.c src/lib/thread_spawn.c src/lib/thread_motors.c src/lib/i2c.c \
	src/bin/controle.c
main_SOURCES = src/lib/file_lock.c src/lib/compass.c src/lib/serial.c \
	src/lib/thread_spawn.c src/lib/thread_motors.c src/lib/thread_gps.c \
	src/lib/i2c.c src/lib/gps_coord.c src/lib/thread_hmc5883l.c src/lib/pid.c \
	src/lib/udp_receiver.c src/lib/thread_sonar.c src/lib/joystick.c \
	src/lib/pixy_cam.c src/bin/main.c
compass_diff_SOURCES = src/lib/compass.c src/bin/utils/compass_diff.c
gpsd_client_SOURCES = src/lib/thread_gpsd_client.c src/lib/thread_spawn.c \
	src/bin/utils/gpsd_client.c

OPTIMIZE = -O3 -flto
LDADD = -lpthread -lm -lpixyusb -lboost_thread -lboost_system -lboost_chrono -lstdc++ -lusb-1.0 -lgps
AM_CFLAGS = -Wall -Wextra -I. -I$(srcdir)/include -I$(srcdir)/src/lib \
	-D_GNU_SOURCE -D_XOPEN_SOURCE=600 $(OPTIMIZE)

.PHONY: scripts

scripts:
	cp -r $(srcdir)/scripts/* .
