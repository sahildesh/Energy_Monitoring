import serial
arduinoSerialData=serial.Serial('com3',9600)

while(True):
    if (arduinoSerialData.inWaiting() > 0):
        myData = arduinoSerialData.readline()
        print(myData)