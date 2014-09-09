from gps import *
import time
import threading
import math
from gira_angulo import gira
from angulo_desejado import angulo_desejado
from motors import *
from hmc5883l import getHeading

def distancia(dx, dy):
	return math.sqrt(dx * dx + dy * dy)

def motor_byte(n):
	if n < 250:
		return int(n)
	return 250

class Coordinates():
	AMOSTRAS_MEDIA = 2
	def __init__(self):
		self.samples = []
		self.pos = self.n_amostras = 0
		for i in range(32):
			self.samples.append([.0, .0])
	def update(self, lat, long):
		self.n_amostras += 1
		self.samples[self.pos] = [lat, long]
		self.pos += 1
		if self.pos >= self.AMOSTRAS_MEDIA:
			self.pos = 0

	@property
	def latitude(self):
		while self.n_amostras < self.AMOSTRAS_MEDIA:
			print "Esperando mais %d amostras" % (self.AMOSTRAS_MEDIA - self.n_amostras)
			time.sleep(.5)
		media = .0
		for amostra in self.samples:
			media += amostra[0]
		return media / self.AMOSTRAS_MEDIA
		
	@property
	def longitude(self):
                while self.n_amostras < self.AMOSTRAS_MEDIA:
                        print "Esperando mais %d amostras" % (self.AMOSTRAS_MEDIA - self.n_amostras)
			time.sleep(1)
		media = .0
                for amostra in self.samples:
                        media += amostra[1]
                return media / self.AMOSTRAS_MEDIA


class GpsController(threading.Thread):
	def __init__(self):
		threading.Thread.__init__(self)
		self.gpsd = gps(mode=WATCH_ENABLE) #starting the stream of info
		self.running = False
		self.coord = Coordinates()
   
	def run(self):
		self.running = True
		while self.running:
			# grab EACH set of gpsd info to clear the buffer
			self.gpsd.next()
			if self.gpsd.fix.latitude != .0 and self.gpsd.fix.longitude != .0:
				print("Novas coordenadas: ", self.gpsd.fix.latitude, self.gpsd.fix.longitude)
				self.coord.update(self.gpsd.fix.latitude, self.gpsd.fix.longitude)

	def stopController(self):
		self.running = False
 
	@property
	def fix(self):
		'''while self.gpsd.fix.latitude == .0 or self.gpsd.fix.longitde == .0:
			print "Esperando GPS..."
			time.sleep(.5)'''
		return self.coord

	@property
	def utc(self):
		return self.gpsd.utc

	@property
	def satellites(self):
		return self.gpsd.satellites

if __name__ == '__main__':
	# create the controller
	gpsc = GpsController() 
	try:
		# start controller
		gpsc.start()
		motors = Motores()
		distancia_desejada = 2 #distancia desejada em metros
		alvo = [-23.64697, -46.5725273332]
		# time.sleep(2)
		print("Posicao atual: ", gpsc.fix.latitude, gpsc.fix.longitude)
		bla = angulo_desejado(alvo, [gpsc.fix.latitude, gpsc.fix.longitude])
		print(bla)
		gira(bla)
	 	print((gpsc.fix.latitude-alvo[0]) * 111111, (gpsc.fix.longitude-alvo[1]) * 111111)
		cont = 0
		ultimo_amostras = 0
		while not (((gpsc.fix.latitude-alvo[0])*(111111))**2)+(((gpsc.fix.longitude-alvo[1])*(111111))**2)<(distancia_desejada)**2:
			"""print "latitude ", gpsc.fix.latitude
			print "longitude ", gpsc.fix.longitude
			print "time utc ", gpsc.utc, " + ", gpsc.fix.time
			print "altitude (m)", gpsc.fix.altitude
			print "eps ", gpsc.fix.eps
			print "epx ", gpsc.fix.epx
			print "epv ", gpsc.fix.epv
			print "ept ", gpsc.gpsd.fix.ept
			print "speed (m/s) ", gpsc.fix.speed
			print "climb ", gpsc.fix.climb
			print "track ", gpsc.fix.track
			print "mode ", gpsc.fix.mode
			print "sats ", gpsc.satellites
			time.sleep(0.5)"""
			# 1/111111 e o fator de conversao aproximado de graus de lat/long para metros
			vel = max(200, motor_byte(distancia((gpsc.fix.latitude-alvo[0])*(111111), (gpsc.fix.longitude-alvo[1])*(111111)) * 10 + 100))
			motors.setSpeed(vel, vel)
			cont += 1
			time.sleep(0.01)
			if cont > 500:
				motors.setSpeed(0, 0)
				# se nao houve nenhuma nova amosta e estamos proximos do alvo, espera
				while ultimo_amostras == gpsc.coord.n_amostras: #and distancia((gpsc.fix.latitude-alvo[0])*(111111),(gpsc.fix.longitude-alvo[1])*(111111)) < 5:
					print("Esperando mais amostras")
					time.sleep(.2)
				time.sleep(.5)
				bla = angulo_desejado(alvo, [gpsc.fix.latitude, gpsc.fix.longitude])
				print(bla)
				gira(bla)
				print((gpsc.fix.latitude-alvo[0]) * 111111, (gpsc.fix.longitude-alvo[1]) * 111111)
				cont = 0
		motors.setSpeed(0, 0)
		print ('Terminou o programa')
		"""while True:
		"""

	#Ctrl C
	except KeyboardInterrupt:
		print "User cancelled"
		
	  #Error
	except:
		print "Unexpected error:", sys.exc_info()
		raise

	finally:
		print "Stopping gps controller"
		gpsc.stopController()
		#wait for the tread to finish
		gpsc.join()
	 
	print "Done"

