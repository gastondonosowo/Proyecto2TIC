# Manager 3000 Plus
Proyecto 2 TIC

## Código de Arduino
´´´
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void Alertas(String type, float value){
    if(type == "Gas"){
        Serial.print("Gas ");
        Serial.println(value);
    }else if (type == "LDR"){
        Serial.print("Luz ");
        Serial.println(value);
    }else if (type == "Humedad"){
        Serial.print("Humedad ");
        Serial.println(value);
    }else{
        Serial.print("Temperatura ");
        Serial.println(value);
    }
}
void setup() {
  Serial.begin(9600);
  dht.begin();
}
void loop() {
  float valorLDR = analogRead(A1);
  float valorGas = analogRead(A0);
  float valorHumedad = dht.readHumidity();
  float valorTemp = dht.readTemperature();
    
  Alertas("Gas",valorGas);
  Alertas("LDR",valorLDR);
  Alertas("Temperatura",valorTemp);
  Alertas("Humedad",valorHumedad);
  delay(6000);
}
´´´

## Código de Python
Envio de datos desde Arduino a base de datos Firebase

´´´
import serial
import time
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db
def IngresarDato(valores, id, number):
    ref = db.reference('/Valores/DatoUser'+str(id))
    ref.update({
            "Cantidad":{
                'Num': number
            },
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
            number = len(array)+1
            
        IngresarDato(array_valores, id, number)
        array_valores = ['','','','']
    print(array_valores)

arduino.close()
´´´