import serial
import time
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

arduino = serial.Serial('COM4',9600)
time.sleep(2)
i = 1
while(i > 0):
    dato = arduino.readline()
    print(dato)
arduino.close()

cred = credentials.Certificate('firebase-sdk.json')
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://proyecto-tics-2-default-rtdb.firebaseio.com/'

})

ref = db.reference('/')
ref.set({
    'Datos':
        {
            'pruebadatos': {
                        'dato': dato
            }
        }
})