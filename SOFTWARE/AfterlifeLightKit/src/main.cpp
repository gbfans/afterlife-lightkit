#include <Arduino.h>

#include "Settings.h"
#include "Lights/Lights.h"
#include "GBFansControl/GBFansControl.h"
#include "WebServer/WebServer.h"

MODES mode;
Lights lights;
GBFansControl controls;
WebServer server;

void setup()
{
    Serial.begin(115200);
    debugln(F("Starting up - This should only appear once"));

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    /**
     * Output Configuration
     */
    pinMode(SHIFT_PIN, OUTPUT); //For AJ's Logic level Shifter, this probably has to be initialized before the buttons.
    digitalWrite(SHIFT_PIN, HIGH);

    controls.init();
    lights.init();
    server.init();
}

void loop()
{
    controls.update();

    if (controls.isSequenceMatch("1:14:1:14:1:14:0")) {
        // We look for a sequence of "Startup => Powerdown" 3 times, which ends in "Off"
        // This can be triggered by switching the "Activate" switch On/Off 3 times.
        // (There shouldn't be a long enough pause between switches for the pack to finish Startup
        // and enter "Idle" State).
        if (server.isStarted()) {
            server.end();
        } else {
            server.begin();
        }
    }

    /**
     * NOTE: It is safe to keep setting the State over again here
     * as lights.setState() includes a check to prevent changing to the same State
     */
    if (controls.isState(STATE_OFF))
    {
        // Powered off
        //debugln(F("Off"));
        lights.setState(PACK_INACTIVE);
    }
    else if (controls.isState(STATE_POWER_UP))
    {
        // Starting up
        //debugln(F("Startup"));
        lights.setState(PACK_START);
    }
    else if (controls.isState(STATE_IDLE))
    {
        // Idling
        //debugln(F("Idle"));
        lights.setState(PACK_IDLE);
    }
    else if (
            controls.isState(STATE_FIRE_MOVIE) ||
            controls.isState(STATE_FIRE_TVG)
    ) {
        // Firing
        // NOTE: For the Pack Lights there is functionally no difference between MOVIE and TVG mode firing
        // TODO: Remove the duration check!
        if (controls.duration() > 5000)
        {
            // We have been firing for > 5 seconds
            //debugln(F("Overheating"));
            lights.setState(PACK_OVERHEATING);
        }
        else
        {
            //debugln(F("Firing"));
            lights.setState(PACK_FIRING);
        }
    }
    else if (controls.isState(STATE_POWER_DOWN))
    {
        //debugln(F("Powering Down"));
        lights.setState(PACK_SHUTDOWN);
    }

    // We should always update Lights AFTER the State has been set
    lights.update();

    server.check();
}
