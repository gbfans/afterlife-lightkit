#include <FS.h>
#include <LittleFS.h>
#include <EEPROM.h>
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

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
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }

    File configFile = LittleFS.open("/config.json", "r");

    if (!configFile)
    {
        Serial.println("Opening config file failed.");
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
            Serial.println(F("Failed to read file, using default configuration"));
        }
    }

    configFile.close();

    /**
     * Output JSON Configuration as read for debugging.
     */
     serializeJsonPretty(doc, Serial);

    /**
     * Do we need to specify direction?
     */

    _configuration.state.startup.powercell_pattern = doc["state"]["startup"]["powercell_pattern"] | "";
    _configuration.state.startup.powercell_speed = doc["state"]["startup"]["powercell_speed"] | "";
    _configuration.state.startup.powercell_brightness = doc["state"]["startup"]["powercell_brightness"] | "";
    _configuration.state.startup.cyclotron_pattern = doc["state"]["startup"]["cyclotron_pattern"] | "";
    _configuration.state.startup.cyclotron_speed = doc["state"]["startup"]["cyclotron_speed"] | "";
    _configuration.state.startup.cyclotron_duration = doc["state"]["startup"]["cyclotron_duration"] | "";
    _configuration.state.startup.vent_pattern = doc["state"]["startup"]["vent_pattern"] | "";
    _configuration.state.startup.vent_speed = doc["state"]["startup"]["vent_speed"] | "";
    _configuration.state.startup.vent_brightness = doc["state"]["startup"]["vent_brightness"] | "";
    _configuration.state.startup.duration = doc["state"]["startup"]["duration"] | "";
    _configuration.state.startup.next_state = doc["state"]["startup"]["next_state"] | "";
    _configuration.state.idle.powercell_pattern = doc["state"]["idle"]["powercell_pattern"] | "";
    _configuration.state.idle.powercell_speed = doc["state"]["idle"]["powercell_speed"] | "";
    _configuration.state.idle.powercell_brightness = doc["state"]["idle"]["powercell_brightness"] | "";
    _configuration.state.idle.cyclotron_pattern = doc["state"]["idle"]["cyclotron_pattern"] | "";
    _configuration.state.idle.cyclotron_speed = doc["state"]["idle"]["cyclotron_speed"] | "";
    _configuration.state.idle.cyclotron_duration = doc["state"]["idle"]["cyclotron_duration"] | "";
    _configuration.state.idle.vent_pattern = doc["state"]["idle"]["vent_pattern"] | "";
    _configuration.state.idle.vent_speed = doc["state"]["idle"]["vent_speed"] | "";
    _configuration.state.idle.vent_brightness = doc["state"]["idle"]["vent_brightness"] | "";
    _configuration.state.idle.duration = doc["state"]["idle"]["duration"] | "";
    _configuration.state.idle.next_state = doc["state"]["idle"]["next_state"] | "";
    _configuration.state.firing.powercell_pattern = doc["state"]["firing"]["powercell_pattern"] | "";
    _configuration.state.firing.powercell_speed = doc["state"]["firing"]["powercell_speed"] | "";
    _configuration.state.firing.powercell_brightness = doc["state"]["firing"]["powercell_brightness"] | "";
    _configuration.state.firing.cyclotron_pattern = doc["state"]["firing"]["cyclotron_pattern"] | "";
    _configuration.state.firing.cyclotron_speed = doc["state"]["firing"]["cyclotron_speed"] | "";
    _configuration.state.firing.cyclotron_duration = doc["state"]["firing"]["cyclotron_duration"] | "";
    _configuration.state.firing.vent_pattern = doc["state"]["firing"]["vent_pattern"] | "";
    _configuration.state.firing.vent_speed = doc["state"]["firing"]["vent_speed"] | "";
    _configuration.state.firing.vent_brightness = doc["state"]["firing"]["vent_brightness"] | "";
    _configuration.state.firing.duration = doc["state"]["firing"]["duration"] | "";
    _configuration.state.firing.next_state = doc["state"]["firing"]["next_state"] | "";
    _configuration.state.venting.powercell_pattern = doc["state"]["venting"]["powercell_pattern"] | "";
    _configuration.state.venting.powercell_speed = doc["state"]["venting"]["powercell_speed"] | "";
    _configuration.state.venting.powercell_brightness = doc["state"]["venting"]["powercell_brightness"] | "";
    _configuration.state.venting.cyclotron_pattern = doc["state"]["venting"]["cyclotron_pattern"] | "";
    _configuration.state.venting.cyclotron_speed = doc["state"]["venting"]["cyclotron_speed"] | "";
    _configuration.state.venting.cyclotron_duration = doc["state"]["venting"]["cyclotron_duration"] | "";
    _configuration.state.venting.vent_pattern = doc["state"]["venting"]["vent_pattern"] | "";
    _configuration.state.venting.vent_speed = doc["state"]["venting"]["vent_speed"] | "";
    _configuration.state.venting.vent_brightness = doc["state"]["venting"]["vent_brightness"] | "";
    _configuration.state.venting.duration = doc["state"]["venting"]["duration"] | "";
    _configuration.state.venting.next_state = doc["state"]["venting"]["next_state"] | "";
    _configuration.state.restart.powercell_pattern = doc["state"]["restart"]["powercell_pattern"] | "";
    _configuration.state.restart.powercell_speed = doc["state"]["restart"]["powercell_speed"] | "";
    _configuration.state.restart.powercell_brightness = doc["state"]["restart"]["powercell_brightness"] | "";
    _configuration.state.restart.cyclotron_pattern = doc["state"]["restart"]["cyclotron_pattern"] | "";
    _configuration.state.restart.cyclotron_speed = doc["state"]["restart"]["cyclotron_speed"] | "";
    _configuration.state.restart.cyclotron_duration = doc["state"]["restart"]["cyclotron_duration"] | "";
    _configuration.state.restart.vent_pattern = doc["state"]["restart"]["vent_pattern"] | "";
    _configuration.state.restart.vent_speed = doc["state"]["restart"]["vent_speed"] | "";
    _configuration.state.restart.vent_brightness = doc["state"]["restart"]["vent_brightness"] | "";
    _configuration.state.restart.duration = doc["state"]["restart"]["duration"] | "";
    _configuration.state.restart.next_state = doc["state"]["restart"]["next_state"] | "";
    _configuration.state.shutdown.powercell_pattern = doc["state"]["shutdown"]["powercell_pattern"] | "";
    _configuration.state.shutdown.powercell_speed = doc["state"]["shutdown"]["powercell_speed"] | "";
    _configuration.state.shutdown.powercell_brightness = doc["state"]["shutdown"]["powercell_brightness"] | "";
    _configuration.state.shutdown.cyclotron_pattern = doc["state"]["shutdown"]["cyclotron_pattern"] | "";
    _configuration.state.shutdown.cyclotron_speed = doc["state"]["shutdown"]["cyclotron_speed"] | "";
    _configuration.state.shutdown.cyclotron_duration = doc["state"]["shutdown"]["cyclotron_duration"] | "";
    _configuration.state.shutdown.vent_pattern = doc["state"]["shutdown"]["vent_pattern"] | "";
    _configuration.state.shutdown.vent_speed = doc["state"]["shutdown"]["vent_speed"] | "";
    _configuration.state.shutdown.vent_brightness = doc["state"]["shutdown"]["vent_brightness"] | "";
    _configuration.state.shutdown.duration = doc["state"]["shutdown"]["duration"] | "";
    _configuration.state.shutdown.next_state = doc["state"]["shutdown"]["next_state"] | "";
    _configuration.state.inactive.powercell_pattern = doc["state"]["inactive"]["powercell_pattern"] | "";
    _configuration.state.inactive.powercell_speed = doc["state"]["inactive"]["powercell_speed"] | "";
    _configuration.state.inactive.powercell_brightness = doc["state"]["inactive"]["powercell_brightness"] | "";
    _configuration.state.inactive.cyclotron_pattern = doc["state"]["inactive"]["cyclotron_pattern"] | "";
    _configuration.state.inactive.cyclotron_speed = doc["state"]["inactive"]["cyclotron_speed"] | "";
    _configuration.state.inactive.cyclotron_duration = doc["state"]["inactive"]["cyclotron_duration"] | "";
    _configuration.state.inactive.vent_pattern = doc["state"]["inactive"]["vent_pattern"] | "";
    _configuration.state.inactive.vent_speed = doc["state"]["inactive"]["vent_speed"] | "";
    _configuration.state.inactive.vent_brightness = doc["state"]["inactive"]["vent_brightness"] | "";
    _configuration.state.inactive.duration = doc["state"]["inactive"]["duration"] | "";
    _configuration.state.inactive.next_state = doc["state"]["inactive"]["next_state"] | "";

    /*
     * I'm not sure party needs to be defined here. Party is a mode, not a state and it should follow the state
     * configuration regardless of what state the pack is in. -AJQ Aug 20, 2022.
     * 
     * _configuration.state.party.powercell_pattern = doc["state"]["party"]["powercell_pattern"] | "";
     * _configuration.state.party.powercell_speed = doc["state"]["party"]["powercell_speed"] | "";
     * _configuration.state.party.powercell_brightness = doc["state"]["party"]["powercell_brightness"] | "";
     * _configuration.state.party.cyclotron_pattern = doc["state"]["party"]["cyclotron_pattern"] | "";
     * _configuration.state.party.cyclotron_speed = doc["state"]["party"]["cyclotron_speed"] | "";
     * _configuration.state.party.cyclotron_duration = doc["state"]["party"]["cyclotron_duration"] | "";
     * _configuration.state.party.vent_pattern = doc["state"]["party"]["vent_pattern"] | "";
     * _configuration.state.party.vent_speed = doc["state"]["party"]["vent_speed"] | "";
     * _configuration.state.party.vent_brightness = doc["state"]["party"]["vent_brightness"] | "";
     * _configuration.state.party.duration = doc["state"]["party"]["duration"] | "";
     * _configuration.state.party.next_state = doc["state"]["party"]["next_state"] | "";
     */
     
    _configuration.mode.classic.powercell_color = doc["mode"]["classic"]["powercell_color"] | CRGB::Blue;
    _configuration.mode.classic.powercell_pattern = doc["mode"]["classic"]["powercell_pattern"] | "scroll";
    _configuration.mode.classic.cyclotron_color = doc["mode"]["classic"]["cyclotron_color"] | CRGB::Red;
    _configuration.mode.classic.cyclotron_pattern = doc["mode"]["classic"]["cyclotron_pattern"] | "cycling";
    _configuration.mode.classic.vent_color = doc["mode"]["classic"]["vent_color"] | CRGB::White;
    _configuration.mode.classic.vent_pattern = doc["mode"]["classic"]["vent_pattern"] | "strobe";
    
    _configuration.mode.slime.powercell_color = doc["mode"]["slime"]["powercell_color"] | CRGB::Green;
    _configuration.mode.slime.powercell_pattern = doc["mode"]["slime"]["powercell_pattern"] | "scroll";
    _configuration.mode.slime.cyclotron_color = doc["mode"]["slime"]["cyclotron_color"] | CRGB::Green;
    _configuration.mode.slime.cyclotron_pattern = doc["mode"]["slime"]["cyclotron_pattern"] | "cycling";
    _configuration.mode.slime.vent_color = doc["mode"]["slime"]["vent_color"] | CRGB::White;
    _configuration.mode.slime.vent_pattern = doc["mode"]["slime"]["vent_pattern"] | "strobe";
    
    _configuration.mode.meson.powercell_color = doc["mode"]["meson"]["powercell_color"] | CRGB::Orange;
    _configuration.mode.meson.powercell_pattern = doc["mode"]["meson"]["powercell_pattern"] | "scroll";
    _configuration.mode.meson.cyclotron_color = doc["mode"]["meson"]["cyclotron_color"] | CRGB::Orange;
    _configuration.mode.meson.cyclotron_pattern = doc["mode"]["meson"]["cyclotron_pattern"] | "cycling";
    _configuration.mode.meson.vent_color = doc["mode"]["meson"]["vent_color"] | CRGB::White;
    _configuration.mode.meson.vent_pattern = doc["mode"]["meson"]["vent_pattern"] | "strobe";
    
    _configuration.mode.stasis.powercell_color = doc["mode"]["stasis"]["powercell_color"] | CRGB::Blue;
    _configuration.mode.stasis.powercell_pattern = doc["mode"]["stasis"]["powercell_pattern"] | "scroll";
    _configuration.mode.stasis.cyclotron_color = doc["mode"]["stasis"]["cyclotron_color"] | CRGB::Blue;
    _configuration.mode.stasis.cyclotron_pattern = doc["mode"]["stasis"]["cyclotron_pattern"] | "cycling";
    _configuration.mode.stasis.vent_color = doc["mode"]["stasis"]["vent_color"] | CRGB::White;
    _configuration.mode.stasis.vent_pattern = doc["mode"]["stasis"]["vent_pattern"] | "strobe";
    
    _configuration.mode.party.powercell_color = doc["mode"]["party"]["powercell_color"] | "rainbow";
    _configuration.mode.party.powercell_pattern = doc["mode"]["party"]["powercell_pattern"] | "scroll";
    _configuration.mode.party.cyclotron_color = doc["mode"]["party"]["cyclotron_color"] | "rainbow";
    _configuration.mode.party.cyclotron_pattern = doc["mode"]["party"]["cyclotron_pattern"] | "cycling";
    _configuration.mode.party.vent_color = doc["mode"]["party"]["vent_color"] | "rainbow";
    _configuration.mode.party.vent_pattern = doc["mode"]["party"]["vent_pattern"] | "strobe";
    
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
 * 
 * DO WE NEED THIS? -AJQ Aug 20, 2022
 * 
 */
Settings ConfigManager::getModeSettings(MODES mode)
{
    Settings _settings;
    switch (mode)
    {
        case CLASSIC:
            _settings = _configuration.afterlifeSettings;
            break;
        //case CLASSIC:
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
