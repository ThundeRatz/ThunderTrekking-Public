import smbus
import time
bus = smbus.SMBus(1)
address = 0x50

def bearing255():
        bear = bus.read_i2c_block_data(address, 0x5F, 4)
        return bear

while True:
	bear255 = bearing255()      #this returns the value as a byte between 0 and 255. 
        print bear255
        time.sleep(0.1)