#include "ConfigManager.h"
#include "Lights.h"

ConfigManager config;

MODES mode;
Lights lights;

/**
 * DEMO: Connect a button to D0/GPIO16
 * This code will be removed later.
 */
#define FIRE_BTN_PIN 16
#include <Bounce2.h>
Bounce2::Button fireButton = Bounce2::Button();


void setup()
{
    Serial.begin(115200);

    /**
     * DEMO: Pushbutton connected to GPIO16 (D0)
     * NOTE: GPIO16 has a built-in pulldown resistor
     */
    fireButton.attach(FIRE_BTN_PIN, INPUT_PULLDOWN_16);
    // Use a debounce interval of 5 milliseconds
    fireButton.interval(5);

    // INDICATE THAT THE HIGH STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
    fireButton.setPressedState(HIGH);
    pinMode(LED_BUILTIN, OUTPUT);


    config.init();
    lights.init(config);

    /**
     * NOTE: Seeing if this will allow us to start at zero and ramp up
     *       on startup? This call occurs after lights.init so FX should already be initialized
     */
    lights.testChangeCyclotronSpeed(100, 0); // drop to 100ms (very slow)
    lights.testChangeCyclotronSpeed(10, 3000); // spin up to 10ms over 3 seconds
}

// unsigned long testLastMillis = 0;
// int delayTime = 10000;
void loop()
{
    lights.update();

    /**
     * NOTE: Test code below.
     * When the fire button is pressed, speed up to 4ms over 3 seconds.
     * When button released, slow back down to 10ms over 3 seconds.
     */
    fireButton.update();
    if (fireButton.pressed()) {
        // We are firing
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("Firing");
        lights.testChangeCyclotronSpeed(4, 3000);
    } else if(fireButton.released()) {
        // Stopped firing
        Serial.println("Stopped");
        digitalWrite(LED_BUILTIN, LOW);
        lights.testChangeCyclotronSpeed(10, 3000);
    }
}
