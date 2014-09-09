import struct
import math
Output_humana = False #Define se usaremos o modo de output humana (nao compativel com a main do GPS)
def hextofloat(x):
        """Converts hexadecimal to single precision float using the struct library"""
        return struct.unpack('f', x.decode('hex'))[0]
def hextofloatlist(x):
        "The function hextofloat applied to a list"""
        floatx=[]
        for i in x:
                floatx.append(hextofloat(i))
        return floatx
def tsip_interpreter(packet):
        """Interprets any packet encoded with the TSIP protocol, first 
        recognizing the type of packet based on its size, then separating 
        the packet in its components, and finally translating that 
        information to human language"""
        ID=packet[0:2]#Packet identification byte
        packet=packet[2:len(packet)]
        if ID=='6d':#Packet: all-in-view satellite selection, DOPs, fix mode (varying length due to number of satellites used) (0X6D)
                DOPS=hextofloatlist([packet[i:i+8] for i in range(2, 34, 8)])
                if Output_humana:
					return('PDOP = %f, HDOP = %f, VDOP = %f, TDOP = %f' %(DOPS[0],DOPS[1],DOPS[2],DOPS[3]))
				else:
					return([ID, DOPS])
        elif ID=='4a':#Packet: GPS position fix (latitude, longitude, altitude) (0X4A)
                LLA=hextofloatlist([packet[i:i+n] for i in range(0, len(packet), 8)])
                if Output_humana:
					return('Latitude = %f rad, Longitude = %f rad, Altitude = %f m, Clock Bias = %f m, Time-of-fix = %f s' %(LLA[0],LLA[1],LLA[2],LLA[3],LLA[4]))
				else:
					return([ID, LLA])
        elif ID=='82':#Packet: DGPS position fix mode (0X82)
                mode=int(packet[0:2])#0, (Manual GPS), 1 (Manual DGPS), 2 (Auto DGPS OFF), 3 (Auto DGPS ON)
                if Output_humana:
					return('Mode ',mode)
				else:
					return([ID, mode])
        elif ID=='41':#Packet: GPS time (0x41)
                time_of_week, GPS_offset=hextofloatlist([packet[0:8],packet[12:20]])#iN seconds
                extended_week=int(packet[8:12], 16)#in number of weeks
                year=1980+math.floor(extended_week/52)
                week=int(((extended_week/52)-math.floor(extended_week/52))*52)
                if Output_humana:
					return("Year: %d, Week: %d, Time of Week: %d, GPS offset: %d" %(year, week, time_of_week, GPS_offset))
				else:
					return([ID, [year, week, time_of_week, GPS_offset]])
        elif ID=='46':#Packet: health of receiver (0x46)
                status=packet[0:2]
                status_list=[0]*16
                status_list[0]='Doing position fixes'
                status_list[1]='Dont have GPS time yet'
                status_list[2]='Need initialization (0=normal, 1=shutdown due to RF initialization timeout)'
                status_list[3]='PDOP is too damn high'
                status_list[8]='No usable satellites'
                status_list[9]='Only 1 usable satellite'
                status_list[10]='Only 2 usable satellites'
                status_list[11]='Only 3 usable satellites'
                status_list[12]='The chosen satellite is unusable'
                if Output_humana:
					return(status_list[int(status, 16)])
				else:
					return([ID, status_list[int(status, 16)]])
        elif ID=='4b':#Packet: machinecode/status (0x4B)
                Machine_ID=int(packet[0:2], 16)
                if Output_humana:
					return('Machine ID: ', Machine_ID)
				else:
					return([ID, Machine_ID])	
        elif ID=='56':#Packet: Velocity Fix (ENU) (0X56)
                east,north,up,clock,time=hextofloatlist([packet[i:i+8] for i in range(0, len(packet), 8)])               
                if Output_humana:
					return('The velocity is: East - %f m/s, North - %f m/s, Up - %f m/s. Clock bias: %f, Time of Fix: %f' %(east,north,up,clock,time))
				else:
					return([ID, [east,north,up,clock,time])
