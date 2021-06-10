import serial
import time

arduino = serial.Serial('COM4',9600)
time.sleep(2)
i = 1
while(i > 0):
    dato = arduino.readline()
    print(dato)
arduino.close()