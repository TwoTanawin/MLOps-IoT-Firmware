#include <Arduino.h>

#include "TdsSensor.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#include "PhSensor.h"
#include "DoEstimator.h"


TdsSensor tdsSensor(A5, 5.0, 25.0, 30);
PhSensor phSensor(A4, -6.80, 25.85);
#define ONE_WIRE_BUS 2  

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void callSensorsValue();

unsigned long lastSampleTime = 0;
const unsigned long sampleInterval = 2000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("System Start ...");
  
  tdsSensor.begin();
  sensors.begin();
}

void callSensorsValue(){
  sensors.requestTemperatures(); 
  float temp_value = sensors.getTempCByIndex(0);

  tdsSensor.update();   
  float salinity_value = tdsSensor.getTdsValue();  
 
  float voltage = phSensor.readPhVoltage();
  float ph_value = phSensor.readPh(); 

  float do_value = DoEstimator::estimate(temp_value, salinity_value); 

  Serial.print("Temp: "); Serial.print(temp_value); Serial.println(" C");
  Serial.print("Salinity: "); Serial.print(salinity_value); Serial.println(" ppm");
  Serial.print("pH: "); Serial.println(ph_value);
  Serial.print("DO: "); Serial.println(do_value);
  Serial.println("------");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastSampleTime >= sampleInterval) {
    lastSampleTime = currentTime;
    callSensorsValue();
  }
}
