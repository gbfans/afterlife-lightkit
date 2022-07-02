/**
 *
 */
#ifndef ConfigManager_h
#define ConfigManager_h

#include <ArduinoJson.h>   // https://github.com/bblanchon/ArduinoJson
#include "Settings.h"

class ConfigManager  {
    public:
        // Constructor
        void init();
        HardwareConfiguration getHardwareConfiguration();
        Configuration getConfiguration();
        Settings getModeSettings(MODES mode);
        void get(String key);

    private:
        HardwareConfiguration _hardwareConfiguration;
        Configuration _configuration;
};

#endif
