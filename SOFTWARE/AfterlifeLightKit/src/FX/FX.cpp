#include "FX.h"

void FX::init(CRGB *pixels, int stripLength, CRGB ledColor, DIRECTIONS direction, int speed)
{
    _pixels = pixels;
    _stripLength = stripLength;
    _ledColor = ledColor;
    _direction = direction;
    _speed = _originalSpeed = speed;
    _brightness = _originalBrightness = 255;

    // We need to set the Grain to 1ms or instant speed changes will not work
    _speedRamp.setGrain(1);

    // Start at default speed
    _speedRamp.go(_speed);
}

void FX::stop()
{
    // Switch all lights off
    setEffect(OFF, true);

    // Update immediately
    update(true);
}

void FX::allOn()
{
    // Switch all lights on
    setEffect(ALL_ON);

    // Update immediately
    update(true);
}

void FX::setEffect(LIGHT_EFFECTS effect, bool reset)
{
    if (reset) {
        // We want to reset everything along with the Effect change
        _currentPixel = _getFirstPixel();
        _tetrisProgress = 0;
        _descendLoopCount = 0;
        _alternateState = 0;
    }

    _effect = effect;
}

void FX::setReverse(bool isReverse)
{
    if (!isReverse) {
        _currentPixel = _getFirstPixel();
        _direction = LIGHTS_FORWARD;
        return;
    }

    _currentPixel = _getLastPixel();
    _direction = LIGHTS_REVERSE;
}

void FX::changeSpeed(int newSpeed)
{
    if (newSpeed == _speed)
    {
        return;
    }

    // Instantly change speed
    _speed = newSpeed;
    _speedRamp.go(newSpeed);
}

void FX::changeSpeed(int newSpeed, int delay, ramp_mode rampMode)
{
    if (newSpeed == _speed)
    {
        return;
    }

    _speed = newSpeed;
    _speedRamp.go(newSpeed, delay, rampMode, ONCEFORWARD);
}

void FX::changeBrightness(int newBrightness)
{
    if (newBrightness == _brightness) {
        return;
    }

    _brightness = newBrightness;
}

/**
 * TODO: Originally this used a Ramp to calculate the brightness
 *       but this proved too resource intensive. For now, return the current value
 * @return
 */
int FX::updateBrightness()
{
      return _brightness;
}

void FX::reset()
{
    // Instantly reset speed/brightness back to original values
    changeSpeed(_originalSpeed);
    changeBrightness(_originalBrightness);
}

bool FX::update(bool force)
{
    _speed = _speedRamp.update();

    if (!_checkTimer() && !force) {
        return false;
    }

    /*
     *  If speed is equal to 255, movement should probably just be stopped and LEDs off.
     */

    switch (_effect)
    {
        case OFF:
            _clear();
            break;
        case ALL_ON:
            _allOn();
            break;
        case CYCLING:
            _cycling();
            break;
        case SPINNING:
            _spinning();
            break;
        case RAINBOW:
            _rainbow();
            break;
        case RAINBOW_SCROLL:
            _rainbowScroll();
            break;
        case CYLON:
            _cylon();
            break;
        case TETRIS:
            _tetris();
            break;
        case DESCEND:
            _descend();
            break;
        case ALTERNATE:
            _alternate();
            break;
        case BLINKING:
            _blinking();
            break;
        default:
            //Serial.print("Unknown: ");
            //Serial.println(_effect);
            break;
    }

    return true;
}

void FX::_clear()
{
    for (uint16_t i = 0; i < _stripLength; i++)
    {
        /**
         * Set all LEDS to black.
         */
        _pixels[i] = CRGB::Black;
    }

    // Reset (back to start)
    _currentPixel = _getFirstPixel();
}

void FX::_allOn()
{
    for (uint16_t i = 0; i < _stripLength; i++)
    {
        /**
         * Set all LEDS to on.
         */
        _pixels[i] = _getLedColor();
    }
}

void FX::_spinning()
{
    _direction = LIGHTS_FORWARD;

    for (uint16_t i = 0; i < _stripLength; i++)
    {
        /**
         * Illuminate LEDs up to and including the current LED,
         * Set the others to black.
         */
        _pixels[i] = (i <= _currentPixel) ? _getLedColor() : CRGB::Black;
    }

    _currentPixel = _getNextPixel();

    if (_currentPixel == _stripLength)
    {
        // We reached the end, go back to first LED
        _currentPixel = _getFirstPixel();
    }
}

void FX::_cycling()
{
    _direction = LIGHTS_FORWARD;

    for (uint16_t i = 0; i < _stripLength; i++)
    {
        /**
         * Illuminate current LED, set the others to black.
         */
        _pixels[i] = (i == _currentPixel) ? _getLedColor() : CRGB::Black;
    }

    _currentPixel = _getNextPixel();
}


void FX::_rainbow()
{
      uint8 hue = round(( (float) _currentPixel / (float) (_stripLength-1)) * 255.0);
      for (uint16_t i = 0; i < _stripLength; i++) {
        _pixels[i].setHue(hue); 
      }

    _currentPixel = _getNextPixel();
}

void FX::_rainbowScroll()
{
    uint8 hue = round(( (float) _currentPixel / (float) (_stripLength-1)) * 255.0);
    fill_rainbow( _pixels, _stripLength, hue, 255/_stripLength );
    _currentPixel = _getNextPixel();
}

void FX::_cylon()
{
    uint8 hue = round((float) _currentPixel / (float) (_stripLength-1));
    _pixels[_currentPixel].setHue(hue);
    _currentPixel = _getNextPixel();

    if ((_currentPixel == _getFirstPixel()) && (_direction == LIGHTS_FORWARD))
    {
        // We reached the end, go backwards
        _direction = LIGHTS_REVERSE;
        _currentPixel = _getLastPixel();
    }
    else if ((_currentPixel == _getFirstPixel()) && (_direction == LIGHTS_REVERSE))
    {
        // We reached the start, go forwards
        _direction = LIGHTS_FORWARD;
    }
}

void FX::_alternate()
{
    for (uint16_t i = 0; i < _stripLength; i++) {
        _pixels[i] = (_alternateState%2 == i%2) ? _getLedColor() : CRGB::Black;
    }
    _alternateState = !_alternateState;
}

/**
 * Set all LEDS on/off
 */
void FX::_blinking()
{
    // Lazy way to alternative on/off states for all LEDS
    _currentPixel = (_currentPixel == 1) ? 0 : 1;

    for (uint16_t i = 0; i < _stripLength; i++)
    {
        /**
         * Illuminate all LEDs if _currentPixel is set, else set all to black.
         */
        _pixels[i] = (_currentPixel) ? _getLedColor() : CRGB::Black;
    }
}

/**
 * LED starts at top and drops down to the bottom, gradually filling the strand.
 * (similar to Tetris)
 */
void FX::_tetris()
{
    _direction = LIGHTS_REVERSE;

    for (uint16_t i = 0; i < _stripLength; i++)
    {
        // If LED is below the current progress, OR is the current pixel, we light it up
        _pixels[i] = (i < _tetrisProgress || i == _currentPixel) ? _getLedColor() : CRGB::Black;
    }

    if (_currentPixel == _tetrisProgress) {
        // We have reached the last LED to light up, so we increase the progress
        _tetrisProgress++;

        // Skip the rest of the LEDs below this point
        _currentPixel = _getLastPixel(); // go back to end of strip and work backwards
        return;
    }

    _currentPixel = _getNextPixel();
}

/**
 * LED strip full, starts at top and drops down to the bottom
 */
void FX::_descend()
{
    if (_descendLoopCount > 0) {
        // we have looped back to the start, prevent this from running again
        return;
    }

    for (uint16_t i = 0; i < _stripLength; i++)
    {
        // If LED is below the current pixel, OR is the current pixel, we light it up
        _pixels[i] = (i < _currentPixel) ? _getLedColor() : CRGB::Black;
    }

    if (_currentPixel == _getFirstPixel()) {
        // We have completed a full loop
        _descendLoopCount++;
    }

    _currentPixel = _getNextPixel();
}

/**
 * Determine which pixel is next, depending on the direction of movement
 */
int FX::_getNextPixel()
{
    // TODO: Add support for an offset/start position
    if (_direction == LIGHTS_REVERSE) {
        _currentPixel--;
        if (_currentPixel < 0) {
            _currentPixel = _getLastPixel();
        }
        return _currentPixel;
    }

    _currentPixel++;
    if (_currentPixel >= _stripLength) {
        // Back to start
        _currentPixel = _getFirstPixel();
    }

    return _currentPixel;
}

CRGB FX::_getLedColor()
{
    return _ledColor.fadeLightBy(255-_brightness);
}

int FX::_getFirstPixel()
{
    // TODO: Add support for an offset/start position
    return 0;
}

int FX::_getLastPixel()
{
    // TODO: Add support for an offset/start position
    return _stripLength - 1;
}

bool FX::_checkTimer()
{
    unsigned long currentMillis = millis();
    if (currentMillis - _previousMillis >= (unsigned long)_speed)
    {
        _previousMillis = currentMillis;
        return true;
    }

    return false;
}
