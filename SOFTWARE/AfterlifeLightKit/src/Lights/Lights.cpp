#include "Lights.h"

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

void Lights::init()
{
    _configManager.init();

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

    //Serial.print("Lights State changed to: ");
    switch (_currentState)
    {
        case INACTIVE:
            _inactive();
            //Serial.println("inactive");
            break;
        case START:
            _startup();
            //Serial.println("start");
            break;
        case IDLE:
            _idle();
            //Serial.println("idle");
            break;
        case FIRING:
            _firing();
            //Serial.println("firing");
            break;
        case OVERHEATING:
            _overheating();
            //Serial.println("overheating");
            break;
        case VENTING:
            _venting();
            //Serial.println("venting");
            break;
        case SHUTDOWN:
            _shutdown();
            //Serial.println("shutdown");
            break;
        case PARTY:
            _party();
            //Serial.println("party");
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
    // Switch off all lights (they should be off already during startup)
    _cyclotronFX.stop();
    _powercellFX.stop();
    _nfilterFX.stop();

    _cyclotronFX.changeBrightness(255);

    _cyclotronFX.setEffect(CYCLING, true);
    // Ramp up to full idle speed over 3 seconds
    _cyclotronFX.changeSpeed(100); // Start slow
    _cyclotronFX.changeSpeed(10, 3000, QUADRATIC_INOUT);

    // Special PowerCell Startup animation
    _powercellFX.changeSpeed(25);
    _powercellFX.changeSpeed(5, 3000, QUADRATIC_INOUT);
    _powercellFX.setEffect(TETRIS, true);
    _powercellFX.setReverse();
}

void Lights::_idle()
{
    // NOTE: We should already be at this speed after Startup, but subsequent effect changes (firing etc) will return
    // here, so this ensures a gradual ramp up/down from our previous speed.
    _cyclotronFX.setEffect(CYCLING);
    _cyclotronFX.changeSpeed(10, 1000, QUADRATIC_INOUT);

    // Set to SPINNING for standard mode
    // Set to CYCLING for that one Afterlife scene where Ray was knocked over and the PowerCell was misconfigured
    _powercellFX.setEffect(SPINNING, true);
    _powercellFX.changeSpeed(50);

    _nfilterFX.stop();
    _nfilterFX.reset();
}

void Lights::_shutdown()
{
    // Afterlife: Slow down the Cyclotron over 3 seconds
    _cyclotronFX.setEffect(CYCLING); // in case we were non-idle at Shutdown
    _cyclotronFX.changeSpeed(100, 3000, LINEAR);

    // Fade out PowerCell over 2-3 seconds
    _powercellFX.allOn();
    _powercellFX.changeSpeed(round(2000/POWERCELL_LENGTH));
    _powercellFX.setEffect(DESCEND, true);
    _powercellFX.setReverse();

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
    // Make Power Cell alternate
    _powercellFX.setEffect(ALTERNATE, true);
    _powercellFX.changeSpeed(200);

    // Faster Cyclotron
    _cyclotronFX.changeSpeed(2, 3000, QUADRATIC_INOUT);

    // Illuminate the N-Filter
    //_nfilterFX.setEffect(ALL_ON);
}

void Lights::_venting()
{
    // To be implemented

    _nfilterFX.setEffect(ALL_ON);
}

void Lights::_party()
{
    // To be implemented
}
