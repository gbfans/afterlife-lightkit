#include <Bounce2.h>
#include "Control.h"

Bounce2::Button enableButton = Bounce2::Button();
Bounce2::Button fireButton = Bounce2::Button();
Bounce2::Button ventButton = Bounce2::Button();
Bounce2::Button changeButton = Bounce2::Button();

#define ENABLE_BTN_PIN D7 //Enable lights (ACTIVE HIGH)
#define FIRE_BTN_PIN D0 //Trigger firing (ACTIVE LOW)
#define VENT_BTN_PIN D6 //Trigger venting (ACTIVE LOW
#define CHANGE_BTN_PIN D5 //Change between video game modes (ACTIVE LOW)
#define SHIFT_PIN 15 //Enable logic shifter (ACTIVE HIGH)

Control controls;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  /**
   * Output Configuration
   */
  pinMode(SHIFT_PIN, OUTPUT); //For AJ's Logic level Shifter, this probably has to be initialized before the buttons.
  digitalWrite(SHIFT_PIN, HIGH);
  delay(100);
  controls.init();

  Serial.println("Ready...");
  
}

void loop() {
  controls.update();
}
