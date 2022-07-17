#include "ConfigManager.h"
#include "Lights.h"
#include "Control.h"

ConfigManager config;

MODES mode;
Lights lights;
Control controls;

void setup()
{
    Serial.begin(115200);

    /**
     * Output Configuration
     */
    pinMode(SHIFT_PIN, OUTPUT); //For AJ's Logic level Shifter, this probably has to be initialized before the buttons.
    digitalWrite(SHIFT_PIN, HIGH);
    pinMode(LED_BUILTIN, OUTPUT);

    controls.init();

    config.init();
    lights.init(config);
    lights.setState(INACTIVE);

    lights.testChangeCyclotronBrightness(0, 0);
    lights.testChangeCyclotronSpeed(20, 0);
    lights.testChangeCyclotronBrightness(10, 0);

}

// unsigned long testLastMillis = 0;
// int delayTime = 10000;
void loop()
{
    controls.update();
    lights.update();
}
