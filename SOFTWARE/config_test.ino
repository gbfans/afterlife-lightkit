#include <Arduino.h>

#include <FS.h>
#include <LittleFS.h>
#include <EEPROM.h>
#include <StreamUtils.h>
#include <ArduinoJson.h>

void setup() {

  Serial.begin(115200);
  
  // put your setup code here, to run once:

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

  String content;
  if (configFile.available())
  {
    content = configFile.readString();
  }
  configFile.close();

  char json[content.length()];

  serializeJsonPretty(content, Serial);
  
  EEPROM.begin(4098);

  EepromStream eepromStream(0, 4098);
  serializeJson(content, eepromStream);
  eepromStream.flush();


  DynamicJsonDocument doc2(4098);
  deserializeJson(doc2, eepromStream);

}

void loop() {
  // put your main code here, to run repeatedly:

}
