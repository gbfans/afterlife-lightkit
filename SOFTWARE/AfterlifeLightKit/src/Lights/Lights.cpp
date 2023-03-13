#include "Lights.h"
#include "../ConfigManager/ConfigManager.h"
#include "../FX/FX.h"

#define FASTLED_ESP8266_RAW_PIN_ORDER

#define FASTLED_INTERNAL // remove annoying pragma messages

#include "FastLED.h"

template<uint8_t DATA_PIN, EOrder GRB_ORDER = GRB, int WAIT_TIME = 5>
class WS2812Controller800Khz_noflicker : public ClocklessController<DATA_PIN, C_NS(250), C_NS(625), C_NS(
        375), GRB_ORDER, 0, false, WAIT_TIME>
{
};

template<uint8_t DATA_PIN, EOrder GRB_ORDER>
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

    _configuration = _configManager.getConfiguration();
    _settings = _configManager.getModeSettings(_configuration.defaultMode);

    // Initialize FastLED instances
    FastLED.addLeds<WS2812B_noflicker, POWERCELL_PIN, GRB>(_powercellLEDS, POWERCELL_LENGTH);
    FastLED.addLeds<WS2812B_noflicker, CYCLOTRON_PIN, GRB>(_cyclotronLEDS, CYCLOTRON_LENGTH);
    FastLED.addLeds<WS2812B_noflicker, NFILTER_PIN, GRB>(_nfilterLEDS, NFILTER_LENGTH);

    FastLED.clear();
    FastLED.show();

    // Initialize Power Cell effects
    _powercellFX.init(
            _powercellLEDS,
            POWERCELL_LENGTH,
            CRGB(
                    _settings.powercell.color.red,
                    _settings.powercell.color.green,
                    _settings.powercell.color.blue
            ),
            _settings.powercell.direction,
            _settings.powercell.speed
    );

    // Initialize Cyclotron effects
    _cyclotronFX.init(
            _cyclotronLEDS,
            CYCLOTRON_LENGTH,
            CRGB(
                    _settings.cyclotron.color.red,
                    _settings.cyclotron.color.green,
                    _settings.cyclotron.color.blue
            ),
            _settings.cyclotron.direction,
            _settings.cyclotron.speed
    );

    // Initialize NFilter effects
    _nfilterFX.init(
            _nfilterLEDS,
            NFILTER_LENGTH,
            CRGB(
                    _settings.nfilter.color.red,
                    _settings.nfilter.color.green,
                    _settings.nfilter.color.blue
            ),
            _settings.nfilter.direction,
            _settings.nfilter.speed
    );
}

void Lights::update(bool force)
{
    /**
     * NOTE: By not using the FastLED.show() function and instead individual .showLeds() functions,
     * we have independent control over the brightness.
     */
    if (_powercellFX.update() || force)
    {
        FastLED[0].showLeds(_powercellFX.updateBrightness());
    }

    if (_cyclotronFX.update() || force)
    {
        FastLED[1].showLeds(_cyclotronFX.updateBrightness());
    }

    if (_nfilterFX.update() || force)
    {
        FastLED[2].showLeds(_nfilterFX.updateBrightness());
    }
}

void Lights::setMode(MODES mode)
{
    if (_currentMode == mode) {
        return;
    }

    _currentMode = mode;
}

void Lights::setState(PACKSTATES state)
{
    if (_currentState == state)
    {
        // Don't set the same State
        return;
    }

    _currentState = state;
    /**
     * TODO: Changing the State or the Mode
     *       will need to change the Cyclotron/Powercell FX animations.
     *       e.g. If current State is 'IDLE' and new State is 'FIRING',
     *            we need to access the correct FX configuration based on
     *            the current mode, and then apply this to the applicable
     *            FX instances. So this could require:
     *            - Change Speed (eg 'Afterlife Firing Speed: 10ms')
     *            - Change Color (eg 'Afterlife Firing Color: Blue')
     *            - Change Effect (eg 'Afterlife Firing Effect: Spinning')
     *            - Change Direction (eg 'Afterlife Firing Direction: Reverse')
     *            All of these are configurable by the user.
     */

    Serial.print("Lights State changed to: ");
    switch (_currentState)
    {
        case INACTIVE:
            _inactive();
            Serial.println("inactive");
            break;
        case START:
            _startup();
            Serial.println("start");
            break;
        case IDLE:
            _idle();
            Serial.println("idle");
            break;
        case FIRING:
            _firing();
            Serial.println("firing");
            break;
        case OVERHEATING:
            _overheating();
            Serial.println("overheating");
            break;
        case VENTING:
            _venting();
            Serial.println("venting");
            break;
        case SHUTDOWN:
            _shutdown();
            Serial.println("shutdown");
            break;
        case PARTY:
            _party();
            Serial.println("party");
            break;
    }
}

void Lights::_inactive()
{
    // Switch off all lights
    _cyclotronFX.stop();
    _powercellFX.stop();
    _nfilterFX.stop();
}

void Lights::_startup()
{
    // Switch off all lights
    _cyclotronFX.stop();
    _powercellFX.stop();
    _nfilterFX.stop();

    // Ramp up Cyclotron

    _powercellFX.setEffect()
}

void Lights::_idle()
{
    _cyclotronFX.setEffect(CYCLING);
    _cyclotronFX.reset();

    _powercellFX.setEffect(SPINNING);
    _powercellFX.reset();

    _nfilterFX.stop();
    _nfilterFX.reset();
}

void Lights::_shutdown()
{
    // Set PowerCell/Cyclotron to all-on
    _cyclotronFX.allOn();
    _cyclotronFX.update(true);
    _powercellFX.allOn();
    _powercellFX.update(true);
    // Write immediately

    // Fade out PowerCell/Cyclotron over 2-3 seconds
    _cyclotronFX.setEffect(FADEOUT);
    _powercellFX.setEffect(FADEOUT);

    _powercellFX.changeSpeed(5, 1500, QUADRATIC_INOUT);
    _cyclotronFX.changeSpeed(5, 3000, QUADRATIC_INOUT);

    // Switch off NFilter (if on)
    _nfilterFX.stop();
}

void Lights::_firing()
{
    // Make Cyclotron faster (3 instead of 5 delay), take 2 seconds to speed up
    _cyclotronFX.setEffect(CYCLING);
    _cyclotronFX.changeSpeed(3, 2000, QUADRATIC_INOUT);

    // Make Power Cell faster (25 instead of 50 delay), take 2 seconds to speed up
    _powercellFX.setEffect(SPINNING);
    _powercellFX.changeSpeed(25, 2000, QUADRATIC_INOUT);

    _nfilterFX.stop();
}

void Lights::_overheating()
{
    // Blink Cyclotron
    _cyclotronFX.setEffect(BLINKING);
    _cyclotronFX.changeSpeed(500, 0, NONE);

    // Make Power Cell alternate
    _powercellFX.setEffect(ALTERNATE);
    _powercellFX.changeSpeed(100, 0, NONE);

    // Blink the N-Filter
    _nfilterFX.setEffect(BLINKING);
}

void Lights::_venting()
{
    // To be implemented
}

void Lights::_party()
{
    // To be implemented
}

/**
 * TODO: Remove this as setState and setMode will handle changing
 *       these values based on Configuration.
 * This demo function changes the speed of the Cyclotron.
 */
void Lights::testChangeCyclotronSpeed(unsigned char newSpeed, int delay)
{
    _cyclotronFX.changeSpeed(newSpeed, delay, QUADRATIC_INOUT);
}

void Lights::testChangeCyclotronBrightness(unsigned char newBrightness, int delay)
{
    _cyclotronFX.changeBrightness(newBrightness, delay, CUBIC_OUT);
}
