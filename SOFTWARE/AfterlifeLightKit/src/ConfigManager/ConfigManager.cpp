#include "ConfigManager.h"

void ConfigManager::init()
{
    if (!LittleFS.begin())
    {
        /*
         * TODO: If this error occurs, we should still
         * continue executing, but the Wifi Configuration feature
         * will be unavailable. Return a hard-coded HTML error page?
         */
        debugln(F("An Error has occurred while mounting LittleFS"));
        return;
    }

    File configFile = LittleFS.open("/config.json", "r");

    if (!configFile)
    {
        debugln(F("Opening config file failed."));
        return;
    }

    // Allocate a temporary JsonDocument
    // Don't forget to change the capacity to match your requirements.
    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<1024> doc;
    if (configFile.available())
    {
        DeserializationError error = deserializeJson(doc, configFile);
        if (error) {
            debugln(F("Failed to read file, using default configuration"));
        }
    }

    configFile.close();

    /**
     * Output JSON Configuration as read for debugging.
     */
     serializeJsonPretty(doc, Serial);

    /**
     * TODO: How do we abstract this out?
     */
    //_configuration.classicSettings.powercellSpeed = doc["modes"]["classic"]["powercell"]["speed"] | 1000;

    // Power Cell
    _configuration.afterlifeSettings.powercell.speed = doc["modes"]["afterlife"]["powercell"]["speed"] | 50;
    _configuration.afterlifeSettings.powercell.color.red = doc["modes"]["afterlife"]["powercell"]["color"]["red"];
    _configuration.afterlifeSettings.powercell.color.green = doc["modes"]["afterlife"]["powercell"]["color"]["green"];
    _configuration.afterlifeSettings.powercell.color.blue = doc["modes"]["afterlife"]["powercell"]["color"]["blue"];

    // Cyclotron
    _configuration.afterlifeSettings.cyclotron.speed = doc["modes"]["afterlife"]["cyclotron"]["speed"] | 5;
    DIRECTIONS cyclotronDirection = LIGHTS_FORWARD;
    if (doc["modes"]["afterlife"]["cyclotron"]["direction"] == "reverse") {
        cyclotronDirection = LIGHTS_REVERSE;
    }
    _configuration.afterlifeSettings.cyclotron.direction = cyclotronDirection;

    _configuration.afterlifeSettings.cyclotron.color.red = doc["modes"]["afterlife"]["cyclotron"]["color"]["red"];
    _configuration.afterlifeSettings.cyclotron.color.green = doc["modes"]["afterlife"]["cyclotron"]["color"]["green"];
    _configuration.afterlifeSettings.cyclotron.color.blue = doc["modes"]["afterlife"]["cyclotron"]["color"]["blue"];

    // N-Filter
    _configuration.afterlifeSettings.nfilter.speed = doc["modes"]["afterlife"]["nfilter"]["speed"] | 500;
    _configuration.afterlifeSettings.nfilter.color.red = doc["modes"]["afterlife"]["nfilter"]["color"]["red"];
    _configuration.afterlifeSettings.nfilter.color.green = doc["modes"]["afterlife"]["nfilter"]["color"]["green"];
    _configuration.afterlifeSettings.nfilter.color.blue = doc["modes"]["afterlife"]["nfilter"]["color"]["blue"];

    // Demo: Save back?
    // EEPROM.begin(4098);

    // EepromStream eepromStream(0, 4098);
    // serializeJson(doc, eepromStream);
    // eepromStream.flush();
}

Configuration ConfigManager::getConfiguration()
{
    return _configuration;
}

/**
 * Load all settings for Mode
 */
Settings ConfigManager::getModeSettings(MODES mode)
{
    Settings _settings;
    switch (mode)
    {
        case AFTERLIFE:
            _settings = _configuration.afterlifeSettings;
            break;
        case CLASSIC:
        default:
            _settings = _configuration.classicSettings;
            break;
    }

    return _settings;
}

void ConfigManager::get(String key)
{
    // TODO: Load one setting?
}

// // TODO: Save settings
// void ConfigManager::save()
// {

// }
