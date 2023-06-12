#ifndef ConfigManager_h
#define ConfigManager_h

#include <FS.h>
#include <LittleFS.h>
#include <EEPROM.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

#include "../Settings.h"

class ConfigManager  {
    public:
        // Constructor
        void init();
        Configuration getConfiguration();
        Settings getModeSettings(MODES mode);
        void get(String key);

    private:
        Configuration _configuration;
        void _extractStripSettings(StripSettings& config, JsonVariant settings, const char* stripId);
        LIGHT_EFFECTS _convertEffect(const char *input);
        unsigned long _hexToUnsignedLong(const char *rgb32_str_);
};

#endif
