#include <Arduino.h>
#include "SPIFFS.h"
#include <WiFi.h>
#include "secrets.h"

#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// MQTT topics
#define AWS_IOT_PUBLISH_TOPIC "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
#define THINGNAME "IoTController1"

WiFiClientSecure net = WiFiClientSecure();
MQTTClient client(256);

// NTP
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000); // UTC+7, update every 60s

// CSV File object
File csvFile;

// Parsed sensor values (global)
float ph_value, do_value, temp_value, salinity_value;

unsigned long previousMillis = 0;
const long interval = 2000; // 2 seconds
unsigned long previousResetMillis = 0;
const long resetInterval = 1800000; // 30 minutes

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

  timeClient.begin();
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

bool readNextSensorData()
{
  if (!csvFile.available())
  {
    Serial.println("Reached end of file. Rewinding...");
    csvFile.close();
    csvFile = SPIFFS.open("/water_balanced_500_rows.csv");
    if (!csvFile)
    {
      Serial.println("Failed to reopen CSV.");
      return false;
    }
    csvFile.readStringUntil('\n'); // Skip header again
  }

  String line = csvFile.readStringUntil('\n');
  line.trim();

  if (line.length() == 0)
    return false;

  // Parse CSV: ph_value, do_value, temp_value, salinity_value
  int c1 = line.indexOf(',');
  int c2 = line.indexOf(',', c1 + 1);
  int c3 = line.indexOf(',', c2 + 1);

  if (c1 == -1 || c2 == -1 || c3 == -1)
  {
    Serial.println("Invalid CSV line: " + line);
    return false;
  }

  ph_value = line.substring(0, c1).toFloat();
  do_value = line.substring(c1 + 1, c2).toFloat();
  temp_value = line.substring(c2 + 1, c3).toFloat();
  salinity_value = line.substring(c3 + 1).toFloat();

  Serial.printf("Parsed CSV -> pH: %.2f, DO: %.2f, Temp: %.2f, Salinity: %.2f\n",
                ph_value, do_value, temp_value, salinity_value);

  return true;
}

void publishMessage()
{
  if (!readNextSensorData())
    return;

  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *timeinfo = localtime(&epochTime);

  char formattedTimestamp[25];
  strftime(formattedTimestamp, sizeof(formattedTimestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
  String timestamp = String(formattedTimestamp);

  String serialNumber = "SN-7DA08233";
  bool status = true;

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

  wifiSetup();
  connectAWS();

  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  csvFile = SPIFFS.open("/water_balanced_500_rows.csv");
  if (!csvFile)
  {
    Serial.println("CSV File not found");
    return;
  }

  csvFile.readStringUntil('\n'); // Skip header
}

void loop()
{
  client.loop();

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
