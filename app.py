#!/usr/bin/python
import time
from time import *
import math
import threading
import tornado.httpserver
import tornado.websocket
import tornado.ioloop
from tornado.ioloop import PeriodicCallback
import tornado.web 
import json
from gps import *

from hmc5883l import getHeading
from leds import *
from motors import *

#bufferSize = 10

gpsd = None #seting the global variable

voltageLogic=0
voltageMotor=0
heading=0
speedE = 0
speedD = 0

leds = Leds()
leds.g = 255

class polli2c(threading.Thread):
	def run(self):
		global bufferSize
		global heading
		global voltageLogic
		global voltageMotor
		global speedE
		global speedD
		
		#bufferCount=0
		#logicBuffer[bufferSize]

		motors = Motores()

		#leds.g = 255
		#leds.update(mode=Leds.PULSE, timestep=0x1)

		while(1):
			motors.liga_led(0x04)
			print 'Sending commands'
			if speedD != None and speedE != None:
				motors.setSpeed(speedE, speedD)
				speedD = speedE = None
				motor_counter = 15
			else:
				motor_counter -= 1
				if motor_counter <= 0:	# sem resposta por muito tempo
					motors.setSpeed(0, 0)
			
			print 'Polling Sensors'
			heading = getHeading()
			voltageLogic = leds.readVoltage(0x03, 77.715)
			voltageMotor = motors.readVoltage(0x06, 33.435)
			'''logicBuffer[bufferCount] = leds.readVoltage(0x03, 77.715)
			motorBuffer[bufferCount] = motors.readVoltage(0x06, 33.435)
			bufferCount = bufferCount+1
			if(bufferCount >= bufferSize):
				bufferCount = 0

			auxlogic=0
			auxmotor=0
			for i in range (0, bufferSize):
				auxlogic = auxlogic + logicBuffer[i]
				auxmotor = auxmotor + motorBuffer[i]

			voltageLogic = auxlogic/bufferSize
			voltageMotor = auxmotor/bufferSize 
'''
			motors.desliga_led(0x04)
			time.sleep(0.1)

class GpsPoller(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
		global gpsd #bring it in scope
		gpsd = gps(mode=WATCH_ENABLE) #starting the stream of info
		self.current_value = None
 
	def run(self):
		global gpsd
		while True:
			print 'Polling GPS'
			gpsd.next() #this will continue to loop and grab EACH set of gpsd info to clear the buffer
			'''print 'latitude	' , gpsd.fix.latitude
			print 'longitude   ' , gpsd.fix.longitude
			print 'time utc	' , gpsd.utc,' + ', gpsd.fix.time
			print 'altitude (m)' , gpsd.fix.altitude
			print 'eps		 ' , gpsd.fix.eps
			print 'epx		 ' , gpsd.fix.epx
			print 'epv		 ' , gpsd.fix.epv
			print 'ept		 ' , gpsd.fix.ept
			print 'speed (m/s) ' , gpsd.fix.speed
			print 'climb	   ' , gpsd.fix.climb
			print 'track	   ' , gpsd.fix.track
			print 'mode		' , gpsd.fix.mode
			print
			print 'sats		' , gpsd.satellites'''
 
class IndexHandler(tornado.web.RequestHandler):
	def get(request):
		request.render('index.html')

class WSHandler(tornado.websocket.WebSocketHandler):
	def open(self):
		print 'Client connected...'
		leds.update(mode=Leds.PULSE, timestep=0x1)
		self.maps_coord = None
		self.keys = [False] * 4
		self.callback = PeriodicCallback(self.sendStatus, 100)
		self.callback.start()

	def sendStatus(self):
		global heading
		global voltageLogic
		global voltageMotor
		global gpsd

		obj = {"compass": heading, "voltagelogic": voltageLogic, "voltagemotor": voltageMotor, "gpslatitude": gpsd.fix.latitude, "gpslongitude": gpsd.fix.longitude}
		if self.maps_coord != None:
		  print 'Enviando', self.maps_coord
		  obj["update_map"] = self.maps_coord
		  self.maps_coord = None
		self.write_message(json.dumps(obj))

	def atualiza_motores(self):
		global speedE
		global speedD
		if not True in self.keys:
		  speedE = speedD = 0
		elif self.keys[1]:
		  if self.keys[2]:
			speedE = 127
			speedD = 250
		  elif self.keys[0]:
			speedE = 250
			speedD = 127
		  else:
			speedE = 250
			speedD = 240
		elif self.keys[3]:
		  if self.keys[2]:
			speedE = -250
			speedD = -127
		  elif self.keys[0]:
			speedE = -127
			speedD = -250
		  else:
			speedE = -250
			speedD = -240
		elif self.keys[2]:
		  speedE = -127
		  speedD = 127
		elif self.keys[0]:
		  speedE = 127
		  speedD = -127


	def on_message(self, message):

		dados = json.loads(message)
		print 'Recebido', dados
		if 'lat' in dados:
		  self.maps_coord = [dados['lat'], dados['long'], dados['ponto']]
		elif 'keydown' in dados:
		  # Ativa motor (0 = esquerda, 1 = p/cima, 2 = direita, 3 = p/ baixo)
		  print 'Motor ligado: ', dados['keydown']
		  self.keys[dados['keydown']] = True
		  self.atualiza_motores()
		elif 'keyup' in dados:
		  # Desliga motor (0 = direta, 1 = p/cima, 2 = esquerda, 3 = p/ baixo)
		  print 'Motor desligado: ', dados['keyup']
		  self.keys[dados['keyup']] = False
		  self.atualiza_motores()
		
	def on_close(self):
		print 'Client disconnected...'
		leds.update(mode=Leds.MANUAL)
		self.callback.stop()

# Creating Threads
threadUpdate = polli2c()
gpsp = GpsPoller() # create the thread

# Starting Threads
threadUpdate.start()
gpsp.start()
 
application = tornado.web.Application([
	(r'/', IndexHandler),
	(r'/ws', WSHandler),
	(r'/map.html', tornado.web.StaticFileHandler, {'path':'map.html'}),
	(r'/video.html', tornado.web.StaticFileHandler, {'path':'video.html'}),
	(r'/static/(.*)', tornado.web.StaticFileHandler, {'path':'./static'}),
])
 
 
if __name__ == "__main__":
	leds.update(mode=Leds.MANUAL)
	http_server = tornado.httpserver.HTTPServer(application)
	http_server.listen(8888)
	tornado.ioloop.IOLoop.instance().start()
