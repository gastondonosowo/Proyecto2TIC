import serial
import time
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
def IngresarDato(valores, id, number):
    ref = db.reference('/Valores/DatoUser'+str(id))
    ref.update({
            number:{
                'Gas': valores[0],
                'Luz': valores[1],
                'Humedad': valores[2],
                'Temperatura': valores[3],
                'Num': number
        }})
arduino = serial.Serial('COM4',9600)
time.sleep(2)

cred = credentials.Certificate('firebase-sdk.json')
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://tic-proy2-default-rtdb.firebaseio.com/'
})

array_valores = ['','','','']
id = 1
number = 0

while True:    
    dato = arduino.readline().decode("utf-8").strip('\n').strip('\r')
    dato_split = dato.split()
    if  dato_split[0] == 'Gas':
        array_valores[0] = dato_split[1]
    elif dato_split[0] == 'Luz':
        array_valores[1] = dato_split[1]
    elif dato_split[0] == 'Humedad':
        array_valores[2] = dato_split[1]
    elif dato_split[0] == 'Temperatura':
        array_valores[3] = dato_split[1]

    if '' not in array_valores:
        ref = db.reference('/Valores/DatoUser'+str(id))
        if ref.get():
            array = ref.get()
            number = len(array)
            
        IngresarDato(array_valores, id, number)
        array_valores = ['','','','']
    print(array_valores)

arduino.close()