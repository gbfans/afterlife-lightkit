#include "src/ConfigManager/ConfigManager.h"
#include "src/Lights/Lights.h"
#include "src/GBFansControl/GBFansControl.h"

ConfigManager config;

MODES mode;
Lights lights;
GBFansControl controls;

void setup()
{
    //Serial.begin(115200);

    /**
     * Output Configuration
     */
    pinMode(SHIFT_PIN, OUTPUT); //For AJ's Logic level Shifter, this probably has to be initialized before the buttons.
    digitalWrite(SHIFT_PIN, HIGH);

    controls.init();

    config.init();
    lights.init(config);

    // Off by default
    lights.setState(INACTIVE);
}

void loop()
{
    controls.update();
    lights.update();

    if (controls.isState(STATE_OFF))
    {
        // Powered off
        Serial.println("Off");
        lights.setState(INACTIVE);
    }
    else if (controls.isState(STATE_IDLE))
    {
        // Idling
        Serial.println("Idle");
        lights.setState(IDLE);
    }
    else if (controls.isState(STATE_FIRE_MOVIE))
    {
        // Firing
        if (controls.duration() > 5000)
        {
            // We have been firing for > 5 seconds
            Serial.println("Overheating");
            lights.setState(OVERHEATING);
        }
        else
        {
            Serial.println("Firing");
            lights.setState(FIRING);
        }
    }
    else if (controls.isState(STATE_POWER_DOWN))
    {
        Serial.println("Powering Down");
        lights.setState(SHUTDOWN);
    }
}
