#ifndef SETTINGS_h
#define SETTINGS_h

// Set DEBUG to 1 to enable debug output to Serial
// IMPORTANT: Leave DEBUG disabled in Production mode
// as this can have a negative performance impact.
// DO NOT COMMIT "DEBUG=1"
#define DEBUG 0
#if DEBUG == 1
    #define debug(x) Serial.print(x)
    #define debugln(x) Serial.println(x)
#else
    #define debug(x)
    #define debugln(x)
#endif

// Which pins the Powercell, Cyclotron, and N-Filter lights are connected to
#define POWERCELL_PIN 0
#define CYCLOTRON_PIN 5
#define NFILTER_PIN 4

// How many LEDS in each?
#define POWERCELL_LENGTH 15
#define CYCLOTRON_LENGTH 40
#define NFILTER_LENGTH 16

//Define the default Hardware pins
#define SHIFT_PIN       15  //Enable logic shifter (ACTIVE HIGH)

enum DIRECTIONS {
    LIGHTS_FORWARD,
    LIGHTS_REVERSE
};

enum MODES
{
    CLASSIC,
    AFTERLIFE,
    TVG,
    REBOOT,
};

enum PACKSTATES
{
    INACTIVE,
    START,
    IDLE,
    FIRING,
    OVERHEATING,
    VENTING,
    SHUTDOWN,
    PARTY
};

enum LIGHT_EFFECTS {
    OFF,
    ALL_ON,
    CYCLING,
    SPINNING,
    RAINBOW,
    RAINBOW_SCROLL,
    CYLON,
    TETRIS,
    DESCEND,
    ALTERNATE,
    BLINKING,
};

struct RGBColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct PowercellSettings
{
    DIRECTIONS direction;
    int speed;
    RGBColor color;
};

struct CyclotronSettings
{
    DIRECTIONS direction;
    int speed;
    RGBColor color;
};

struct NfilterSettings
{
    DIRECTIONS direction;
    int speed;
    RGBColor color;
};

struct Settings
{
    PowercellSettings powercell;
    CyclotronSettings cyclotron;
    NfilterSettings nfilter;
};

struct Configuration
{
    // TODO: Make CLASSIC the default mode
    MODES defaultMode = AFTERLIFE;
    Settings classicSettings;
    Settings afterlifeSettings;
};

#endif
