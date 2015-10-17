PIXY=-lpixyusb -lusb-1.0 -lboost_thread -lboost_system -lboost_chrono -lstdc++

bin_PROGRAMS = main udp_recv_bigodes udp_recv_hmc5883l \
	udp_send pixy_test distance compass_diff controle \
	udp_recv_contato azimuth gps_error gpsd_client_new PID \
	GPS_compass_data log_read_GPS bumpers LedsTest_I2C BNO055_test \
	BumperTest TesteGeral

main_SOURCES = src/bin/main.cc src/lib/ThreadSpawn.cc src/lib/ThreadMotors.cc \
	src/lib/ThreadPixy.cc src/lib/ThreadHmc5883l.cc src/lib/compass.cc \
	src/lib/PID.cc src/lib/UDP.cc src/lib/I2C.cc src/lib/errno_string.cc \
	src/lib/serial.cc src/lib/GPS.cc src/lib/ThreadProximity.cc \
	src/lib/Bumper.cc src/lib/LedsI2C.cc src/lib/BNO055.cc src/lib/bno055.cc \
	src/lib/sleep.cc
main_LDADD = -lm -lpthread -lgps $(PIXY) -levdev

udp_recv_bigodes_SOURCES = src/lib/UDP.cc src/bin/udp_recv/udp_recv_bigodes.cc \
	src/lib/errno_string.cc

udp_recv_hmc5883l_SOURCES = src/lib/UDP.cc src/lib/compass.cc src/bin/udp_recv/udp_recv_hmc5883l.cc \
	src/lib/errno_string.cc
udp_recv_hmc5883l_LDADD = -lm

udp_recv_contato_SOURCES = src/lib/UDP.cc src/bin/udp_recv/udp_recv_contato.cc \
	src/lib/errno_string.cc

udp_send_SOURCES = src/bin/utils/udp_send.cc

pixy_test_SOURCES = src/lib/ThreadSpawn.cc src/lib/ThreadPixy.cc src/bin/utils/pixy_test.cc \
	src/lib/sleep.cc src/lib/errno_string.cc
pixy_test_LDADD = $(PIXY) -lpthread

azimuth_SOURCES = src/lib/GPS.cc src/bin/utils/azimuth.cc
azimuth_LDADD = -lm -lgps

distance_SOURCES = src/lib/GPS.cc src/bin/utils/distance.cc
distance_LDADD = -lm -lgps

gps_error_SOURCES = src/bin/utils/gps_error.cc src/lib/ThreadSpawn.cc \
	src/lib/ThreadHmc5883l.cc src/lib/compass.cc \
	src/lib/UDP.cc src/lib/GPS.cc src/lib/Stats.cc
gps_error_LDADD = -lpthread -lm -lgps

controle_SOURCES = src/bin/utils/controle.cc src/lib/ThreadSpawn.cc \
	src/lib/ThreadMotors.cc src/lib/serial.cc src/lib/errno_string.cc \
	src/lib/sleep.cc
controle_LDADD = -lpthread

compass_diff_SOURCES = src/lib/compass.cc src/bin/utils/compass_diff.cc
compass_diff_LDADD = -lm

# color_follow_SOURCES = src/lib/compass.cc src/lib/serial.cc \
#	src/lib/ThreadSpawn.cc src/lib/ThreadMotors.cc src/lib/ThreadHmc5883l.cc \
#	src/lib/UDP.cc src/lib/ThreadPixy.cc src/lib/leds.cc \
#	src/lib/ThreadProximity.cc src/bin/utils/color_follow.cc src/lib/errno_string.cc
# color_follow_LDADD = -lpthread $(PIXY) -lm

gpsd_client_new_SOURCES = src/bin/utils/GPSdClient.cc src/lib/GPS.cc src/lib/sleep.cc \
	src/lib/errno_string.cc
gpsd_client_new_LDADD = -lgps -lm

PID_SOURCES = src/bin/utils/pid.cc src/lib/PID.cc src/lib/ThreadHmc5883l.cc src/lib/compass.cc \
	src/lib/UDP.cc src/lib/ThreadSpawn.cc src/lib/ThreadMotors.cc \
	src/lib/serial.cc src/lib/errno_string.cc src/lib/sleep.cc
PID_LDADD = -lpthread

GPS_compass_data_SOURCES = src/bin/utils/GPS_compass_data.cc \
	src/lib/UDP.cc src/lib/GPS.cc src/lib/compass.cc src/lib/errno_string.cc \
	src/lib/sleep.cc
GPS_compass_data_LDADD = -lgps -lpthread

bumpers_SOURCES = src/bin/utils/bumpers.cc src/lib/GPIO.cc src/lib/GPIOButton.cc
bumpers_LDADD = -lpthread

LedsTest_I2C_SOURCES = src/lib/LedsI2C.cc src/lib/I2C.cc src/bin/utils/LedsTest.cc
LedsTest_I2C_LDADD = -lm -lpthread -levdev

BNO055_test_SOURCES = src/bin/utils/BNO055_test.cc src/lib/bno055.cc \
	src/lib/BNO055.cc src/lib/I2C.cc src/lib/sleep.cc src/lib/errno_string.cc

BumperTest_SOURCES = src/bin/utils/BumperTest.cc src/lib/Bumper.cc src/lib/Leds.cc \
	src/lib/errno_string.cc
BumperTest_LDADD = -levdev

# KalmanTeste_SOURCES = src/bin/utils/KalmanTeste.cc src/lib/TrekkingKF.cc \
# 	src/lib/ThreadSpawn.cc src/lib/UDP.cc \s
# 	src/lib/GPS.cc
# KalmanTeste_LDADD = -lpthread -lgps -lkalman -lm

log_read_GPS_SOURCES = src/bin/utils/log_read_GPS.cc src/lib/GPS.cc

TesteGeral_SOURCES = src/bin/utils/TestaTudo.cc src/lib/ThreadSpawn.cc src/lib/ThreadPixy.cc \
	src/lib/Stats.hh src/lib/Bumper.cc src/lib/BNO055.cc src/lib/bno055.cc src/lib/I2C.cc \
	src/lib/GPS.cc src/lib/errno_string.cc src/lib/sleep.cc src/lib/Leds.cc
TesteGeral_LDADD = -lncurses -lpthread -lgps -levdev $(PIXY)

# -DRAPIDJSON_SSE42 for x86
AM_CXXFLAGS = -I. -D_GNU_SOURCE -D_XOPEN_SOURCE=600 -DRAPIDJSON_HAS_STDSTRING \
	-I$(srcdir)/include -I$(srcdir)/src/lib -Wall -Wextra -std=gnu++11 \
	-isystem $(srcdir)/../externals/include -L$(srcdir)/../externals/lib \
	-I/usr/include/libevdev-1.0/libevdev/ -I/usr/include/libevdev-1.0/
scripts:
	cp -rf $(srcdir)/scripts/* .
.PHONY: scripts

clean-scripts:
	ls $(srcdir)/scripts | xargs rm -f
.PHONY: clean-scripts

all: scripts
clean-local: clean-scripts
