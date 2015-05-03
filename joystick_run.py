#!/usr/bin/python
import joystick
from motors import *
import time

# motors = Motores(FALTA COMPLETAR AQUI)
HORIZONTAL = 0
VERTICAL = 1

PROP_MIXAGEM = 1.5

motors = Motores()

def mixagem(eixo_x, eixo_y):	# array [float, float] com eixo x e y, retorna floats para esquerda e direita
	x = abs(eixo_x)
	y = abs(eixo_y)
	
	if x == 0 and y == 0:
		return [0., 0.]
	
	tan = (y - x) / (y + x)
	
	if y > x:
		speed = y
		tan = tan * (PROP_MIXAGEM - 1) + (PROP_MIXAGEM - 1.)
	else:
		speed = x
		tan = tan * PROP_MIXAGEM + (PROP_MIXAGEM - 1.)
	if eixo_x >= 0 and eixo_y >= 0:
		return [speed, tan * speed]
	elif eixo_x < 0 and eixo_y >= 0:
		return [tan * speed, speed]
	elif eixo_x < 0 and eixo_y < 0:
		return [-speed, -tan * speed]
	elif eixo_x >= 0 and eixo_y < 0:
		return [-tan * speed, -speed]

def atualiza(eixo_x, eixo_y):
	eixo_y = -eixo_y
	print("Eixos: x %f, y %f" % (eixo_x, eixo_y))
	mix = mixagem(eixo_x, eixo_y)
	print("Mixagem: %f %f" % (mix[0], mix[1]))
	return mix

joystick.init()
while True:
	time.sleep(0.05)
	modificados = joystick.get()
	if VERTICAL in modificados[0] or HORIZONTAL in modificados[0]:
		mix = atualiza(joystick.axes[HORIZONTAL], joystick.axes[VERTICAL])
		motors.setSpeed(int(255 * mix[0]), int(255 * mix[1]))
