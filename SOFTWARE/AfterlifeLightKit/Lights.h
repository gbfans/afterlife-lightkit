#define FASTLED_INTERNAL //remove annoying pragma messages
#include "FastLED.h"

#include "ConfigManager.h"
#include "FX.h"

#ifndef Lights_h
#define Lights_h

class Lights {
    public:
        // Constructor
        Lights();

        // Public Methods
        void init(ConfigManager configManager);
        void setMode(MODES mode); // Both Powercell/Cyclotron are in same mode
        void update();
        void setState(PACKSTATES state);

        // TODO: Remove this test method
        void testChangeCyclotronSpeed(unsigned char newSpeed, int delay);
        void testChangeCyclotronBrightness(unsigned char newSpeed, int delay);
    private:
        // Private Properties
        bool _isDirty = false;

        ConfigManager _configManager;
        Configuration _configuration;
        Settings _settings;
        MODES _currentMode;
        PACKSTATES _currentState;
        FX _powercellFX;
        FX _cyclotronFX;

        CRGB _powercellLEDS[POWERCELL_LENGTH];
        CRGB _cyclotronLEDS[CYCLOTRON_LENGTH];
};

#endif
