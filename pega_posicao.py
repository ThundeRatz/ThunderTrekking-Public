#!/usr/bin/python
AMOSTRAS = 10
from gps import *
import threading
import time

class GpsController(threading.Thread):
        def __init__(self):
                threading.Thread.__init__(self)
                self.gpsd = gps(mode=WATCH_ENABLE) #starting the stream of info
                self.running = False

        def run(self):
                self.running = True
                while self.running:
                        # grab EACH set of gpsd info to clear the buffer
                        self.gpsd.next()

        def stopController(self):
                self.running = False

        @property
        def fix(self):
                while self.gpsd.fix.latitude == .0 or self.gpsd.fix.longitude == 1:
                        print "Esperando GPS..."
                        time.sleep(.5)
                return self.gpsd.fix

        @property
        def utc(self):
                return self.gpsd.utc

	@property
        def satellites(self):
                return self.gpsd.satellites

gpsc = GpsController()
gpsc.start()
lat = long = .0
ultima_lat = ultima_long = .0
for i in range(1, AMOSTRAS + 1):
	while gpsc.fix.latitude == ultima_lat and gpsc.fix.longitude == ultima_long:
		print "Esperando atualizacao..."
		time.sleep(.2)
	ultima_lat, ultima_long = gpsc.fix.latitude, gpsc.fix.longitude
	lat += ultima_lat
	long += ultima_long
	print "Amostra %d:" % i,  ultima_lat, ultima_long

print "Media:", lat / AMOSTRAS, long / AMOSTRAS
gpsc.stopController()
