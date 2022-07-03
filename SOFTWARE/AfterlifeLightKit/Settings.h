#ifndef SETTINGS_h
#define SETTINGS_h

#include <Ramp.h>

// Which pins the Powercell & Cyclotron are connected to
#define POWERCELL_PIN 0
#define CYCLOTRON_PIN 5

// How many LEDS in each?
#define POWERCELL_LENGTH 15
#define CYCLOTRON_LENGTH 60

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
};

enum LIGHT_EFFECTS {
    CYCLING,
    SPINNING
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

struct Settings
{
    PowercellSettings powercell;
    CyclotronSettings cyclotron;
};

struct Configuration
{
    // TODO: Make CLASSIC the default mode
    MODES defaultMode = AFTERLIFE;
    Settings classicSettings;
    Settings afterlifeSettings;
};

#endif
