#include "Control.h"

//Enable logic shifter (D8/GPIO15, ACTIVE HIGH)
#define SHIFT_PIN 15

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

  Serial.print("Boot Mode: ");
  int mode = controls.getMode();
  if (mode == MODE_STANDALONE) {
    Serial.println("Standalone");
  } else {
    Serial.println("Controlled");
  }
}

void loop() {
  controls.update();
  if (controls.changed()) {
    Serial.print("CHANGED - Old: ");
    Serial.print(controls.getPreviousState());
    Serial.print(", New: ");
    Serial.println(controls.getCurrentState());
  }
}
