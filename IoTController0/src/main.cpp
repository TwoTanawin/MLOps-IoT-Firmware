#include <Arduino.h>
#include <SoftwareSerial.h>

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

unsigned long lastSampleTime = 0;
const unsigned long sampleInterval = 1000;

// MAX485 RS-485 communication
#define RE_DE 8           // Direction control pin
SoftwareSerial rs485Serial(10, 11);  // RX, TX

void callSensorsValue();
void sendViaRS485(const String& data);

void setup() {
  Serial.begin(115200);
  rs485Serial.begin(9600);
  pinMode(RE_DE, OUTPUT);
  digitalWrite(RE_DE, LOW); // Default to RX

  Serial.println("System Start ...");

  tdsSensor.begin();
  sensors.begin();
}

void sendViaRS485(const String& data) {
  digitalWrite(RE_DE, HIGH);   // Enable TX mode
  delay(1);                    // Wait before sending
  rs485Serial.print(data);     // Send data
  rs485Serial.flush();         // Wait until data sent
  digitalWrite(RE_DE, LOW);    // Back to RX mode
}

void callSensorsValue(){
  sensors.requestTemperatures();
  float temp_value = sensors.getTempCByIndex(0);

  tdsSensor.update();
  float salinity_value = tdsSensor.getTdsValue();

  float voltage = phSensor.readPhVoltage();
  float ph_value = phSensor.readPh();

  float do_value = DoEstimator::estimate(temp_value, salinity_value);

  String payload = "TEMP:" + String(temp_value) +
                   ",SAL:" + String(salinity_value) +
                   ",PH:" + String(ph_value) +
                   ",DO:" + String(do_value) + "\n";

  Serial.println(payload);
  sendViaRS485(payload);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastSampleTime >= sampleInterval) {
    lastSampleTime = currentTime;
    callSensorsValue();
  }
}
