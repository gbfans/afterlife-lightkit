#include "Control.h"

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

  if (_currentState == STATE_STANDALONE) {
    _mode = MODE_STANDALONE;
  } else {
    _mode = MODE_CONTROLLED;
  }
}

/**
 * Read the current state of all Input pins into a local variable
 */
void Control::update()
{
  _isChanged = false;

  if ((millis() - _lastDebounceTime) <= INPUT_DEBOUNCE_DELAY && _currentState != STATE_NONE) {
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

  CONTROL_STATES newState;

  switch (inputState) {
    case 1:
      newState = STATE_POWER_UP;
      break;
    case 2:
      newState = STATE_RED_CYCLOTRON;
      break;
    case 3:
      newState = STATE_GREEN_CYCLOTRON;
      break;
    case 4:
      newState = STATE_BLUE_CYCLOTRON;
      break;
    case 5:
      newState = STATE_ORANGE_CYCLOTRON;
      break;
    case 6:
      newState = STATE_AUTOMATIC_VENTING;
      break;
    case 7:
      newState = STATE_CHANGE_CYCLOTRON_COLOR;
      break;
    case 8:
      newState = STATE_IDLE;
      break;
    case 9:
      newState = STATE_VENT_STROBE;
      break;
    case 10:
      newState = STATE_TEST_MODE;
      break;
    case 11:
      newState = STATE_VENTING_ACTION;
      break;
    case 12:
      newState = STATE_FIRE_MOVIE;
      break;
    case 13:
      newState = STATE_FIRE_TVG;
      break;
    case 14:
      newState = STATE_POWER_DOWN;
      break;
    case 15:
      newState = STATE_STANDALONE;
      break;
    case 0:
    default:
      newState = STATE_OFF;
      break;
  }

  if (newState != _currentState) {
    // Input has changed
    // Serial.print("Changed! - Old: ");
    // Serial.print(_currentState);
    // Serial.print(", New: ");
    // Serial.println(newState);
    _lastDebounceTime = millis();
    _isChanged = true;
    _previousState = _currentState;
    _currentState = newState;
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
