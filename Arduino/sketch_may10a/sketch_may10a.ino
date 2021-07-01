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
