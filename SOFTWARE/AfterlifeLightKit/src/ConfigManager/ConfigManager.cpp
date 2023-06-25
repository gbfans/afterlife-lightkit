#include "ConfigManager.h"

void ConfigManager::init()
{
    /**
     * TODO: We need to check if config.json exists on the filesystem.
     * If not, we should load the 'default' config.json as encoded by the node sketch (or just store default values somehow)
     * _configuration.preset = "afterlife.json"
     *
     * Next, we look for this preset on the filesystem.
     * If not found, we revert to the default preset and use this.
     * If found, we load it and use it for the _extractStripSettings function.
     *
     *
     */
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
    StaticJsonDocument<2100> doc;
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

     // Extract Strip Colors
    _extractStripColors(_configuration.color.proton, doc["color"]["proton"]);
    _extractStripColors(_configuration.color.slime, doc["color"]["slime"]);
    _extractStripColors(_configuration.color.stasis, doc["color"]["stasis"]);
    _extractStripColors(_configuration.color.meson, doc["color"]["meson"]);

    // Extract Strip Settings for each State
    _extractStripSettings(_configuration.inactive, doc["states"]["inactive"]);
    _extractStripSettings(_configuration.start, doc["states"]["start"]);
    _extractStripSettings(_configuration.idle, doc["states"]["idle"]);
    _extractStripSettings(_configuration.firing, doc["states"]["firing"]);
    _extractStripSettings(_configuration.overheating, doc["states"]["overheating"]);
    _extractStripSettings(_configuration.venting, doc["states"]["venting"]);
    _extractStripSettings(_configuration.shutdown, doc["states"]["shutdown"]);

    // Demo: Save back?
    // EEPROM.begin(4098);

    // EepromStream eepromStream(0, 4098);
    // serializeJson(doc, eepromStream);
    // eepromStream.flush();
}

void ConfigManager::_extractStripColors(
    ColorConfig& config,
    JsonVariant colors
) {
    config.powercell = _hexToUnsignedLong(colors["powercell"].as<const char *>());
    config.cyclotron = _hexToUnsignedLong(colors["cyclotron"].as<const char *>());
    config.nfilter = _hexToUnsignedLong(colors["nfilter"].as<const char *>());
}

void ConfigManager::_extractStripSettings(
    StripSettings& config,
    JsonVariant states
) {
    /**
     * TODO: We need to be checking if these JsonVariant elements actually exist (eg if invalid JSON),
     *       or else the entire device will crash instantly.
     */

    config.powercell = _convertConfigurationToStateConfig(states["powercell"]);
    config.cyclotron = _convertConfigurationToStateConfig(states["cyclotron"]);
    config.nfilter = _convertConfigurationToStateConfig(states["nfilter"]);
}

unsigned long ConfigManager::_hexToUnsignedLong(const char *rgb32_str_)
{
    return strtoul(rgb32_str_, 0, 16);
}

StateConfig ConfigManager::_convertConfigurationToStateConfig(JsonVariant state)
{
    StateConfig config;

    config.effect = _convertEffect(state["effect"].as<const char *>());
    config.startSpeed = state["startSpeed"].as<int>();
    config.endSpeed = state["endSpeed"].as<int>();
    config.duration = state["duration"].as<int>();
    config.reverse = state["reverse"].as<bool>();
    config.reset = state["reset"].as<bool>();

    return config;
}

/**
 * TODO: This needs to be refactored to something cleaner/simpler
 * @param input
 * @return LIGHT_EFFECTS
 */
LIGHT_EFFECTS ConfigManager::_convertEffect(const char *input)
{
//    debug(F("Effect: "));
//    debugln(input);
    if (strcmp(input, STR_EFFECT_OFF) == 0) {
        return EFFECT_OFF;
    }

    if (strcmp(input, STR_EFFECT_ALL_ON) == 0) {
        return EFFECT_ALL_ON;
    }

    if (strcmp(input, STR_EFFECT_CYCLING) == 0) {
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
ColorConfig ConfigManager::getColorConfig(MODES mode)
{
    switch (mode)
    {
        case MODE_SLIME:
            return _configuration.color.slime;
            break;
        case MODE_STASIS:
            return _configuration.color.stasis;
            break;
        case MODE_MESON:
            return _configuration.color.meson;
            break;
        default:
        case MODE_PROTON:
            return _configuration.color.proton;
            break;
    }
}

void ConfigManager::get(String key)
{
    // TODO: Load one setting?
}

// // TODO: Save settings
// void ConfigManager::save()
// {

// }
