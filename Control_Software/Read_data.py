import serial
import time
SerialData=serial.Serial('com5',9600)

while(True):
    #if(SerialData.inWaiting()>0):
    myData=SerialData.readline()
    print(myData)
    time.sleep(2)