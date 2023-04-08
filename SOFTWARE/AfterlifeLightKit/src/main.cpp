#include <Arduino.h>

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
WiFiManager wm;

#include "Lights/Lights.h"
#include "GBFansControl/GBFansControl.h"
#include "Settings.h"

MODES mode;
Lights lights;
GBFansControl controls;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    /**
     * Output Configuration
     */
    pinMode(SHIFT_PIN, OUTPUT); //For AJ's Logic level Shifter, this probably has to be initialized before the buttons.
    digitalWrite(SHIFT_PIN, HIGH);

    controls.init();

    lights.init();

    // Off by default
    lights.setState(INACTIVE);
    lights.update(true);

    Serial.begin(115200);
}

void loop()
{
    controls.update();

    /**
     * NOTE: It is safe to keep setting the State over again here
     * as setState includes a check to prevent changing to the same State
     */
    if (controls.isState(STATE_OFF))
    {
        // Powered off
        //Serial.println("Off");
        lights.setState(INACTIVE);
    }
    else if (controls.isState(STATE_POWER_UP))
    {
        // Starting up
        //Serial.println("Startup");
        lights.setState(START);
    }
    else if (controls.isState(STATE_IDLE))
    {
        // Idling
        //Serial.println("Idle");
        lights.setState(IDLE);
    }
    else if (controls.isState(STATE_FIRE_MOVIE))
    {
        // Firing
        if (controls.duration() > 5000)
        {
            // We have been firing for > 5 seconds
            //Serial.println("Overheating");
            lights.setState(OVERHEATING);
        }
        else
        {
            //Serial.println("Firing");
            lights.setState(FIRING);
        }
    }
    else if (controls.isState(STATE_POWER_DOWN))
    {
        //Serial.println("Powering Down");
        lights.setState(SHUTDOWN);
    }

    // We should always update Lights AFTER the State has been set
    lights.update();
}
