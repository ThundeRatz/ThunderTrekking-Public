#!/usr/bin/python
from hmc5883l import getHeading
while True:
	print(getHeading())
