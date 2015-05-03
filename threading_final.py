#!/usr/bin/python
from gps import *
import time
import threading
import math
from gira_angulo import gira
from angulo_desejado import angulo_desejado
from motors import *
from hmc5883l import getHeading
from leds import *
import ColorTracking2

DISTANCIA_GPS = 2# distancia para parar de usar GPS e usar camera
ALVOS = [[-23.6467584999, -46.5724168333], [-23.6469173333, -46.5725231667], [ -23.6468156666, -46.5727473334]]
INTERMEDIARIO = [-23.6467216667, -46.5726653333]
INTERVALO_RODA_GPS = 100

DEBUG = 1
VEL_MIN = 130
VEL_MAX = 150
MARGEM = 100
CONE_MINIMO = 500
CONE_MAXIMO = 16000
FORA_DE_VISAO = 1000.
MOMENTO_MAX = 3000000

vel_rotacao = 100 # velocidade inicial
QUEDA_ROTACAO = 30 # quanto diminui?
VEL_MIN_ROTACAO = 130

def distancia(dx, dy):
	return math.sqrt(dx * dx + dy * dy)

def motor_byte(n):
	if n < 250:
		return int(n)
	return 250

class Coordinates():
	AMOSTRAS_MEDIA = 1
	def __init__(self):
		self.samples = []
		self.pos = self.n_amostras = 0
		for i in range(32):
			self.samples.append([.0, .0])
	def update(self, lat, lon):
		self.n_amostras += 1
		self.samples[self.pos] = [lat, lon]
		self.pos += 1
		if self.pos >= self.AMOSTRAS_MEDIA:
			self.pos = 0

	@property
	def latitude(self):
		while self.n_amostras < self.AMOSTRAS_MEDIA:
			print "GPS - Esperando mais %d amostras" % (self.AMOSTRAS_MEDIA - self.n_amostras)
			time.sleep(.5)
		media = .0
		for amostra in self.samples:
			media += amostra[0]
		return media / self.AMOSTRAS_MEDIA
		
	@property
	def longitude(self):
		while self.n_amostras < self.AMOSTRAS_MEDIA:
			print "GPS - Esperando mais %d amostras" % (self.AMOSTRAS_MEDIA - self.n_amostras)
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
   		last_lat = last_long = 0.
		while self.running:
			# grab EACH set of gpsd info to clear the buffer
			self.gpsd.next()
			if self.gpsd.fix.latitude != last_lat or self.gpsd.fix.longitude != last_long:
				print("Novas coordenadas: ", self.gpsd.fix.latitude, self.gpsd.fix.longitude)
				last_lat = self.gpsd.fix.latitude
				last_long = self.gpsd.fix.longitude
				self.coord.update(self.gpsd.fix.latitude, self.gpsd.fix.longitude)
				print("Coordenadas filtradas: ", self.coord.latitude, self.coord.longitude)
				time.sleep(.1)

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
	leds = Leds()
	# start controller
	gpsc = GpsController()
	leds.r = leds.g = 0
	leds.b = 255
	leds.update(mode=Leds.PULSE, timestep=0x1)
	gpsc.start()
	motors = Motores()
	motors.setSpeed(250, 250)
	time.sleep(10)
	colour_tracker = ColorTracking2.ColourTracker()
	ultimo_amostras = 0
	for alvo in ALVOS:
		leds.r = leds.g = 0
		leds.b = 255
		leds.update(mode=Leds.PULSE, timestep=0x1)
		if alvo == ALVOS[2]:
			print("Ponto intermediario")
			cont = 0
			while (((gpsc.fix.latitude-INTERMEDIARIO[0])*(111111))**2)+(((gpsc.fix.longitude-INTERMEDIARIO[1])*(111111))**2)>(5)**2:
				vel = min(200, motor_byte(distancia((gpsc.fix.latitude-alvo[0])*(111111), (gpsc.fix.longitude-alvo[1])*(111111)) * 20 + 100) )
				motors.setSpeed(vel, vel)
				cont += 1
				time.sleep(0.01)
				if cont >= INTERVALO_RODA_GPS:
					print("Rodando...")
					motors.setSpeed(0, 0)
					# se nao houve nenhuma nova amosta e estamos proximos do alvo, espera
					while ultimo_amostras == gpsc.coord.n_amostras: #and distancia((gpsc.fix.latitude-alvo[0])*(111111),(gpsc.fix.longitude-alvo[1])*(111111)) < 5:
						ang_lat = gpsc.fix.latitude
						ang_long = gpsc.fix.longitude
						print("Esperando mais amostras")
						time.sleep(.2)
					ang_lat = gpsc.fix.latitude
					ang_long = gpsc.fix.longitude
					ultimo_amostras = gpsc.coord.n_amostras
					time.sleep(.2)
					bla = angulo_desejado(alvo, [ang_lat, ang_long])
					print(bla)
					gira(bla)
					print((gpsc.fix.latitude-alvo[0]) * 111111, (gpsc.fix.longitude-alvo[1]) * 111111)
					cont = 0
		print(alvo)
		print("Proximo alvo: %f x %f" % (alvo[0], alvo[1]))
		try:
			# time.sleep(2)
			print("Posicao atual: ", gpsc.fix.latitude, gpsc.fix.longitude)
			bla = angulo_desejado(alvo, [gpsc.fix.latitude, gpsc.fix.longitude])
			print(bla)
			gira(bla)
			print((gpsc.fix.latitude-alvo[0]) * 111111, (gpsc.fix.longitude-alvo[1]) * 111111)
			cont = INTERVALO_RODA_GPS
			ultimo_amostras = 0
			terminou_cone = False
			while not terminou_cone:
				print("Usando GPS")
				# create the controller
				leds.r = leds.g = 0
				leds.b = 255
				leds.update(mode=Leds.PULSE, timestep=0x1)
				while (((gpsc.fix.latitude-alvo[0])*(111111))**2)+(((gpsc.fix.longitude-alvo[1])*(111111))**2)>(DISTANCIA_GPS)**2:
					'''if colour_tracker.sensor_branco():
						print("Sensor branco chegou em modo GPS")
						break
					'''
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
					vel = min(200, motor_byte(distancia((gpsc.fix.latitude-alvo[0])*(111111), (gpsc.fix.longitude-alvo[1])*(111111)) * 20 + 100) )
					motors.setSpeed(vel, vel)
					cont += 1
					time.sleep(0.01)
					if cont >= INTERVALO_RODA_GPS:
						print("Rodando...")
						motors.setSpeed(0, 0)
						# se nao houve nenhuma nova amosta e estamos proximos do alvo, espera
						while ultimo_amostras == gpsc.coord.n_amostras: #and distancia((gpsc.fix.latitude-alvo[0])*(111111),(gpsc.fix.longitude-alvo[1])*(111111)) < 5:
							ang_lat = gpsc.fix.latitude
							ang_long = gpsc.fix.longitude
							print("Esperando mais amostras")
							time.sleep(.2)
						ang_lat = gpsc.fix.latitude
						ang_long = gpsc.fix.longitude
						ultimo_amostras = gpsc.coord.n_amostras
						time.sleep(.2)
						bla = angulo_desejado(alvo, [ang_lat, ang_long])
						print(bla)
						gira(bla)
						print((gpsc.fix.latitude-alvo[0]) * 111111, (gpsc.fix.longitude-alvo[1]) * 111111)
						cont = 0
				motors.setSpeed(0, 0)
				print ('Terminou GPS')
				
				leds.g = leds.b = 0
				leds.r = 255
				leds.update(mode=Leds.PULSE, timestep=0x1)
				horario = False
				encontrou = False
				ciclos_reto = 0
				print("Usando camera")
				while True:
					time.sleep(0.01)
					camera = colour_tracker.run()
					if camera[1] != FORA_DE_VISAO:
						vel = int(max(VEL_MIN, abs(camera[1])/6 + VEL_MIN))
						print("Vendo cone")
					else:
						print("Fora de visao")
						vel = VEL_MIN
					if vel > VEL_MAX:
						vel = VEL_MAX
					if not (camera[0] > CONE_MAXIMO):
						print("Camera:", camera)
						if (((gpsc.fix.latitude-alvo[0])*(111111))**2)+(((gpsc.fix.longitude-alvo[1])*(111111))**2)>(DISTANCIA_GPS + 1)**2:
							print("GPS longe, saindo de camera")
							break
						if (camera[1] == FORA_DE_VISAO or camera[0] < CONE_MINIMO) and ciclos_reto == 0:
							vel = min(vel_rotacao, vel)
							if encontrou:
								encontrou = False
								horario = not horario
								vel_rotacao = max(vel_rotacao - QUEDA_ROTACAO, VEL_MIN_ROTACAO)
							if horario:
								dir_str = "horario"
							else:
								dir_str = "anti-horario"
							print("Fora de visao, girando %s com velocidade %d" % (dir_str, vel))
							if horario:
								motors.setSpeed(vel, -vel)
							else:
								motors.setSpeed(-vel, vel)
						else:
							if camera[1] != FORA_DE_VISAO:
								ciclos_reto = 2
								encontrou = True
							else:
								ciclos_reto -= 1
							if camera[0] == 0: # fora do campo de visao
								vel = vel_motor
								if horario:  # estava em horario, agora anti-horario
									motors.setSpeed(vel, motor_byte(vel + 30))
									print("Fora do campo de visao, corrigindo ", vel, motor_byte(vel + 30))
								else:
									motors.setSpeed(motor_byte(vel + 30), vel)
									print("Fora do campo de visao, corrigindo ", motor_byte(vel + 30), vel)
							else:
								margem = max(60, (1000 / camera[0]) * 600)
								'''if(camera[1]<-margem):
									vel = min(vel_rotacao, vel)
									motors.setSpeed(vel, vel + 10)
									if(DEBUG):
										print ("Gira anti-horario: ", camera[1], camera[0])
								elif(camera[1]>+margem):
									vel = min(vel_rotacao, vel)
									motors.setSpeed(vel + 10, vel)
									if(DEBUG):
										print ("Gira horario: ", camera[1], camera[0])
								else:'''
								# vel_motor = VEL_MIN + VEL_MAX
								if(camera[0]<1300):
									vel_motor = (VEL_MAX + VEL_MIN) // 2
								elif camera[0]<1500:
									vel_motor = VEL_MAX
								else:
									vel_motor= VEL_MIN
								if camera[1] > 0:
									motors.setSpeed(motor_byte(vel_motor + camera[1] // 2), vel_motor)
									if(DEBUG):
										print ("Anda reto: cameras ", camera[1], camera[0], "vel", motor_byte(vel_motor + camera[1] // 2 ), vel_motor)
								else:
									motors.setSpeed(vel_motor, motor_byte(vel_motor - camera[1] // 2))
									if(DEBUG):
										print ("Anda reto: cameras ", camera[1], camera[0], "vel", vel_motor, motor_byte(vel_motor - camera[1] // 2))
					else:
						time.sleep(.5)
						#print("Sensor branco chegou")
						#if not colour_tracker.sensor_branco:
						#	print("Segunda checagem do sensor falhou, reiniciando cone")
						#	break
						while ultimo_amostras == gpsc.coord.n_amostras:
							time.sleep(.1)
							print("Camera ok, provando com gps...")
						if (((gpsc.fix.latitude-alvo[0])*(111111))**2)+(((gpsc.fix.longitude-alvo[1])*(111111))**2)>(DISTANCIA_GPS + .8)**2:
							print("GPS nao bate com destino, reiniciando...")
							break
						terminou_cone = True
						print("Cone terminado")
						motors.setSpeed(0, 0)
						leds.update(mode=Leds.PULSE3, timestep=1)
						time.sleep(3)
						break
		#Ctrl C
		except KeyboardInterrupt:
			print "User cancelled"
			raise
			
		  #Error
		except:
			print "Unexpected error:", sys.exc_info()
			raise

print "Stopping gps controller"
gpsc.stopController()
#wait for the thread to finish
gpsc.join()

