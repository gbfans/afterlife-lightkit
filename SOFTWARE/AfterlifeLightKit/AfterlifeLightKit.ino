#include "ConfigManager.h"
#include "Lights.h"

ConfigManager config;

MODES mode;
Lights lights;

#include <Bounce2.h>
Bounce2::Button enableButton = Bounce2::Button();
Bounce2::Button fireButton   = Bounce2::Button();
Bounce2::Button ventButton   = Bounce2::Button();
Bounce2::Button changeButton = Bounce2::Button();

void setup()
{
    Serial.begin(115200);

    /**
     * Output Configuration
     */
    pinMode(SHIFT_PIN, OUTPUT); //For AJ's Logic level Shifter, this probably has to be initialized before the buttons.
    digitalWrite(SHIFT_PIN, HIGH);
    pinMode(LED_BUILTIN, OUTPUT);

    /**
     * Input Configuration
     */
    enableButton.attach(ENABLE_BTN_PIN, INPUT_PULLUP);
    enableButton.interval(5);     // Use a debounce interval of 5 milliseconds
    enableButton.setPressedState(HIGH);     // INDICATE THAT THE HIGH STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
     
    fireButton.attach(FIRE_BTN_PIN, INPUT);
    fireButton.interval(5);     // Use a debounce interval of 5 milliseconds
    fireButton.setPressedState(LOW);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
    
    ventButton.attach(VENT_BTN_PIN, INPUT_PULLUP);
    ventButton.interval(5);     // Use a debounce interval of 5 milliseconds
    ventButton.setPressedState(LOW);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

    changeButton.attach(CHANGE_BTN_PIN, INPUT_PULLUP);
    changeButton.interval(5);     // Use a debounce interval of 5 milliseconds
    changeButton.setPressedState(LOW);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

    config.init();
    lights.init(config);
    lights.setState(INACTIVE);

    lights.testChangeCyclotronBrightness(0, 0);
    lights.testChangeCyclotronSpeed(255, 0);

}

// unsigned long testLastMillis = 0;
// int delayTime = 10000;
void loop()
{
    lights.update();

    fireButton.update();
    enableButton.update();
    ventButton.update();
    changeButton.update();

    if (enableButton.isPressed())
    {

        if (enableButton.pressed())
        {
            lights.testChangeCyclotronBrightness(50, 10000);
            lights.testChangeCyclotronSpeed(10, 10000);
            lights.setState(START); 
        }
        else if (fireButton.pressed())
        {
            lights.testChangeCyclotronBrightness(255, 2500);
            lights.testChangeCyclotronSpeed(5, 5000);
            lights.setState(FIRING);
        }
        else if (fireButton.released())
        {
            lights.testChangeCyclotronBrightness(50, 2500);
            lights.testChangeCyclotronSpeed(10, 5000);
            lights.setState(IDLE);
        }
        else if (ventButton.pressed())
        {
            lights.setState(VENTING);
        }
        else if (changeButton.pressed())
        {
            //Changing for videogame mode.
        }

    }
    else if (enableButton.released())
    {
        lights.testChangeCyclotronSpeed(255, 10000);
        lights.testChangeCyclotronBrightness(0, 5000);
        lights.setState(SHUTDOWN);
    }
}
