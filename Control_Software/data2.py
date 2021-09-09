import serial
import csv
arduinoSerialData=serial.Serial('com3',9600)
rows=[]

while(True):
    if (arduinoSerialData.inWaiting() > 0):
        myData = arduinoSerialData.readline()
        print(myData)