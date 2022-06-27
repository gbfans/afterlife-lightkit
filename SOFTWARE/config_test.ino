#include <Arduino.h>

#include <FS.h>
#include <LittleFS.h>
#include <EEPROM.h>
#include <StreamUtils.h>
#include <ArduinoJson.h>

void setup() {

  Serial.begin(115200);

  if(!LittleFS.begin()){
    /*
     * TODO: If this error occurs, we should still
     * continue executing, but the Wifi Configuration feature
     * will be unavailable. Return a hard-coded HTML error page?
     */
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  File configFile = LittleFS.open("/config.json", "r");
  if (!configFile){
    Serial.println("Opening config file failed.");
  }

  StaticJsonDocument<512> doc;
  if (configFile.available())
  {
    doc = configFile.readString();
  }
  configFile.close();
  serializeJsonPretty(doc, Serial);
  EEPROM.begin(4098);

  EepromStream eepromStream(0, 4098);
  serializeJson(doc, eepromStream);
  eepromStream.flush(); 

  StaticJsonDocument<512> doc2;
  deserializeJson(doc2, eepromStream);
  serializeJsonPretty(doc2, Serial);

}

void loop() {}
