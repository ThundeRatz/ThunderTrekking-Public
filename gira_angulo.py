VEL = 120
from motors import *
from hmc5883l import getHeading
motors = Motores()
def gira(angulo):
	angulo %= 360
	tolerancia=5
	angulo_atual = getHeading()
	motors.setSpeed(0, 0)
	delta = angulo-angulo_atual
	if delta < -180:
		delta += 360
	if delta > 180:
		delta -= 360 #correcoes da defasagem entre os angulos
	if delta < 0:
		motors.setSpeed(-VEL, VEL)
		while not (angulo-tolerancia)<getHeading()<(angulo+tolerancia):
			nada=0
		motors.setSpeed(0, 0)
	else:
		motors.setSpeed(VEL, -VEL)
		while not (angulo-tolerancia)<getHeading()<(angulo+tolerancia):
			nada=0
		motors.setSpeed(0, 0)
