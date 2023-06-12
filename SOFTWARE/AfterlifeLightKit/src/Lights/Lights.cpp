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
    debugln(F("Initializing Lights"));
    _configManager.init();

    _settings = _configManager.getModeSettings(MODE_PROTON); // TODO: Update this to work dynamically

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
            CRGB(_settings.powercell.color)
    );

    // Initialize Cyclotron effects
    _cyclotronFX.init(
            _cyclotronLEDS,
            CYCLOTRON_LENGTH,
            CRGB(_settings.cyclotron.color)
    );

    // Initialize NFilter effects
    _nfilterFX.init(
            _nfilterLEDS,
            NFILTER_LENGTH,
            CRGB(_settings.nfilter.color)
    );

    // Off by default
    setState(PACK_INACTIVE);
    update(true);
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

    //debug("Lights State changed to: ");
    switch (_currentState)
    {
        case PACK_INACTIVE:
            _inactive();
            //debugln("inactive");
            break;
        case PACK_START:
            _startup();
            //debugln("start");
            break;
        case PACK_IDLE:
            _idle();
            //debugln("idle");
            break;
        case PACK_FIRING:
            _firing();
            //debugln("firing");
            break;
        case PACK_OVERHEATING:
            _overheating();
            //debugln("overheating");
            break;
        case PACK_VENTING:
            _venting();
            //debugln("venting");
            break;
        case PACK_SHUTDOWN:
            _shutdown();
            //debugln("shutdown");
            break;
    }
}

void Lights::_inactive()
{
    // Switch off all lights
    _stop();
}

void Lights::_stop()
{
    // Switch off all lights
    _cyclotronFX.stop();
    _powercellFX.stop();
    _nfilterFX.stop();
}

void Lights::_startup()
{
    // Switch off all lights (they should be off already during startup)
    _cyclotronFX.setEffect(EFFECT_CYCLING, true);
    // Ramp up to full idle speed over 3 seconds
    // Start slow at speed 100, increase to speed 10 over 3 seconds
    _cyclotronFX.changeSpeedFrom(100, 10, 3000, QUADRATIC_INOUT);

    // Special PowerCell Startup animation
    // Start at speed 25, increase to 5 over 3 seconds
    _powercellFX.changeSpeedFrom(25, 5, 3000, QUADRATIC_INOUT);
    _powercellFX.setEffect(EFFECT_TETRIS, true);
    _powercellFX.setReverse();

    _nfilterFX.setEffect(EFFECT_OFF);
}

void Lights::_idle()
{
    // Switch off NFilter (if on)
    _nfilterFX.stop();

    // NOTE: We should already be at this speed after Startup, but subsequent effect changes (firing etc) will return
    // here, so this ensures a gradual ramp up/down from our previous speed.
    _cyclotronFX.setEffect(EFFECT_CYCLING);
    _cyclotronFX.changeSpeed(10, 1000, QUADRATIC_INOUT);

    // Set to SPINNING for standard mode
    // Set to CYCLING for that one Afterlife scene where Ray was knocked over and the PowerCell was misconfigured
    //_powercellFX.setEffect(EFFECT_SPINNING, true);
    _powercellFX.setEffect(_settings.powercell.idle.effect, true);
    _powercellFX.changeSpeed(50, 0, QUADRATIC_INOUT); // instant speed change
}

void Lights::_shutdown()
{
    // Switch off NFilter (if on)
    _nfilterFX.stop();

    // Afterlife: Slow down the Cyclotron over 3 seconds
    _cyclotronFX.setEffect(EFFECT_CYCLING); // in case we were non-idle (firing/venting/etc) at Shutdown
    _cyclotronFX.changeSpeed(100, 3000, LINEAR);

    // Fade out PowerCell over 2-3 seconds
    //_powercellFX.allOn();
    _powercellFX.setEffect(EFFECT_DESCEND, true);
    _powercellFX.changeSpeed(round(2000/POWERCELL_LENGTH));
    _powercellFX.setReverse();
}

void Lights::_firing()
{
    // Switch off NFilter (if on)
    _nfilterFX.stop();

    // Make Cyclotron faster (3 instead of 5 delay), take 2 seconds to speed up
    _cyclotronFX.setEffect(EFFECT_CYCLING);
    _cyclotronFX.changeSpeed(3, 2000, QUADRATIC_INOUT);

    // Make Power Cell faster (25 instead of 50 delay), take 2 seconds to speed up
    _powercellFX.setEffect(EFFECT_SPINNING);
    _powercellFX.changeSpeed(25, 2000, QUADRATIC_INOUT);
}

void Lights::_overheating()
{
    // Switch off NFilter (if on)
    _nfilterFX.stop();

    // Faster Cyclotron
    _cyclotronFX.setEffect(EFFECT_CYCLING);
    _cyclotronFX.changeSpeed(2, 3000, QUADRATIC_INOUT);

    // Make Power Cell alternate
    _powercellFX.setEffect(EFFECT_ALTERNATE, true);
    _powercellFX.changeSpeed(200);

    // TODO: We won't actually switch the nfilter on during overheating, it will be during venting
    _nfilterFX.setEffect(EFFECT_ALL_ON);
}

void Lights::_venting()
{
    // To be implemented

    // We don't need to do anything to the powercell/cyclotron
    // so they should continue doing what they're doing

    _nfilterFX.setEffect(EFFECT_ALL_ON);
}
