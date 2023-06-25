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

//    Dir dir = LittleFS.openDir("/");
//    while (dir.next()) {
//        debug(dir.fileName());
//        if(dir.fileSize()) {
//            File f = dir.openFile("r");
//            debugln(f.size());
//        }
//    }

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
//
//    const char* preset = doc["preset"];
//    debug(F("Preset: "));
//    debugln(preset);
//
//    return;
    /**
     * Output JSON Configuration as read for debugging.
     */
     serializeJsonPretty(doc, Serial);

    // Power Cell
    _extractStripSettings(
            _configuration.proton.powercell,
            doc["modes"]["proton"],
            "powercell"
    );

    // Cyclotron
    _extractStripSettings(
            _configuration.proton.cyclotron,
            doc["modes"]["proton"],
            "cyclotron"
    );

    // N-Filter
    _extractStripSettings(
            _configuration.proton.nfilter,
            doc["modes"]["proton"],
            "nfilter"
    );

    // Demo: Save back?
    // EEPROM.begin(4098);

    // EepromStream eepromStream(0, 4098);
    // serializeJson(doc, eepromStream);
    // eepromStream.flush();
}

void ConfigManager::_extractStripSettings(
    StripSettings& config,
    JsonVariant settings,
    const char* stripId
) {
    /**
     * TODO: We need to be checking if these JsonVariant elements actually exist (eg if invalid JSON),
     *       or else the entire device will crash instantly.
     */
    config.color = _hexToUnsignedLong(settings["color"][stripId].as<const char *>());

    config.idle.effect = _convertEffect(settings["states"]["idle"][stripId]["effect"].as<const char *>());
}

unsigned long ConfigManager::_hexToUnsignedLong(const char *rgb32_str_)
{
    return strtoul(rgb32_str_, 0, 16);
}


/**
 * TODO: This needs to be refactored to something cleaner/simpler
 * @param input
 * @return LIGHT_EFFECTS
 */
LIGHT_EFFECTS ConfigManager::_convertEffect(const char *input)
{
    if (strcmp(input, STR_EFFECT_OFF) == 0) {
        return EFFECT_OFF;
    }

    if (strcmp(input, STR_EFFECT_ALL_ON) == 0) {
        return EFFECT_ALL_ON;
    }

    if (strcmp(input, STR_EFFECT_CYCLING) == 0) {
        debug("CYCLING: ");
        debugln(input);
        return EFFECT_CYCLING;
    }

    if (strcmp(input, STR_EFFECT_SPINNING) == 0) {
        return EFFECT_SPINNING;
    }

    if (strcmp(input, STR_EFFECT_RAINBOW) == 0) {
        return EFFECT_RAINBOW;
    }

    if (strcmp(input, STR_EFFECT_RAINBOW_SCROLL) == 0) {
        return EFFECT_RAINBOW_SCROLL;
    }

    if (strcmp(input, STR_EFFECT_CYLON) == 0) {
        return EFFECT_CYLON;
    }

    if (strcmp(input, STR_EFFECT_TETRIS) == 0) {
        return EFFECT_TETRIS;
    }

    if (strcmp(input, STR_EFFECT_DESCEND) == 0) {
        return EFFECT_DESCEND;
    }

    if (strcmp(input, STR_EFFECT_ALTERNATE) == 0) {
        return EFFECT_ALTERNATE;
    }

    if (strcmp(input, STR_EFFECT_BLINKING) == 0) {
        return EFFECT_BLINKING;
    }

    // Default/fallback effect is 'off'
    return EFFECT_OFF;
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
    switch (mode)
    {
        default:
        case MODE_PROTON:
            return _configuration.proton;
            break;
        case MODE_SLIME:
            //return _configuration.
            break;
        case MODE_STASIS:
            break;
        case MODE_MESON:
            break;
    }

    return _configuration.proton;
}

void ConfigManager::get(String key)
{
    // TODO: Load one setting?
}

// // TODO: Save settings
// void ConfigManager::save()
// {

// }
