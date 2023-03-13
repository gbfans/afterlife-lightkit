# GBFansControl Library

The purpose of this library is to provide an abstraction layer for the GBFans Proton Pack Sound Board:
https://www.gbfans.com/shop/electronics/amplified-sound-board/

This allows the Afterlife Lightkit to receive commands/states from the Sound Board without any knowledge of the underlying hardware or communication protocol.

NOTE: This library is specifically designed for the Afterlife Lightkit hardware running on an ESP8266,
as such the pin definitions may not work on other devices without modifications to the library.

It is currently outside the scope of this library to support other hardware however this could be implemented relatively easily.

## Provided States
The library provides the following States

**Legend**
* **State**: The ENUM state returned by this library if received from the Sound Board
* **Sequence**: The PIN sequence received from the Sound Board via the 4 data pins on the 10-pin IDC connector
* **Value**: The bitwise value received
* **Description**: What this State actually refers to

| State                        | Sequence | Value | Description                                        |
|------------------------------|----------|-------|----------------------------------------------------|
| STATE_OFF                    | 0,0,0,0  |     0 | Off                                                |
| STATE_POWER_UP               | 0,0,0,1  |     1 | Power Up                                           |
| STATE_RED_CYCLOTRON          | 0,0,1,0  |     2 | Red Cyclotron Color                                |
| STATE_GREEN_CYCLOTRON        | 0,0,1,1  |     3 | Green Cyclotron Color                              |
| STATE_BLUE_CYCLOTRON         | 0,1,0,0  |     4 | Blue Cyclotron Color                               |
| STATE_ORANGE_CYCLOTRON       | 0,1,0,1  |     5 | Orange Cyclotron Color                             |
| STATE_AUTOMATIC_VENTING      | 0,1,1,0  |     6 | Automatic Venting Mode                             |
| STATE_CHANGE_CYCLOTRON_COLOR | 0,1,1,1  |     7 | Change Cyclotron Colors (Standalone mode only)**   |
| STATE_IDLE                   | 1,0,0,0  |     8 | Normal Operation (Idle) (both modes)               |
| STATE_VENT_STROBE            | 1,0,0,1  |     9 | Venting Strobe Only                                |
| STATE_TEST_MODE              | 1,0,1,0  |    10 | Test Mode                                          |
| STATE_VENTING_ACTION         | 1,0,1,1  |    11 | Venting Action with Interaction (both modes)       |
| STATE_FIRE_MOVIE             | 1,1,0,0  |    12 | Fire (movie version)                               |
| STATE_FIRE_TVG               | 1,1,0,1  |    13 | Fire (TVG, overheat version) (Both modes)          |
| STATE_POWER_DOWN             | 1,1,1,0  |    14 | Power Down (both modes)                            |
| STATE_STANDALONE             | 1,1,1,1  |    15 | Delay -> Power Up -> Idle (Standalone mode only)** |
| STATE_NONE                   | N/A      |    16 | None (ie State has not yet been read)              |

* **Standalone Mode** refers to the ability to run the Sound Board alone.
As this doesn't apply to the Afterlife LightKit, support for this mode is not fully implemented in this library.

## Usage

### Available Methods

| Method               | Description                                                     | Usage                                 |
|----------------------|-----------------------------------------------------------------|---------------------------------------|
| `init()`             | Initialize the Library                                          | `controls.init();`                    |
| `update()`           | Check for new Values                                            | `controls.update();`                  |
| `changed()`          | Returns `true` if the state has changed since the last loop     | `controls.changed();`                 |
| `getMode()`          | Returns the Mode                                                | `controls.getMode();`                 |
| `isMode()`           | Returns `true` if the Mode matches the provided value           | `controls.isMode(MODE_STANDALONE);`   |
| `getCurrentState()`  | Returns the Current State                                       | `controls.getCurrentState();`         |
| `isState()`          | Returns `true` if the Current State matches the provided value  | `controls.isState(STATE_FIRE_MOVIE);` |
| `getPreviousState()` | Returns the Previous State                                      | `controls.getPreviousState();`        |
| `wasState()`         | Returns `true` if the Previous State matches the provided value | `controls.wasState(STATE_IDLE);`      |


### Example Sketch
```c++
#include "GBFansControl.h"

// Instantiate the GBFans Control instance
GBFansControl controls;

void setup()
{
    Serial.begin(115200);

    // Initialize the Controls
    controls.init();

    if (controls.isMode(MODE_CONTROLLED)) {
        // Handle Controlled Mode
    } else {
        // Unsupported Mode (eg Standalone)
    }
}

void loop() {
    // Check for new Values (must be run every loop)
    controls.update();

    // Returns true if the Controls now return a new value
    // eg if a button was pressed on the Wand connected to the Sound Board
    if (controls.changed()) {
        // Display the Old/Previous State
        Serial.print("CHANGED - Old: ");
        Serial.print(controls.getPreviousState());
        // Display the New/Current State
        Serial.print(", New: ");
        Serial.println(controls.getCurrentState());

        if (controls.isState(STATE_IDLE)) {
            // Pack is powered up and idle, enable the Pack lights
        }
        else if(controls.isState(STATE_OFF)) {
            // Pack is switched off, turn the Pack Lights off
        }
    }
}
```

## ButtonTest Sketch
`ButtonTest.ino` allows the debugging of the library against the GBFans Sound Board.
Connect the 10-pin IDC Connector to the Afterlife board and trigger the various actions while the Serial Monitor is running.

Example Output (triggered by switching on the Sound Board and cycling through various states):
```
Boot Mode: Controlled
CHANGED - Old: 1, New: 7
CHANGED - Old: 7, New: 0
CHANGED - Old: 0, New: 8
CHANGED - Old: 8, New: 1
CHANGED - Old: 1, New: 11
CHANGED - Old: 11, New: 1
CHANGED - Old: 1, New: 9
CHANGED - Old: 9, New: 1
CHANGED - Old: 1, New: 9
CHANGED - Old: 9, New: 1
CHANGED - Old: 1, New: 9
CHANGED - Old: 9, New: 1
CHANGED - Old: 1, New: 11
CHANGED - Old: 11, New: 1
CHANGED - Old: 1, New: 7
CHANGED - Old: 7, New: 0
```