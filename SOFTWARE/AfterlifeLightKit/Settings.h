#define POWERCELL_PIN 0
#define CYCLOTRON_PIN 5

enum Directions
{
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

struct LightStrip
{
    int length;
    unsigned char speed;
    unsigned char brightness;
    // GRB/RGB
    // WS2812B/WS2811 (or FastLed type?)
};

struct HardwareConfiguration
{
    // Default length of a Powercell should be 15 LEDs
    LightStrip Powercell = {15};
    // Default length of Cyclotron should be 60 LEDs
    LightStrip Cyclotron = {60};
};

struct RGBColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct PowercellSettings
{
    Directions direction;
    int speed;
    RGBColor color;
};

struct CyclotronSettings
{
    Directions direction;
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
