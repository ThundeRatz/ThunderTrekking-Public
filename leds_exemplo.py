import time
from leds import *

leds = Leds()
leds.update(mode=Leds.PULSE3, timestep=0x1)
time.sleep(3)

leds.r = leds.g = leds.b = 0xf
leds.update(mode=Leds.BLINK, timestep=0x30)
