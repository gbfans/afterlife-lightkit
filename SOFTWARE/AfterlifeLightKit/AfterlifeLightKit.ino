#include "ConfigManager.h"
#include "Lights.h"
#include "Control.h"

ConfigManager config;

MODES mode;
Lights lights;
Control controls;

    unsigned long testLastMillis = 0;
    int delayTime = 100000;

void setup()
{
    Serial.begin(115200);

    /**
     * Output Configuration
     */
    pinMode(SHIFT_PIN, OUTPUT); //For AJ's Logic level Shifter, this probably has to be initialized before the buttons.
    digitalWrite(SHIFT_PIN, HIGH);
    pinMode(LED_BUILTIN, OUTPUT);

    config.init();
    lights.init(config);
    lights.setState(INACTIVE);

    controls.init(lights);
    
}

void loop()
{
    controls.update();
    lights.update();
}
