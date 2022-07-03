#define FASTLED_INTERNAL //remove annoying pragma messages
#include "FastLED.h"

#include "ConfigManager.h"

#ifndef Lights_h
#define Lights_h

class Lights {
    public:
        // Constructor
        Lights();

        // Public Methods
        void init(ConfigManager configManager);
        void setMode(MODES mode);
        void update();

        void tickPowercell();
        void tickCyclotron();
    private:
        // Private Methods
        bool _checkTimer(unsigned long &previous, unsigned long interval);
        int _getNextPixel(int currentPixel, int stripLength, Directions direction);

        // Private Properties
        int _powercellLength;
        int _cyclotronLength;
        unsigned long _previousPowercellMillis = 0;
        unsigned long _previousCyclotronMillis = 0;
        int _powercellInterval;
        int _cyclotronInterval;
        int _currentPowercell = 0;
        int _currentCyclotron = 0;

        bool _isDirty = false;

        ConfigManager _configManager;
        Configuration _configuration;
        HardwareConfiguration _hardwareConfiguration;
        Settings _settings;
        MODES _mode;
        /**
         * Have to define these with 100 as we can't seem to dynamically set the strip length
         * So 20 should be more than enough for the Powercell, and 100 for the Cyclotron
         */
        CRGB _powercellLEDS[20];
        CRGB _cyclotronLEDS[100];
};

#endif


/**
 * We need TimedAction
 *
 * We need an init method which starts both light strips
 *
 * We need an update method which
 * - loops over each light strip
 * - Calls the (whatever) method which sets the LED states based on some algorith/settings
 *   - Each mode has internal methods which calculate currentLed (next LED based on direction etc) etc
 * - Writes the LEDs
 *
 * We need a setMode method to change mode
 *
 * Some way to change settings (non-Hardware) configuration on the fly without restarting strip
 *
 * Do we need Enable/Disable methods?
 */

/**
 * PowerCell:
 *   Hardware:
 *     PIN
 *     LENGTH (#LEDS)
 *     TYPE (WS2812B etc)
 *     COLORTYPE RGB/GRB
 *   Settings:
 *     Color (blue, white, RGB)
 *     Speed
 *     Direction
 *     Width
 *     StartLED
 *
 * Cyclotron:
 *   Hardware (same as above)
 *   Color/Speed as above
 *
 * Some kind of interface with the methods required, ie:
 *   startUp
 *   shutDown
 *   idle
 *   firing
 *   venting
 *   overheat
 *
 * eg Cyclotron Idle and Firing are the same except for speed
 *
 * needs to be able to determine firing duration
 */