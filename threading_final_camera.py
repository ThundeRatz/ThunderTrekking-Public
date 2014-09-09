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

DISTANCIA_GPS = 1.5	# distancia para parar de usar GPS e usar camera
ALVOS = [[-23.6467753334, -46.5724383334], [-23.6469426666, -46.5725283334], [-23.6467066666, -46.5727843332]]
INTERVALO_RODA_GPS = 300

DEBUG = 1
VEL_MIN = 130
VEL_MAX = 150
MARGEM = 100
CONE_MINIMO = 500
CONE_MAXIMO = 1500
FORA_DE_VISAO = 1000.
MOMENTO_MAX = 3000000

vel_rotacao = 100 # velocidade inicial
QUEDA_ROTACAO = 30 # quanto diminui?
VEL_MIN_ROTACAO = 100

def distancia(dx, dy):
	return math.sqrt(dx * dx + dy * dy)

def motor_byte(n):
	if n < 250:
		return int(n)
	return 250

if __name__ == '__main__':
	leds = Leds()
	leds.g = leds.b = 0
	leds.r = 255
	leds.update(mode=Leds.PULSE, timestep=0x1)
	horario = True
	encontrou = False
	ciclos_reto = 0
	print("Usando camera")
	# start controller
	
	motors = Motores()
	motors.setSpeed(250, 250)
	colour_tracker = ColorTracking2.ColourTracker()
	print("Usando camera")
	while True:
		time.sleep(0.01)
		camera = colour_tracker.run()
		if camera[1] != FORA_DE_VISAO:
			vel = int(max(VEL_MIN, abs(camera[1])/6 + VEL_MIN))
		else:
			vel = VEL_MIN
		if vel > VEL_MAX:
			vel = VEL_MAX
		if not (len(camera) == 4 and camera[2]>MOMENTO_MAX):
			print("Camera:", camera)
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
					ciclos_reto = 3
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
			print("Cone terminado")
			motors.setSpeed(0, 0)
			leds.update(mode=Leds.PULSE3, timestep=1)
			time.sleep(5)
			break
