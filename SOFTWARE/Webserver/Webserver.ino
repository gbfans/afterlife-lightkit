#include <Arduino.h>

#include <FS.h>
#include <LittleFS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <EEPROM.h>

const byte        DNS_PORT = 53;
IPAddress         apIP(10, 10, 10, 1);
DNSServer         dnsServer;

// Create AsyncWebServer object on port 80
AsyncWebServer webServer(80);

#ifndef APSSID
#define APSSID "AFTERLIFE " + WiFi.macAddress()
#define APPSK  "ghostbusters"
#endif

void setup() {

  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(APSSID, APPSK);

  dnsServer.start(DNS_PORT, "*", apIP);

  // Initialize LittleFS
  if(!LittleFS.begin()){
    /*
     * TODO: If this error occurs, we should still
     * continue executing, but the Wifi Configuration feature
     * will be unavailable. Return a hard-coded HTML error page?
     */
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  webServer.on("/trigger", HTTP_POST, [] (AsyncWebServerRequest *request) {
    String inputMessage;

    if (request->hasParam("action", true)) {
      inputMessage = request->getParam("action", true)->value();
      if (inputMessage == "On") {
        digitalWrite(LED_BUILTIN, HIGH);
      } else if(inputMessage == "Off") {
        digitalWrite(LED_BUILTIN, LOW);
      }
    }

    // Redirect back to main page
    AsyncWebServerResponse *response = request->beginResponse(302);
    response->addHeader("Location", String("/"));

    request->send(response);
  });

  webServer.onNotFound([](AsyncWebServerRequest *request) {
    // Serve index.html for all requests
    request->send(LittleFS, "/index.html", String(), false);
  });

  webServer.begin();

  EEPROM.begin(512);
}

void loop() {
  dnsServer.processNextRequest();
}
