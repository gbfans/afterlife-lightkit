#ifndef Lights_h

// Hopefully prevent jitter as per https://github.com/jasoncoon/esp8266-fastled-webserver/issues/85#issuecomment-618656354
//#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_INTERRUPT_RETRY_COUNT 0

//remove annoying pragma messages
#define FASTLED_INTERNAL
#define FASTLED_ESP8266_RAW_PIN_ORDER

#include "FastLED.h"

#include "../ConfigManager/ConfigManager.h"
#include "../FX/FX.h"

#define Lights_h

class Lights {
    public:
        // Constructor
        Lights();

        // Public Methods
        void init();
        void setMode(MODES mode); // Both Powercell/Cyclotron are in same mode
        void update(bool force = false);
        void setState(PACKSTATES state);
    private:
        // Private Properties
        ConfigManager _configManager;
        Settings _settings;
        MODES _currentMode;
        PACKSTATES _currentState;
        FX _powercellFX;
        FX _cyclotronFX;
        FX _nfilterFX;

        CRGB _powercellLEDS[POWERCELL_LENGTH];
        CRGB _cyclotronLEDS[CYCLOTRON_LENGTH];
        CRGB _nfilterLEDS[NFILTER_LENGTH];

        void _stop();
        void _inactive();
        void _startup();
        void _idle();
        void _firing();
        void _overheating();
        void _venting();
        void _shutdown();
};

#endif
