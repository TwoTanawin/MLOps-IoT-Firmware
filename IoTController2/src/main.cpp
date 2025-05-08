#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"

#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// << Sensors Call

#include "TdsSensor.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#include "PhSensor.h"

#include "DoEstimator.h"


// MQTT topics
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define THINGNAME "IoTController1"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client(256);

// NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000); // UTC+7, update every 60s

// String station_number = "AUTO-01";
unsigned long previousMillis = 0;
// const long interval = 2 * 60 * 1000;
const long interval = 2000;
unsigned long previousResetMillis = 0;
const long resetInterval = 1800000;

TdsSensor tdsSensor(39, 5.0, 25.0, 30);

PhSensor phSensor(36, -6.80, 25.85);


#define ONE_WIRE_BUS 14  // GPIO4 or any digital pin

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#define RS485_RX 16
#define RS485_TX 17
#define RS485_CONTROL 4
HardwareSerial RS485Serial(2);  // Use UART2

String receivedData = "";
float remote_temp = 0.0, remote_sal = 0.0, remote_ph = 0.0, remote_do = 0.0;
bool hasNewData = false;

void messageHandler(String &topic, String &payload)
{
  Serial.println("Incoming: " + topic + " - " + payload);
}

void wifiSetup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi...");
  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
    if (millis() - startAttemptTime > 10000)
    {
      Serial.println("\nFailed to connect to WiFi!");
      return;
    }
  }

  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  timeClient.begin(); // Start NTP Client
}

void connectAWS()
{
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.begin(AWS_IOT_ENDPOINT, 8883, net);
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");
}

void readRS485Data() {
  while (RS485Serial.available()) {
    char c = RS485Serial.read();
    if (c == '\n') {
      receivedData.trim();
      Serial.println("Received: " + receivedData);
      // Example: TEMP:28.4,SAL:230.1,PH:7.12,DO:6.87
      int tempIdx = receivedData.indexOf("TEMP:");
      int salIdx = receivedData.indexOf("SAL:");
      int phIdx = receivedData.indexOf("PH:");
      int doIdx = receivedData.indexOf("DO:");

      if (tempIdx != -1 && salIdx != -1 && phIdx != -1 && doIdx != -1) {
        remote_temp = receivedData.substring(tempIdx + 5, salIdx).toFloat();
        remote_sal = receivedData.substring(salIdx + 4, phIdx).toFloat();
        remote_ph = receivedData.substring(phIdx + 3, doIdx).toFloat();
        remote_do = receivedData.substring(doIdx + 3).toFloat();
        hasNewData = true;

        Serial.printf("Parsed → Temp: %.2f, Sal: %.2f, pH: %.2f, DO: %.2f\n",
                      remote_temp, remote_sal, remote_ph, remote_do);
      }

      receivedData = ""; 
    } else {
      receivedData += c;  
    }
  }
}


void publishMessage()
{
  timeClient.update();

  // Get epoch time
  time_t epochTime = timeClient.getEpochTime();

  // Convert epoch time to local time struct
  struct tm *timeinfo = localtime(&epochTime);

  // Format: YYYY-MM-DD HH:MM:SS
  char formattedTimestamp[25];
  strftime(formattedTimestamp, sizeof(formattedTimestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

  String timestamp = String(formattedTimestamp);

  unsigned long t = millis() / 1000;
  
  readRS485Data();

  if (!hasNewData) {
    Serial.println("No new RS-485 data. Skipping publish.");
    return;
  }
  
  float temp_value = remote_temp;
  float salinity_value = remote_sal;
  float ph_value = remote_ph;
  float do_value = remote_do;
  hasNewData = false;

  bool status = true;
  String serialNumber = "SN-47F2B3CE";

  char jsonBuffer[512];
  snprintf(jsonBuffer, sizeof(jsonBuffer),
           "{\"serialNumber\":\"%s\",\"timestamp\":\"%s\",\"data\":{\"do_value\":%.2f,\"temp_value\":%.1f,\"salinity_value\":%.1f,\"ph_value\":%.1f,\"status\":%s}}",
           serialNumber.c_str(), timestamp.c_str(), do_value, temp_value, salinity_value, ph_value, status ? "true" : "false");

  if (client.connected())
  {
    if (client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer))
    {
      Serial.print("Published: ");
      Serial.println(jsonBuffer);
    }
    else
    {
      Serial.println("Failed to publish");
    }
  }
  else
  {
    Serial.println("Client not connected, reconnecting...");
    connectAWS();
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("System Start ...");
  randomSeed(analogRead(0));
  wifiSetup();
  connectAWS();

  tdsSensor.begin();
  sensors.begin();

  RS485Serial.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX);
  pinMode(RS485_CONTROL, OUTPUT);
  digitalWrite(RS485_CONTROL, LOW);  // Receive mode
}

void loop()
{
  client.loop(); // Keep MQTT alive

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    publishMessage();
  }

  if (currentMillis - previousResetMillis >= resetInterval)
  {
    previousResetMillis = currentMillis;
    Serial.println("Resetting ESP32...");
    ESP.restart();
  }
}
