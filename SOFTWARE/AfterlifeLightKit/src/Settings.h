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

/**
 * The Color of this lightStrip in each Mode
 * NOTE: We use the same Color across all States
 * (i.e. Red while starting up, idle, firing, AND shutting down in Proton Mode).
 */
typedef struct ColorConfig
{
    unsigned long powercell;
    unsigned long cyclotron;
    unsigned long nfilter;

} ColorConfig;

typedef struct ColorSettings
{
    ColorConfig proton;
    ColorConfig slime;
    ColorConfig stasis;
    ColorConfig meson;
} ColorSettings;

typedef struct StateConfig
{
    // Which Effect should we use for this State?
    LIGHT_EFFECTS effect;
    // What speed (interval) should we start at?
    int startSpeed;
    // What speed (interval) should we end at?
    int endSpeed;
    // How long (in milliseconds) should we take to transition to the end speed?
    int duration;
    // Should we reverse the animation?
    bool reverse = false;
    // Should we perform a reset before starting this animation?
    bool reset = false;
} StateConfig;

/**
 * Configuration for each Strip include how to behave in each Pack State
 */
typedef struct StripSettings
{
    StateConfig powercell;
    StateConfig cyclotron;
    StateConfig nfilter;

} StripSettings;

typedef struct Configuration
{
    ColorSettings color;
    StripSettings inactive;
    StripSettings start;
    StripSettings idle;
    StripSettings firing;
    StripSettings overheating;
    StripSettings venting;
    StripSettings shutdown;
} Configuration;

#endif
