import smbus

class Motores:
    # MEMORY MAP
    DESIREDDIRD = 0x00
    DESIREDPWMD = 0x01
    DESIREDDIRE = 0x02
    DESIREDPWME = 0x03
    LED_STATE_STATUS = 0x04
    LED_STATE_BAT = 0x05
    ADC_LOW_BYTE = 0x06
    ADC_HIGH_BYTE = 0x07
    
    LIMITE = 250

    def __init__(self, busaddr=1, devaddr=0x69):
        self.devaddr = devaddr
        self.bus = smbus.SMBus(busaddr)
        self.speed_e = self.speed_d = None

    def setSpeed(self, speedE, speedD):
	try:
		if self.speed_e != speedE:
			if (speedE > 0):
				if speedE > self.LIMITE:
					speedE = self.LIMITE
                                self.speed_e = speedE
				self.bus.write_byte_data(self.devaddr, self.DESIREDDIRE, 0)
				self.bus.write_byte_data(self.devaddr, self.DESIREDPWME, speedE)
			elif (speedE < 0):
				if speedE < -self.LIMITE:
					speedE = -self.LIMITE
				self.bus.write_byte_data(self.devaddr, self.DESIREDDIRE, 1)
				self.bus.write_byte_data(self.devaddr, self.DESIREDPWME, -speedE)
			else:
				self.bus.write_byte_data(self.devaddr, self.DESIREDPWME, 0)
			self.speed_e = speedE
		if self.speed_d != speedD:
			if (speedD > 0):
				if speedD > self.LIMITE:
					speedD = self.LIMITE
				self.bus.write_byte_data(self.devaddr, self.DESIREDDIRD, 0)
				self.bus.write_byte_data(self.devaddr, self.DESIREDPWMD, speedD)
			elif (speedD < 0):
				if speedD < -self.LIMITE:
					speedD = -self.LIMITE
				self.bus.write_byte_data(self.devaddr, self.DESIREDDIRD, 1)
				self.bus.write_byte_data(self.devaddr, self.DESIREDPWMD, -speedD)
			else:
				self.bus.write_byte_data(self.devaddr, self.DESIREDPWMD, 0)
			self.speed_d = speedD
	except Exception, e:
		print("Falha i2c", e)
    def liga_led(self, led):
        self.bus.write_byte_data(self.devaddr, led, int(True))

    def desliga_led(self, led):
        self.bus.write_byte_data(self.devaddr, led, int(False))

    def readVoltage(self, adr, factor):
        low = self.bus.read_byte_data(self.devaddr, adr)
        high = self.bus.read_byte_data(self.devaddr, adr+1)
        val = (high << 8) + low
        return val/factor
