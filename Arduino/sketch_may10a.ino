#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float parametrizar(String type, float rawValue){
    if(type == "Gas"){
        return (rawValue*100)/1023;
    }
     else if(type == "LDR"){
        return (rawValue*100)/496;
    }
}

void Alertas(String type, float value){
    float param = parametrizar(type, value);
    if(type == "Gas"){
        if(param > 65){
            Serial.print("Gas ALTO: ");
            Serial.println(param);
        }else if( param > 40){
            Serial.print("Gas NORMAL: ");
            Serial.println(param);
        }else if(param >= 0){
            Serial.print("Gas BAJO: ");
            Serial.println(param);
        }
    }else if (type == "LDR"){
        if(param > 75){
            Serial.print("Luz ALTA: ");
            Serial.println(param);
        }else if(param > 50){
            Serial.print("Luz NORMAL: ");
            Serial.println(param);
        }else if(param >= 0){
            Serial.print("Luz BAJA: ");
            Serial.println(param);
        }
    }else if (type == "Humedad"){
        if(value > 60){
            Serial.print("Humedad ALTA: ");
            Serial.println(value);
        }else if( value > 30){
            Serial.print("Humedad NORMAL: ");
            Serial.println(value);
        }else if( value >= 20){
            Serial.print("Humedad BAJA: ");
            Serial.println(value);
        }
    }else{
        if(value > 28){
            Serial.print("Temperatura ALTA: ");
            Serial.println(value);
        }else if( value > 20){
            Serial.print("Temperatura NORMAL: ");
            Serial.println(value);
        }else if( value >= 0){
            Serial.print("Temperatura BAJA: ");
            Serial.println(value);
        }
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
  delay(400);

}
