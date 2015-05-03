#
# tsip_parse.py - Trimble Thunderbolt TSIP parser class
#

import serial

ETX = '\x03'
DLE = '\x10'


class TSIPParser(object):
    """Parse TSIP packets.

    """
    START = 1
    FRAMEDLE = 2
    DATA = 3
    DATADLE = 4
    MAXDATA = 1024
    def __init__(self, port='/dev/ttyAMA0', baud=9600):
        """Return a TSIP parser.
        """
        self.state = self.START
        self.packets = []
        self.packet = []
        self.port = serial.Serial(port, baud)
        
    def open(self):
        self.packets = []
        if not self.port.isOpen():
            self.port.open()

    def close(self):
        if self.port.isOpen():
            self.port.close()
            
    def parse(self, c):
        """Parse single TSIP character.
        
        If packet made return True otherwise False
        """
        # search for framing DLE
        if self.state == self.START:
            if c == DLE:
                self.state = self.FRAMEDLE
        # found framing DLE     
        elif self.state == self.FRAMEDLE:
            # mis-framed
            if c == DLE or c == ETX:
                self.state = self.START
            # start packet    
            else:
                self.state = self.DATA
                self.packet = []    
                self.packet.append(c)
        # accumulating packet data        
        elif self.state == self.DATA:
            # found data DLE
            if c == DLE:
                self.state = self.DATADLE
            # add byte to packet    
            else:
                self.packet.append(c)
        # found data DLE        
        elif self.state == self.DATADLE:
            # store second data DLE
            if c == DLE:
                self.packet.append(c)
                self.state = self.DATA
            # found end of packet    
            elif c == ETX:
                #self.packets.append(''.join(self.packet))
                self.state = self.START
                return True
            # mis-framed    
            else:
                self.state = self.START
        # should not get here
        else:
            raise RuntimeError('Invalid state: %d' % self.state)        
        return False

    def do_parse(self):
        while True:
            if self.port.inWaiting() > 0:
                if self.parse(self.port.read()):
                    pkt = ''.join(self.packet)
                    hexpkt = []
                    for ch in pkt:
                        hexpkt.append('%02x' % ord(ch))
                    print ''.join(hexpkt)


if __name__ == '__main__':
    TSIPParser().do_parse()

