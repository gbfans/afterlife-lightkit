#include "Lights.h"
#include "ConfigManager.h"
#include "FX.h"

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

    _configuration = _configManager.getConfiguration();
    _settings = _configManager.getModeSettings(_configuration.defaultMode);

    FastLED.addLeds<WS2812B_noflicker, POWERCELL_PIN, GRB>(_powercellLEDS, POWERCELL_LENGTH);
    FastLED.addLeds<WS2812B_noflicker, CYCLOTRON_PIN, GRB>(_cyclotronLEDS, CYCLOTRON_LENGTH);

    FastLED.clear();
    FastLED.show();

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
    _powercellFX.setEffect(SPINNING);

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
    _cyclotronFX.setEffect(CYCLING);
}

void Lights::update()
{
    _isDirty = false;

    if(_powercellFX.update()) {
        FastLED[0].showLeds(_powercellFX.updateBrightness());
        _isDirty = true;
    }

    if(_cyclotronFX.update()) {
        FastLED[1].showLeds(_cyclotronFX.updateBrightness());
        // Something updated
        _isDirty = true;        
    }

    if (_isDirty)
    {
        // Write LEDs
        //FastLED.show();
        //By not using the .show() function and instead individual .showLeds() functions, you have independant control over the brightness.
    }
}

void Lights::setMode(MODES mode)
{
    _currentMode = mode;
}

void Lights::setState(PACKSTATES state)
{
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

     Serial.print("State changed to: ");
     switch (_currentState)
     {
          case INACTIVE:
          Serial.println("inactive");
          break;
          case START:
          Serial.println("start");
          break;
          case IDLE:
          Serial.println("idle");
          break;
          case FIRING:
          Serial.println("firing");
          break;
          case OVERHEATING:
          Serial.println("overheating");
          break;
          case VENTING:
          Serial.println("venting");
          break;
          case SHUTDOWN:
          Serial.println("shutdown");
          break;
          case PARTY:
          Serial.println("party");
          break;
     }
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
