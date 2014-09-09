import time
import math
from motors import *
from leds import *
import ColorTracking2


DEBUG = 1
VEL_MIN = 120
VEL_MAX = 150
MARGEM = 100
CONE_MINIMO = 500
CONE_MAXIMO = 1500
FORA_DE_VISAO = 1000.

vel_rotacao = 100 # velocidade inicial
QUEDA_ROTACAO = 30 # quanto diminui?
VEL_MIN_ROTACAO = 100
	
leds = Leds()
leds.r = 255
leds.g = 0
leds.b = 0

def motor_byte(n):
	if n < 250:
		return int(n)
	return 250

if __name__ == '__main__':
	leds.update(mode=Leds.PULSE, timestep=0x1)
	colour_tracker = ColorTracking2.ColourTracker()
	motors = Motores()
	horario = True
	encontrou = False
	ciclos_reto = 0
	while True:
		time.sleep(0.01)
		camera = colour_tracker.run()
		vel = int(max(VEL_MIN, abs(camera[1])/6 + VEL_MIN))
		if vel > VEL_MAX:
			vel = VEL_MAX
		if not (camera[0]>CONE_MAXIMO):
			if camera[1] == FORA_DE_VISAO and ciclos_reto == 0:
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
					ciclos_reto = 4
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
			motors.setSpeed(0, 0)
			leds.r=0
			leds.update(mode=Leds.MANUAL)
			break;
