#!/usr/bin/python

#referencia: http://blog.bitify.co.uk/2013/11/connecting-and-calibrating-hmc5883l.html

import smbus
import time
import math

bus = smbus.SMBus(1)
address = 0x1e

MAG_DECLINATION = -21.9
Fator_de_conversao = -100.3
def read_byte(adr):
    return bus.read_byte_data(address, adr)

def read_word(adr):
    high = bus.read_byte_data(address, adr)
    low = bus.read_byte_data(address, adr+1)
    val = (high << 8) + low
    return val

def read_word_2c(adr):
    val = read_word(adr)
    if (val >= 0x8000):
        return -((65535 - val) + 1)
    else:
        return val

def write_byte(adr, value):
    bus.write_byte_data(address, adr, value)

def getHeading():
    write_byte(0, 0b01110000) # Set to 8 samples @ 15Hz
    write_byte(1, 0b00100000) # 1.3 gain LSb / Gauss 1090 (default)
    write_byte(2, 0b00000000) # Continuous sampling

    scale = 0.92

    x_offset = 22.5
    y_offset = -131
    x_out = (read_word_2c(3) - x_offset) * scale
    y_out = (read_word_2c(7) - y_offset) * scale
    z_out = (read_word_2c(5)) * scale

    bearing  = math.atan2(y_out, x_out) 
    if (bearing < 0):
        bearing += 2 * math.pi

    bearing = math.degrees(bearing)+MAG_DECLINATION+Fator_de_conversao
    if (bearing < 0):
        bearing = 360+bearing

    return bearing
