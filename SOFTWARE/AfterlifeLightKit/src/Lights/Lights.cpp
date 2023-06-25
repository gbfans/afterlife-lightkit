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
    _configuration = _configManager.getConfiguration();

    // Initialize FastLED instances
    FastLED.addLeds<WS2812B_noflicker, POWERCELL_PIN, GRB>(_powercellLEDS, POWERCELL_LENGTH);
    FastLED.addLeds<WS2812B_noflicker, CYCLOTRON_PIN, GRB>(_cyclotronLEDS, CYCLOTRON_LENGTH);
    FastLED.addLeds<WS2812B_noflicker, NFILTER_PIN, GRB>(_nfilterLEDS, NFILTER_LENGTH);

    FastLED.clear();
    FastLED.show();

    // Get the current Colors for this Mode
    ColorConfig colorConfig = _configManager.getColorConfig(_currentMode);

    // Initialize Power Cell effects
    _powercellFX.init(
            _powercellLEDS,
            POWERCELL_LENGTH,
            CRGB(colorConfig.powercell)
    );

    // Initialize Cyclotron effects
    _cyclotronFX.init(
            _cyclotronLEDS,
            CYCLOTRON_LENGTH,
            CRGB(colorConfig.cyclotron)
    );

    // Initialize NFilter effects
    _nfilterFX.init(
            _nfilterLEDS,
            NFILTER_LENGTH,
            CRGB(colorConfig.nfilter)
    );

    // Off by default
    setState(PACK_INACTIVE);

    // Trigger an immediate update
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
//    debug(F("Mode Changed"));
//    debugln(mode);

    _currentMode = mode;

    // Set the Colors for all Strips based on the current Mode
    ColorConfig colorConfig = _configManager.getColorConfig(_currentMode);
    _powercellFX.changeLedColor(colorConfig.powercell);
    _cyclotronFX.changeLedColor(colorConfig.cyclotron);
    _nfilterFX.changeLedColor(colorConfig.nfilter);
}

void Lights::setState(PACKSTATES state)
{
    if (_currentState == state)
    {
        // Don't set the same State
        return;
    }

    _currentState = state;

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
    _powercellFX.setFromConfig(_configuration.inactive.powercell);
    _cyclotronFX.setFromConfig(_configuration.inactive.cyclotron);
    _nfilterFX.setFromConfig(_configuration.inactive.nfilter);
}

void Lights::_startup()
{
    _powercellFX.setFromConfig(_configuration.start.powercell);
    _cyclotronFX.setFromConfig(_configuration.start.cyclotron);
    _nfilterFX.setFromConfig(_configuration.start.nfilter);
}

void Lights::_idle()
{
    _cyclotronFX.setFromConfig(_configuration.idle.cyclotron);
    _powercellFX.setFromConfig(_configuration.idle.powercell);
    _nfilterFX.setFromConfig(_configuration.idle.nfilter);
}

void Lights::_shutdown()
{
    _cyclotronFX.setFromConfig(_configuration.shutdown.cyclotron);
    _powercellFX.setFromConfig(_configuration.shutdown.powercell);
    _nfilterFX.setFromConfig(_configuration.shutdown.nfilter);
}

void Lights::_firing()
{
    _cyclotronFX.setFromConfig(_configuration.firing.cyclotron);
    _powercellFX.setFromConfig(_configuration.firing.powercell);
    _nfilterFX.setFromConfig(_configuration.firing.nfilter);
}

void Lights::_overheating()
{
    _cyclotronFX.setFromConfig(_configuration.overheating.cyclotron);
    _powercellFX.setFromConfig(_configuration.overheating.powercell);
    _nfilterFX.setFromConfig(_configuration.overheating.nfilter);
}

void Lights::_venting()
{
    _cyclotronFX.setFromConfig(_configuration.venting.cyclotron);
    _powercellFX.setFromConfig(_configuration.venting.powercell);
    _nfilterFX.setFromConfig(_configuration.venting.nfilter);
}
