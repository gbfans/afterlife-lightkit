#include <Arduino.h>

#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

const byte        DNS_PORT = 53;          
IPAddress         apIP(10, 10, 10, 1);    
DNSServer         dnsServer;              
ESP8266WebServer  webServer(80);

#ifndef APSSID
#define APSSID "AFTERLIFE " + WiFi.macAddress()
#define APPSK  "ghostbusters"
#endif

String responseHTML = "<!DOCTYPE html>"
  "<html lang=\"en\">"
  "<head>"
    "<meta charset=\"utf-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "<title>Hello World</title>"
  "</head>"
  "<body>"
  "<p>Hello, World!</p>"
  "</body>"
  "</html>";
  
void setup() {

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(APSSID, APPSK);

  dnsServer.start(DNS_PORT, "*", apIP);
  
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", responseHTML);
  });
  webServer.begin();

  LittleFS.format();
  Serial.println("Mount LittleFS");
  if (!LittleFS.begin()) {
    //error
  }

  EEPROM.begin(512);
  
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}
