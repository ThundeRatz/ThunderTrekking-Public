from motors import *
from hmc5883l import getHeading
motors = Motores()
motors.setSpeed(127, -127)
while True:
	print(getHeading())
