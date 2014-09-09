import smbus
class Leds:
    # Modos:
    MANUAL = 0x0
    BLINK = 0x1
    PULSE = 0x2
    PULSE3 = 0x3
    
    # Registradores:    
    COR = 0x00
    LOWADC = 0x03
    HIGHADC = 0x04
    MODE = 0x05
    TIMESTEP = 0x06
    
    device = MANUAL
    saved_timestep = 0xa
    saved_mode = 0
    r = g = b = 0
    
    def __init__(self, busaddr=0x1, device=0x24):
        self.device = device
        self.bus = smbus.SMBus(busaddr)

    def update(self, mode=None, timestep=None):
        self.bus.write_i2c_block_data(self.device, self.COR, [self.r, self.g, self.b])
        if mode != None:
            self.saved_mode = mode
        if timestep != None:
            self.saved_timestep = timestep
        self.bus.write_byte_data(self.device, self.MODE, self.saved_mode)
        self.bus.write_byte_data(self.device, self.TIMESTEP, self.saved_timestep)

    def readVoltage(self, adr, factor):
        low = self.bus.read_byte_data(self.device, adr)
        high = self.bus.read_byte_data(self.device, adr+1)
        val = (high << 8) + low
        return val/factor
