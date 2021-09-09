import serial
import time
arduinoSearialData=serial.Serial('com5',9600)

while(True):
    arduinoSearialData.write(b'1')
    time.sleep(2)

