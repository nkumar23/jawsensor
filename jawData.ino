// IoT Workshop
// Send temperature and humidity data to MQTT
//
// WiFiNINA https://www.arduino.cc/en/Reference/WiFiNINA (MKR WiFi 1010)
// Arduino MKR ENV https://www.arduino.cc/en/Reference/ArduinoMKRENV
// Arduino MQTT Client  https://github.com/arduino-libraries/ArduinoMqttClient

#include <WiFiNINA.h>
//#include <Arduino_MKRENV.h>
#include <ArduinoMqttClient.h>

#include "config.h"

WiFiSSLClient net;
MqttClient mqtt(net);

String jawTopic = "jawbone/" + DEVICE_ID;


// Publish every 10 seconds for the workshop. Real world apps need this data every 5 or 10 minutes.
unsigned long publishInterval = .5 * 1000;
unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);

//  // Wait for a serial connection... comment out in production
//  while (!Serial) { }

//  // initialize the shield
//  if (!ENV.begin()) {
//    Serial.println("Failed to initialize MKR ENV shield!");
//    while (1);
//  }
 
  Serial.println("Connecting WiFi");
  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqtt.connected()) {
    connectMQTT();
  }
  
  // poll for new MQTT messages and send keep alives
  mqtt.poll();

  if (millis() - lastMillis > publishInterval) {
    lastMillis = millis();

    int jawData = analogRead(A1);

    Serial.print(jawData);
    Serial.println(" jaw data");
    
    mqtt.beginMessage(jawTopic);
    mqtt.print(jawData); 
    mqtt.endMessage();

  }  
}

void connectWiFi() {
  // Check for the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  Serial.print("WiFi firmware version ");
  Serial.println(WiFi.firmwareVersion());
  
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(WIFI_SSID);
  Serial.print(" ");

  while (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(3000);
  }

  Serial.println("Connected to WiFi");
  printWiFiStatus();

}

void connectMQTT() {
  Serial.print("Connecting MQTT...");
  mqtt.setId(DEVICE_ID);
  mqtt.setUsernamePassword(MQTT_USER, MQTT_PASSWORD);

  while (!mqtt.connect(MQTT_BROKER, MQTT_PORT)) {
    Serial.print(".");
    delay(5000);
  }

  Serial.println("connected.");
}

void printWiFiStatus() {
  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
