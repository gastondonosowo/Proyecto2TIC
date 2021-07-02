# Manager 3000 Plus
Proyecto 2 TIC
## A. Documentación de diseño
### Diagrama de ensamblaje
    Para la fabricación del dispositivo, se deberá contar en primera instancia con un Arduino uno, el cual será revisado para verificar que no cuenta con ninguna imperfección antes de proceder a la línea principal de ensamblaje.

    Se tendrá por un lado el montaje base de cada sensor, conectados con sus correspondientes resistencias y condensadores. Al tener estos circuitos listos se procede a conectarlos al Arduino uno por uno. Al llegar al módulo wifi, se verifica     que este no esté defectuoso, una vez verificado se procede a conectarlo con el Arduino. 

    El código del Arduino se le hará un Debugging antes de integrarlo a la línea principal, pues se quiere disminuir la probabilidad de errores antes de la fase final, la cual es una prueba de estrés, por lo que cualquier error o “bug” en el código causará problemas.

## B. Documentación de operación y uso de artefacto
    El proyecto Manager 3000 se centra en la administración de ambientes, siendo el objetivo principal, el garantizar que el usuario posea un entorno óptimo para sus necesidades diarias. El dispositivo despliega los datos de humedad, temperatura, gas CO2 y luminosidad de una habitación, posibilitando al usuario regular estos parámetros , por medio de la aplicación ,como estime conveniente.

    El artefacto cuenta con 3 principales ejes de operación:
    
        1.Arduino y sensores configurados para capturar datos.
    
        2.Base de datos Firebase, que almacena los datos proporcionados por el arduino.
    
        3.Aplicación móvil con funcionalidad de receptor, interpretador y usuario de los datos capturados por el arduino.    
    -En el eje de operación de Arduino, se encuentran los sensores que realizan la captura de datos; sensor de gas, sensor de humedad, sensor de temperatura y sensor de luz en base a fotorresistor , estos se conectan directamente a la base de datos de Firebase referenciando los datos interpretados por los sensores.
    -En las base de datos se almacenan los datos respectivos al usuario, con un index de fácil referenciación (Ejemplo: DatosUsuario1), en esta se almacenan los datos de “Gas”, “Luz”, “Humedad” y “Temperatura”, seguido a su vez por un dato “Cantidad” que referencia la cantidad de veces que se han agregado los datos en la base respectiva.
    -En el eje de aplicación móvil, se estructuró y programó una aplicación móvil a través del framework React Native, el cual permite exportar a plataforma Web, Android y IOs. En esta aplicación se observa en un principio un ingreso de cuenta y registro correspondiente para almacenar los datos con respecto al usuario que desee utilizar el artefacto, cuando la acción de registro seguido por acceso a la aplicación se ejecuta correctamente, por consiguiente, nos encontraremos dentro del menú de la aplicación, componente que posee:
    1.Display.
      En esta pestaña el usuario puede observar los módulos conectados con su aplicación y a su vez los datos en tiempo real que capturan los artefactos, desplegando los principales valores de interés para su supervisión.
    2.Resumen.
      En esta pestaña el usuario podrá observar un gráfico de los datos a través del tiempo que han sido capturados, entregando una información contextual más acertada del entorno para asimilar la información en distintos momentos lo que pueda conllevar a una identificación de problemas y/o toma de decisiones.
    3.Configuración.
      Esta pestaña está diseñada para que el usuario pueda cambiar los niveles de sensibilidad que vienen por defecto con los que se alertan valores “extremos” captados del entorno, otorgando flexibilidad de utilidad para los rangos en los que se ejecuta la notificación de alerta.
    4.Suscripción.
      En esta pestaña el usuario podrá realizar la modificación de su credencial de usuario básico a un usuario de suscripción pagada, esto le permitirá principalmente insertar a la conexión más de 4 módulos de captura, ideado principalmente para empresas o usuarios que requieran supervisión de distintos entornos en tiempo real.
      
## Códigos
### Código de Arduino
    Este código escrito en C++ es el encargado de capturar los datos otorgados por los sensores a través de la función analogRead que referencian a los pines analógicos de la placa arduino.
    Luego a través de una función Alerta, se referencia 4 veces, función la cual permite desplegar los valores en base al parámetro de nombre de circuito entregado en la consola, siendo la consola conectada a un puerto de la red lo cual permite entregar los datos al servidor.

```
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
```

### Código de conexión al servidor
    Este código escrito en python es el encargado de enviar los datos capturados por el Arduino hasta la base de datos creada en Firebase.
    Esto lo hace conectándose mediante un puerto serial con el dispositivo Arduino y así es posible recibir los datos por parte del programa en python, estos datos que se reciben son modificados de tal manera que quede un string lo más limpio posible para poder enviarlos de  forma correcta a la base de datos.
    A la base de datos  se conecta mediante una credencial entregada por esta misma, haciendo que la conexión sea sencilla,  y logrando que solo con un push de los datos ya estos están en tiempo real en Firebase.


```
import serial """Se importa una librería que conecta python con el arduino mediante serial port"""
import time
import firebase_admin """Se importa una librería que conecta python con una base de datos en firebase"""
from firebase_admin import credentials
from firebase_admin import db
def IngresarDato(valores, id, number):
    ref = db.reference('/Valores/DatoUser'+str(id)) """Se ingresa a la tabla de valores del usuario según id"""
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
        }})"""Ingreso de datos recibidos a firebase
arduino = serial.Serial('COM4',9600)"""Se logra comunicar el codigo de python con los datos enviados por Arduino mediante serial port"""
time.sleep(2)

cred = credentials.Certificate('firebase-sdk.json') """Se obtienen las llaves con las que se puede ingresar a nueestra base de datos"""
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://tic-proy2-default-rtdb.firebaseio.com/'
}) """ Se logra conectar la base de datos con python"""

array_valores = ['','','','']
id = 1
number = 0

while True:    
    dato = arduino.readline().decode("utf-8").strip('\n').strip('\r')"""Se obtienen los datos enviados por el dispositivo arduino y se limpia de valores innecesarios el string de llegada"""
    dato_split = dato.split()
    if  dato_split[0] == 'Gas':
        array_valores[0] = dato_split[1]
    elif dato_split[0] == 'Luz':
        array_valores[1] = dato_split[1]
    elif dato_split[0] == 'Humedad':
        array_valores[2] = dato_split[1]
    elif dato_split[0] == 'Temperatura':
        array_valores[3] = dato_split[1]
    """Se identifica que sensor envió los datos"""
    if '' not in array_valores:
        ref = db.reference('/Valores/DatoUser'+str(id))
        if ref.get(): """Se obtiene el total de datos almacenados por el usuario en la base de datos"""
            array = ref.get()
            number = len(array)+1
            
        IngresarDato(array_valores, id, number)"""Envio de datos a la función que conecta finalmente con la base de datos"""
        array_valores = ['','','','']
    print(array_valores)

arduino.close()
```
## D. Resultados de las pruebas
    Las pruebas realizadas fueron las siguientes:
    -Se interpretaron los datos capturados por los sensores, ya que estos son expresados en su forma inicial de voltaje, por lo que se tuvo que realizar una conversión de estos a información utilizable, esto se realizó realizando recopilación inicial de los datos para compararlos con sensores reales (por ejemplo, el sensor de luminosidad creado se comparó con un luxómetro para comprobar sus valores), de esta forma se comprueba que la calibración de los sensores fue correcta.
    -Se realizó una prueba de conexión entre servidor arduino y la base de datos, esto se comprobó mediante la identificación de los datos en la interfaz gráfica que otorga google en la plataforma web de Firebase, a su vez, de esta manera se revisa la integridad de los datos con respecto a los valores de la base de datos con respecto a la consola en arduino.
    -Se realizó una prueba de conexión en tiempo real entre los datos de la base de datos y la aplicación móvil, a través de la librería de Firebase para react native, se logran desplegar correctamente los datos que van siendo insertados por el arduino a la base de datos, de esta manera, con ayuda de las funciones nativas del framework (useState y useEffect) se logran actualizar los datos en tiempo real mediante cambian en la base de datos.
