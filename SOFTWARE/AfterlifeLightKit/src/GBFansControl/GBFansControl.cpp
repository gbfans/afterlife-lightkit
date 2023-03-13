#include "GBFansControl.h"

/**
 * Input pins from the 10-pin IDC connector to the GBFans Soundboard.
 * NOTE: Pin 1 is typically closest to the red wire
 *
 * Pin 1  = 12V
 * Pin 2  = 12V
 * Pin 3  = 12V
 * Pin 4  = GND
 * Pin 5  = GND
 * Pin 6  = GND
 * Pin 7  = D5/GPIO14 (MSB)
 * Pin 8  = D6/GPIO12
 * Pin 9  = D0/GPIO16
 * Pin 10 = D7/GPIO13 (LSB)
 */
const int INPUT_PINS[4] = {D5, D6, D0, D7};

/**
 * How many milliseconds to debounce?
 */
const int INPUT_DEBOUNCE_DELAY = 50;

/**
 * How many frames of the same signal should we require before we consider it 'real'?
 * (Resolves potential data interference issues over loose wires)
 *
 * For example, we must receive 'Firing' 50 times in a row (approx 50 milliseconds)
 * before we actually believe that we received that signal from the Sound Board.
 * NOTE: This should be a lower number than the duration of a "quick" button press,
 * such as firing a boson dart. A number too high would result in those button presses being
 * ignored.
 *
 */
const int INPUT_MIN_REQUIRED_FRAMES = 50;

void GBFansControl::init()
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

    if (_currentState == STATE_STANDALONE)
    {
        _mode = MODE_STANDALONE;
    }
    else
    {
        _mode = MODE_CONTROLLED;
    }
}

/**
 * Read the current state of all Input pins into a local variable
 */
void GBFansControl::update()
{
    _isChanged = false;

    if ((millis() - _lastDebounceTime) <= INPUT_DEBOUNCE_DELAY && _currentState != STATE_NONE)
    {
        /**
         * Debounce interval has not been met, or we have never read the inputs (eg on first boot)
         */
        return;
    }

    int inputState = 0;
    if (digitalRead(INPUT_PINS[0]) == HIGH)
    {
        inputState += 1;
    }
    if (digitalRead(INPUT_PINS[1]) == HIGH)
    {
        inputState += 2;
    }
    if (digitalRead(INPUT_PINS[2]) == HIGH)
    {
        inputState += 4;
    }
    if (digitalRead(INPUT_PINS[3]) == HIGH)
    {
        inputState += 8;
    }

    if (inputState == _lastInputFrame)
    {
        // This is the same input we received last time
        if (_inputFramesReceived < INPUT_MIN_REQUIRED_FRAMES)
        {
            // We haven't met the required min frames yet
            _inputFramesReceived++;
            return;
        }
    }

    if (inputState != _lastInputFrame)
    {
        // We received a new input signal from the Sound Board

        // Reset frame count back to 1
        _inputFramesReceived = 1;

        // Set the Last Input received to check against next time
        _lastInputFrame = inputState;

        // Do not proceed further until this frame has been verified
        return;
    }

    CONTROL_STATES newState;

    switch (inputState)
    {
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

    if (newState != _currentState)
    {
        // Input has changed
//         Serial.print("Changed! - Old: ");
//         Serial.print(_currentState);
//         Serial.print(", New: ");
//         Serial.println(newState);
        _lastDebounceTime = millis();
        _isChanged = true;
        _previousState = _currentState;
        _currentState = newState;
    }
}

CONTROL_MODES GBFansControl::getMode()
{
    return _mode;
}

bool GBFansControl::isMode(CONTROL_MODES mode)
{
    return _mode == mode;
}

CONTROL_STATES GBFansControl::getCurrentState()
{
    return _currentState;
}

CONTROL_STATES GBFansControl::getPreviousState()
{
    return _previousState;
}

bool GBFansControl::isState(CONTROL_STATES state)
{
    return _currentState == state;
}

bool GBFansControl::wasState(CONTROL_STATES state)
{
    return _previousState == state;
}

bool GBFansControl::changed()
{
    return _isChanged;
}

/**
 * How long were we in this State for?
 */
unsigned long GBFansControl::duration()
{
    return (unsigned long)(millis() - _lastDebounceTime);
}
