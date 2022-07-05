#ifndef SETTINGS_h
#define SETTINGS_h

#include <Ramp.h>

// Which pins the Powercell & Cyclotron are connected to
#define POWERCELL_PIN 0
#define CYCLOTRON_PIN 5

// How many LEDS in each?
#define POWERCELL_LENGTH 15
#define CYCLOTRON_LENGTH 60

//Define the default Hardware pins
#define ENABLE_BTN_PIN  13  //Enable lights (ACTIVE HIGH)
#define FIRE_BTN_PIN    16  //Trigger firing (ACTIVE LOW)
#define VENT_BTN_PIN    12  //Trigger venting (ACTIVE LOW
#define CHANGE_BTN_PIN  14  //Change between video game modes (ACTIVE LOW)
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
    CYCLING,
    SPINNING,
    RAINBOW,
    RAINBOW_SCROLL,
    CYLON,
    ALTERNATE
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
