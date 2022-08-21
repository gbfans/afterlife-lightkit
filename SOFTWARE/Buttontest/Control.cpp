#include "Control.h"
#include "Arduino.h"

/**
 * Input pins from the 10-pin IDC connector to the GBFans Soundboard.
 * Pin 7  = MSB
 * Pin 10 = LSB
 *
 * 0: D5/GPIO14 (IDC Pin 7)
 * 1: D6/GPIO12 (IDC Pin 8)
 * 2: D0/GPIO16 (IDC Pin 9)
 * 3: D7/GPIO13 (IDC Pin 10)
 */
const int INPUT_PINS[4] = {D5,D6,D0,D7};

/**
 * How many milliseconds to debounce?
 */
const int INPUT_DEBOUNCE_DELAY = 5;

void Control::init()
{
  /**
   * Set all 4 input pins to INPUT_PULLUP
   */
  for (size_t i = 0; i < 4; i++)
  {
    pinMode(INPUT_PINS[i], INPUT_PULLUP);
  }

  // Load the current state of the inputs at boot
  update();

  // Serial.println();
  // Serial.print("First Boot State: ");
  // Serial.println(_currentInputState);

  if (_currentState == STATE_STANDALONE) {
    _mode = MODE_STANDALONE;
    //Serial.println("Standalone Mode");
  } else {
    _mode = MODE_CONTROLLED;
    //Serial.println("Controlled Mode");
  }
}

/**
 * Read the current state of all Input pins into a local variable
 */
void Control::update()
{
  _isChanged = false;

  if ((millis() - _lastDebounceTime) <= INPUT_DEBOUNCE_DELAY && _currentInputState < 99) {
    /**
     * Debounce interval has not been met, or we have never read the inputs (eg on first boot)
     */
    return;
  }

  int inputState = 0;
  if (digitalRead( INPUT_PINS[0] ) == HIGH) {
    inputState += 1;
  }
  if (digitalRead( INPUT_PINS[1] ) == HIGH) {
    inputState += 2;
  }
  if (digitalRead( INPUT_PINS[2] ) == HIGH) {
    inputState += 4;
  }
  if (digitalRead( INPUT_PINS[3] ) == HIGH) {
    inputState += 8;
  }

  if (inputState != _currentInputState) {
    // Input has changed
    // Serial.print("Changed! - Old: ");
    // Serial.print(_currentInputState);
    // Serial.print(", New: ");
    // Serial.println(inputState);

    _lastDebounceTime = millis();
    _currentInputState = inputState;
    _isChanged = true;
    _previousState = _currentState;

    switch (_currentInputState)
    {
      case 1:
        _currentState = STATE_POWER_UP;
        break;
      case 2:
        _currentState = STATE_RED_CYCLOTRON;
        break;
      case 3:
        _currentState = STATE_GREEN_CYCLOTRON;
        break;
      case 4:
        _currentState = STATE_BLUE_CYCLOTRON;
        break;
      case 5:
        _currentState = STATE_ORANGE_CYCLOTRON;
        break;
      case 6:
        _currentState = STATE_AUTOMATIC_VENTING;
        break;
      case 7:
        _currentState = STATE_CHANGE_CYCLOTRON_COLOR;
        break;
      case 8:
        _currentState = STATE_IDLE;
        break;
      case 9:
        _currentState = STATE_VENT_STROBE;
        break;
      case 10:
        _currentState = STATE_TEST_MODE;
        break;
      case 11:
        _currentState = STATE_VENTING_ACTION;
        break;
      case 12:
        _currentState = STATE_FIRE_MOVIE;
        break;
      case 13:
        _currentState = STATE_FIRE_TVG;
        break;
      case 14:
        _currentState = STATE_POWER_DOWN;
        break;
      case 15:
        _currentState = STATE_STANDALONE;
        break;
      case 0:
      default:
        _currentState = STATE_OFF;
        break;
    }
  }
}

CONTROL_MODES Control::getMode()
{
  return _mode;
}

CONTROL_STATES Control::getCurrentState()
{
  return _currentState;
}

CONTROL_STATES Control::getPreviousState()
{
  return _previousState;
}

bool Control::changed()
{
  return _isChanged;
}
