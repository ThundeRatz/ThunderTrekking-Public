PIXY=-lpixyusb -lusb-1.0 -lboost_thread -lboost_system -lboost_chrono -lstdc++
LEDDAR=-lLeddarC -lLeddar -lLeddarTech -lstdc++

bin_PROGRAMS = udp_send pixy_test distance compass_diff controle \
	azimuth gps_error gpsd_client_new \
	GPS_compass_data log_read_GPS bumpers LedsTest_I2C BNO055_test \
	BumperTest TesteGeral LeddarTest FindCone GPIOButtonTest \
	GPSTest BNOCalib mainMaps cameraTest pino164 main CompassTest \
	log_read_kalman

## @TODO: Limpar aqui
main_SOURCES = src/bin/main.cc src/lib/ThreadSpawn.cc src/lib/ThreadMotors.cc \
	src/lib/ThreadPixy.cc src/lib/Compass.cc \
	src/lib/PID.cc src/lib/UDP.cc src/lib/I2C.cc src/lib/errno_string.cc \
	src/lib/serial.cc src/lib/GPS.cc src/lib/ThreadProximity.cc \
	src/lib/Bumper.cc src/lib/Leds.cc src/lib/BNO055.cc src/lib/bno055.cc \
	src/lib/sleep.cc src/lib/Pixy.cc src/lib/Stats.cc src/lib/GPIOButton.cc \
	src/lib/camera.cpp src/lib/GPIO.cc src/lib/LedsI2C.cc
main_LDADD = -lm -lpthread -lgps $(PIXY) -levdev

udp_send_SOURCES = src/bin/utils/udp_send.cc

pixy_test_SOURCES = src/lib/ThreadSpawn.cc src/lib/ThreadPixy.cc src/bin/utils/pixy_test.cc \
	src/lib/sleep.cc src/lib/errno_string.cc
pixy_test_LDADD = $(PIXY) -lpthread

azimuth_SOURCES = src/lib/GPS.cc src/bin/utils/azimuth.cc src/lib/Stats.cc
azimuth_LDADD = -lm -lgps

distance_SOURCES = src/lib/GPS.cc src/bin/utils/distance.cc src/lib/Stats.cc
distance_LDADD = -lm -lgps

gps_error_SOURCES = src/bin/utils/gps_error.cc src/lib/ThreadSpawn.cc \
	src/lib/errno_string.cc src/lib/Compass.cc \
	src/lib/GPS.cc src/lib/Stats.cc src/lib/UDP.cc src/lib/sleep.cc
gps_error_LDADD = -lpthread -lm -lgps

controle_SOURCES = src/bin/utils/controle.cc src/lib/ThreadSpawn.cc \
	src/lib/ThreadMotors.cc src/lib/serial.cc src/lib/errno_string.cc \
	src/lib/sleep.cc
controle_LDADD = -lpthread

compass_diff_SOURCES = src/lib/Compass.cc src/bin/utils/compass_diff.cc
compass_diff_LDADD = -lm

FindCone_SOURCES = src/bin/utils/FindCone.cc src/lib/Compass.cc src/lib/serial.cc \
	src/lib/ThreadSpawn.cc src/lib/ThreadMotors.cc src/lib/Bumper.cc src/lib/LeddarEK.cc \
	src/lib/UDP.cc src/lib/Pixy.cc src/lib/sleep.cc src/lib/BNO055.cc \
	src/lib/Leds.cc src/lib/errno_string.cc src/lib/I2C.cc src/lib/bno055.cc
FindCone_LDADD = -lpthread $(PIXY) -lm $(LEDDAR) -levdev -lgps

gpsd_client_new_SOURCES = src/bin/utils/GPSdClient.cc src/lib/GPS.cc src/lib/sleep.cc \
	src/lib/errno_string.cc src/lib/Stats.cc
gpsd_client_new_LDADD = -lgps -lm

GPS_compass_data_SOURCES = src/bin/utils/GPS_compass_data.cc \
	src/lib/BNO055.cc src/lib/bno055.cc src/lib/GPS.cc src/lib/Compass.cc \
	src/lib/errno_string.cc src/lib/sleep.cc src/lib/Stats.cc src/lib/I2C.cc
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

GPIOButtonTest_SOURCES = src/lib/GPIOButton.cc src/bin/utils/GPIOButtonTest.cc \
    src/lib/GPIO.cc
GPIOButtonTest_LDADD = -lpthread
GPIOButtonTest_LDFLAGS = -pthread -Wl,--no-as-needed

cameraTest_SOURCES = src/lib/ThreadMotors.cc src/visao/camera.cpp src/visao/cameraMainTest.cc \
	src/lib/serial.cc src/lib/ThreadSpawn.cc src/lib/sleep.cc src/lib/errno_string.cc
cameraTest_LDADD = $(opencv_LIBS)

log_read_kalman_SOURCES = src/bin/utils/log_read_kalman.cc src/lib/TrekkingKF.cc \
	src/lib/GPS.cc
log_read_kalman_LDADD = -lpthread -lgps -lkalman -lm

<<<<<<< HEAD
udp_sendr_led_SOURCES = src/bin/udp_sendr/udp_sendr_led.c src/lib/udp_sender.c src/lib/leds.c \
						src/lib/thread_spawn.c
udp_sendr_led_LDADD = -lpthread
=======
log_read_GPS_SOURCES = src/bin/utils/log_read_GPS.cc src/lib/GPS.cc src/lib/Stats.cc
log_read_GPS_LDADD = -lgps
>>>>>>> tiagoshibata

TesteGeral_SOURCES = src/bin/utils/TestaTudo.cc src/lib/Pixy.cc \
	src/lib/Stats.hh src/lib/Bumper.cc src/lib/BNO055.cc src/lib/bno055.cc src/lib/I2C.cc \
	src/lib/GPS.cc src/lib/errno_string.cc src/lib/sleep.cc src/lib/Leds.cc src/lib/LeddarEK.cc \
	src/lib/ThreadSpawn.cc src/lib/ThreadMotors.cc src/lib/serial.cc src/lib/Stats.cc
TesteGeral_LDADD = -lncurses -lpthread -lgps -levdev $(PIXY) $(LEDDAR)

LeddarTest_SOURCES = src/bin/utils/LeddarTest.cc src/lib/sleep.cc src/lib/errno_string.cc \
	src/lib/LeddarEK.cc
LeddarTest_LDADD = $(LEDDAR) -lpthread

GPSTest_SOURCES = src/bin/utils/GPSTest.cc src/lib/GPS.cc src/lib/sleep.cc src/lib/errno_string.cc \
	src/lib/Stats.cc src/lib/bno055.cc src/lib/Compass.cc src/lib/I2C.cc src/lib/BNO055.cc
GPSTest_LDADD = -lgps

pino164_SOURCES = src/bin/utils/pino164.cc src/lib/GPIOButton.cc src/lib/GPIO.cc \
	src/lib/sleep.cc src/lib/errno_string.cc
pino164_LDADD = -lpthread
pino164_LDFLAGS = -pthread -Wl,--no-as-needed

BNOCalib_SOURCES = src/bin/utils/BNO055_calibration_test.cc src/lib/BNO055.cc src/lib/sleep.cc \
	src/lib/bno055.cc src/lib/I2C.cc src/lib/errno_string.cc

mainMaps_SOURCES = src/bin/mainMaps.cc src/lib/LeddarEK.cc src/lib/sleep.cc src/lib/errno_string.cc \
	src/lib/Bumper.cc src/lib/Leds.cc src/lib/ThreadMotors.cc src/lib/ThreadSpawn.cc src/lib/serial.cc \
	src/lib/BNO055.cc src/lib/bno055.cc src/lib/GPS.cc src/lib/Stats.cc src/lib/I2C.cc src/lib/Compass.cc
mainMaps_LDADD = $(LEDDAR) -levdev -lpthread -lgps

CompassTest_SOURCES = src/tests/CompassTest.cc src/lib/Compass.cc
CompassTest_LDADD = -lgtest -lgtest_main -lpthread

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
