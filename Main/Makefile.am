PIXY=-lpixyusb -lusb-1.0 -lboost_thread -lboost_system -lboost_chrono -lstdc++

bin_PROGRAMS = udp_recv_hc_sr04 udp_recv_bigodes udp_recv_hmc5883l udp_recv_bussola_cell \
	udp_recv_gps_cell udp_send main pixy_test azimuth distance compass_diff \
	udp_sendr_led controle color_follow udp_recv_contato azimuth_new gps_error \
	gpsd_client_new PID spherical_to_plane GPS_compass_data buttons KalmanTeste

udp_recv_hc_sr04_SOURCES = src/lib/udp_receiver.cc src/bin/udp_recv/udp_recv_hc_sr04.cc

udp_recv_bigodes_SOURCES = src/lib/udp_receiver.cc src/bin/udp_recv/udp_recv_bigodes.cc

udp_recv_hmc5883l_SOURCES = src/lib/udp_receiver.cc src/lib/compass.cc src/bin/udp_recv/udp_recv_hmc5883l.cc
udp_recv_hmc5883l_LDADD = -lm

udp_recv_bussola_cell_SOURCES = src/lib/udp_receiver.cc src/bin/udp_recv/udp_recv_bussola_cell.cc

udp_recv_gps_cell_SOURCES = src/lib/udp_receiver.cc src/bin/udp_recv/udp_recv_gps_cell.cc
udp_recv_contato_SOURCES = src/lib/udp_receiver.cc src/bin/udp_recv/udp_recv_contato.cc

udp_send_SOURCES = src/bin/utils/udp_send.cc

pixy_test_SOURCES = src/lib/thread_spawn.cc src/lib/pixy_cam.cc src/bin/utils/pixy_test.cc
pixy_test_LDADD = $(PIXY) -lpthread

azimuth_SOURCES = src/lib/gps_coord.cc src/bin/utils/azimuth.cc
azimuth_LDADD = -lm

azimuth_new_SOURCES = src/lib/GPS.cc src/bin/utils/azimuth_new.cc
azimuth_new_LDADD = -lm -lgps

distance_SOURCES = src/lib/gps_coord.cc src/bin/utils/distance.cc
distance_LDADD = -lm

gps_error_SOURCES = src/lib/gps_coord.cc src/bin/utils/gps_error.cc src/lib/thread_spawn.cc \
					src/lib/thread_gps.cc src/lib/thread_hmc5883l.cc src/lib/compass.cc \
					src/lib/udp_receiver.cc src/lib/pid.cc src/lib/GPS.cc
gps_error_LDADD = -lpthread -lm

main_SOURCES = src/lib/file_lock.cc src/lib/compass.cc src/lib/serial.cc \
	src/lib/thread_spawn.cc src/lib/thread_motors.cc src/lib/thread_gps.cc \
	src/lib/i2c.cc src/lib/gps_coord.cc src/lib/thread_hmc5883l.cc src/lib/pid.cc \
	src/lib/udp_receiver.cc src/lib/thread_sonar.cc src/lib/joystick.cc \
	src/lib/pixy_cam.cc src/bin/main.cc src/lib/leds.cc src/lib/udp_sender.cc \
	src/lib/thread_proximity.cc
main_LDADD = -lpthread -lm $(PIXY)

controle_SOURCES = src/bin/controle.cc src/lib/thread_spawn.cc \
	src/lib/thread_motors.cc src/lib/serial.cc
controle_LDADD = -lpthread

compass_diff_SOURCES = src/lib/compass.cc src/bin/utils/compass_diff.cc
compass_diff_LDADD = -lm

udp_sendr_led_SOURCES = src/bin/udp_sendr/udp_sendr_led.cc src/lib/udp_sender.cc src/lib/leds.cc \
						src/lib/thread_spawn.cc
udp_sendr_led_LDADD = -lpthread

color_follow_SOURCES = src/lib/file_lock.cc src/lib/compass.cc src/lib/serial.cc \
	src/lib/thread_spawn.cc src/lib/thread_motors.cc src/lib/thread_gps.cc \
	src/lib/i2c.cc src/lib/gps_coord.cc src/lib/thread_hmc5883l.cc src/lib/pid.cc \
	src/lib/udp_receiver.cc src/lib/thread_sonar.cc src/lib/joystick.cc \
	src/lib/pixy_cam.cc src/lib/leds.cc src/lib/udp_sender.cc \
	src/lib/thread_proximity.cc src/bin/color_follow.cc
color_follow_LDADD = -lpthread $(PIXY) -lm

gpsd_client_new_SOURCES = src/bin/utils/GPSdClient.cc src/lib/GPSMonitor.cc \
	src/lib/GPS.cc
gpsd_client_new_LDADD = -lgps -lm

PID_SOURCES = src/bin/utils/pid.cc src/lib/PID.cc src/lib/thread_hmc5883l.cc src/lib/compass.cc \
	src/lib/udp_receiver.cc src/lib/pid.cc src/lib/thread_spawn.cc src/lib/thread_motors.cc \
	src/lib/serial.cc
PID_LDADD = -lpthread

spherical_to_plane_SOURCES = src/bin/utils/spherical_to_plane.cc src/lib/GPS.cc

GPS_compass_data_SOURCES = src/bin/utils/GPS_compass_data.cc \
	src/lib/udp_receiver.cc src/lib/GPSMonitor.cc src/lib/GPS.cc src/lib/compass.cc
GPS_compass_data_LDADD = -lgps -lpthread

buttons_SOURCES = src/bin/utils/buttons.cc src/lib/Button.cc

KalmanTeste_SOURCES = src/bin/utils/KalmanTeste.cc src/lib/TrekkingKF.cc src/lib/thread_gps.cc \
	src/lib/gps_coord.cc src/lib/thread_spawn.cc src/lib/udp_receiver.cc
KalmanTeste_LDADD = -lpthread -lgps -lkalman -lm

# -DRAPIDJSON_SSE42 para x86
AM_CXXFLAGS = -I. -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -DRAPIDJSON_HAS_STDSTRING \
	-I$(srcdir)/include -I$(srcdir)/src/lib -Wall -Wextra -std=c++14 \
	-isystem $(srcdir)/../externals/include -L$(srcdir)/../externals/lib

scripts:
	cp -rf $(srcdir)/scripts/* .
.PHONY: scripts

clean-scripts:
	ls $(srcdir)/scripts | xargs rm -f
.PHONY: clean-scripts

all: scripts
clean-local: clean-scripts
