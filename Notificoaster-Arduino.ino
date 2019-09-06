#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"


void setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.begin("SSID", "password"); // replace with your SSID and password

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

float readTemperature()
{
    float temp = analogRead(A0);
    temp = temp * 0.0864;
    int a = round(temp * 10);
    float newTemp = a / 10.0;
    temp -= 1;
    return temp;
}

void loop() 
{
  if(WiFi.status()== WL_CONNECTED) {   //Check WiFi connection status
    float temperature = readTemperature();
   
    HTTPClient http;
    http.begin(URL_SECRET);   // taken from the config.h file
    http.addHeader("Content-Type", "application/json");
  
    StaticJsonBuffer<200> jsonBuffer;
    String postMessage;
  
    JsonObject& root = jsonBuffer.createObject();
    root["deviceID"] = DEVICE_ID;   // taken from the config.h file
    root["temp"] = temperature;
    root.printTo(postMessage);
    int httpCode = http.POST(postMessage);
    http.writeToStream(&Serial);
    Serial.println(httpCode);
    http.end();
 
 }
 else {
    Serial.println("Error in WiFi connection");   
 }
  delay(60000);  //Send a request every 60 seconds
}
