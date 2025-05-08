#include <Arduino.h>
#include <SoftwareSerial.h>

#include "TdsSensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "PhSensor.h"
#include "DoEstimator.h"

// Sensor setup
TdsSensor tdsSensor(A5, 5.0, 25.0, 30);
PhSensor phSensor(A4, -6.80, 25.85);
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Timing
unsigned long lastSampleTime = 0;
const unsigned long sampleInterval = 1000;

// UART communication via SoftwareSerial (not RS-485)
SoftwareSerial sensorSerial(10, 11);  // RX, TX

void callSensorsValue();
void sendViaSerial(const String& data);

void setup() {
  Serial.begin(115200);        // USB Serial for debugging
  sensorSerial.begin(9600);    // UART to external device

  Serial.println("System Start ...");

  tdsSensor.begin();
  sensors.begin();
}

void sendViaSerial(const String& data) {
  sensorSerial.print(data);
  sensorSerial.flush();
}

void callSensorsValue() {
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

  Serial.println(payload);     // Print to USB serial
  sendViaSerial(payload);     // Send to external device via TX
}

void loop() {
  unsigned long currentTime = millis();
  if (currentTime - lastSampleTime >= sampleInterval) {
    lastSampleTime = currentTime;
    callSensorsValue();
  }
}
