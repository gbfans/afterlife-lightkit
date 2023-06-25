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
    MODE_PROTON,
    MODE_SLIME,
    MODE_STASIS,
    MODE_MESON
};

enum PACKSTATES
{
    PACK_INACTIVE,
    PACK_START,
    PACK_IDLE,
    PACK_FIRING,
    PACK_OVERHEATING,
    PACK_VENTING,
    PACK_SHUTDOWN
};

#define STR_EFFECT_OFF "OFF"
#define STR_EFFECT_ALL_ON "ALL_ON"
#define STR_EFFECT_CYCLING "CYCLING"
#define STR_EFFECT_SPINNING "SPINNING"
#define STR_EFFECT_RAINBOW "RAINBOW"
#define STR_EFFECT_RAINBOW_SCROLL "RAINBOW_SCROLL"
#define STR_EFFECT_CYLON "CYLON"
#define STR_EFFECT_TETRIS "TETRIS"
#define STR_EFFECT_DESCEND "DESCEND"
#define STR_EFFECT_ALTERNATE "ALTERNATE"
#define STR_EFFECT_BLINKING "BLINKING"

enum LIGHT_EFFECTS {
    EFFECT_OFF,
    EFFECT_ALL_ON,
    EFFECT_CYCLING,
    EFFECT_SPINNING,
    EFFECT_RAINBOW,
    EFFECT_RAINBOW_SCROLL,
    EFFECT_CYLON,
    EFFECT_TETRIS,
    EFFECT_DESCEND,
    EFFECT_ALTERNATE,
    EFFECT_BLINKING,
};

typedef struct StateConfig
{
    LIGHT_EFFECTS effect;
    int startSpeed;
    int endSpeed;
    int duration;
    bool reverse = false;
} StateConfig;

/**
 * Configuration for each Strip include how to behave in each Pack State
 */
typedef struct StripSettings
{
    // Color is the same across all States
    unsigned long color;
    StateConfig inactive;
    StateConfig start;
    StateConfig idle;
    StateConfig firing;
    StateConfig overheating;
    StateConfig venting;
    StateConfig shutdown;
} StripSettings;

typedef struct Settings
{
    StripSettings powercell;
    StripSettings cyclotron;
    StripSettings nfilter;
} Settings;

/**
 * How the pack should behave in each Mode
 */
typedef struct Configuration
{
    Settings proton;
    Settings slime;
    Settings stasis;
    Settings meson;
} Configuration;

#endif
