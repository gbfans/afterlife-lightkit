#include "Lights.h"
#include "ConfigManager.h"

#define FASTLED_ESP8266_RAW_PIN_ORDER

#define FASTLED_INTERNAL // remove annoying pragma messages
#include "FastLED.h"

template <uint8_t DATA_PIN, EOrder GRB_ORDER = GRB, int WAIT_TIME = 5>
class WS2812Controller800Khz_noflicker : public ClocklessController<DATA_PIN, C_NS(250), C_NS(625), C_NS(375), GRB_ORDER, 0, false, WAIT_TIME>
{
};

template <uint8_t DATA_PIN, EOrder GRB_ORDER>
class WS2812B_noflicker : public WS2812Controller800Khz_noflicker<DATA_PIN, GRB_ORDER>
{
};

Lights::Lights()
{
    // Do nothing
}

void Lights::init(ConfigManager configManager)
{
    _configManager = configManager;

    // TODO: Get Pins/Length from Config
    _hardwareConfiguration = _configManager.getHardwareConfiguration();
    _configuration = _configManager.getConfiguration();
    _settings = _configManager.getModeSettings(_configuration.defaultMode);

    _powercellLength = _hardwareConfiguration.Powercell.length;
    _cyclotronLength = _hardwareConfiguration.Cyclotron.length;

    FastLED.addLeds<WS2812B_noflicker, POWERCELL_PIN, GRB>(_powercellLEDS, _powercellLength);
    FastLED.addLeds<WS2812B_noflicker, CYCLOTRON_PIN, GRB>(_cyclotronLEDS, _cyclotronLength);

    FastLED.clear();
    FastLED.show();
}

void Lights::tickPowercell()
{
    if (!_checkTimer(_previousPowercellMillis, _settings.powercell.speed))
    {
        return;
    }

    CRGB ledColor(
        _settings.powercell.color.red,
        _settings.powercell.color.green,
        _settings.powercell.color.blue
    );

    /**
     * TODO: This is where we hand over to the specific animation
     *       eg:
     *          Classic:
     *              Startup
     *              Idle
     *              Firing
     *              Overheating
     *              Venting
     *              Shutdown
     *          Afterlife:
     *              Startup
     *              Idle
     *              Firing
     *              Overheating
     *              Venting
     *              Shutdown
     * We may need to create a separate class for each Mode
     */
    for (uint16_t i = 0; i < _powercellLength; i++)
    {
        /**
         * Illuminate Powercell LEDs up to and including the current LED,
         * Set the others to black.
         */
        _powercellLEDS[i] = (i <= _currentPowercell) ? ledColor : CRGB::Black;
    }

    _currentPowercell = _getNextPixel(_currentPowercell, _powercellLength, _settings.powercell.direction);

    //Reverse the direction at the top.
    if ((_currentPowercell == 0) && (_settings.powercell.direction == FORWARD))
    {
        _settings.powercell.direction = REVERSE;
        _currentPowercell = _powercellLength-1;
    }
    else if ((_currentPowercell == 0) && (_settings.powercell.direction == REVERSE))
    {
        _settings.powercell.direction = FORWARD;
    }
    
    _isDirty = true;
}

void Lights::tickCyclotron()
{
    if (!_checkTimer(_previousCyclotronMillis, _settings.cyclotron.speed))
    {
        return;
    }

    CRGB ledColor(
        _settings.cyclotron.color.red,
        _settings.cyclotron.color.green,
        _settings.cyclotron.color.blue
    );

    for (uint16_t i = 0; i < _cyclotronLength; i++)
    {
        _cyclotronLEDS[i] = (i == _currentCyclotron) ? ledColor : CRGB::Black;
    }

    _currentCyclotron = _getNextPixel(_currentCyclotron, _cyclotronLength, _settings.cyclotron.direction);

    _isDirty = true;
}

/**
 * Determine which pixel is next, depending on the direction of movement
 */
int Lights::_getNextPixel(int currentPixel, int stripLength, Directions direction)
{
    // TODO: Add support for an offset/start position
    if (direction == REVERSE) {
        currentPixel--;
        if (currentPixel < 0) {
            currentPixel = stripLength - 1;
        }

        return currentPixel;
    }

    currentPixel++;
    if (currentPixel >= stripLength) {
        // Back to start
        currentPixel = 0;
    }

    return currentPixel;
}

void Lights::update()
{
    _isDirty = false;
    tickCyclotron();
    tickPowercell();

    if (_isDirty)
    {
        // Write LEDs
        FastLED.show();
    }
}

void Lights::setMode(MODES mode)
{
    _mode = mode;
}

bool Lights::_checkTimer(unsigned long &previous, unsigned long interval)
{
    if (millis() - previous >= interval)
    {
        previous = millis();
        return true;
    }

    return false;
}
