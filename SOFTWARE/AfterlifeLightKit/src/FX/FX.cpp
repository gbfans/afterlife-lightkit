#include "FX.h"

#define FASTLED_INTERNAL //remove annoying pragma messages
#include "FastLED.h"

void FX::init(CRGB *pixels, int stripLength, CRGB ledColor, DIRECTIONS direction, int speed)
{
    _pixels = pixels;
    _stripLength = stripLength;
    _ledColor = ledColor;
    _direction = direction;
    _speed = _originalSpeed = speed;
    _brightness = _originalBrightness = 50; //temp value

    // Start at default speed
    _speedRamp.go(_speed);
    _brightnessRamp.go(_brightness);
}

void FX::stop()
{
    // Switch all lights off
    setEffect(OFF);

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

void FX::setEffect(LIGHT_EFFECTS effect)
{
    _effect = effect;
}

void FX::changeSpeed(unsigned char newSpeed, int delay, ramp_mode rampMode)
{
    if (newSpeed == _speed)
    {
        return;
    }

    _speed = newSpeed;
    _speedRamp.go(newSpeed, delay, rampMode, ONCEFORWARD);
}

void FX::changeBrightness(unsigned char newBrightness, int delay, ramp_mode rampMode)
{
    if (newBrightness == _brightness) {
        return;
    }

    _brightnessRamp.go(newBrightness, delay, rampMode, ONCEFORWARD);
}

int FX::updateBrightness()
{
      return _brightnessRamp.update();
}

void FX::reset()
{
    // Instantly reset speed/brightness back to original values
    changeSpeed(_originalSpeed, 0, NONE);
    changeBrightness(_originalBrightness, 0, NONE);
}

bool FX::update(bool force)
{
    if (!_checkTimer() && !force) {
        return false;
    }

    _speed = _speedRamp.update();

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
        case ALTERNATE:
            _alternate();
            break;
        case BLINKING:
            _blinking();
            break;
        case FADEIN:
            _fadeIn();
            break;
        case FADEOUT:
            _fadeOut();
            break;
        default:
            Serial.print("Unknown: ");
            Serial.println(_effect);
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
}

void FX::_allOn()
{
    for (uint16_t i = 0; i < _stripLength; i++)
    {
        /**
         * Set all LEDS to on.
         */
        _pixels[i] = _ledColor;
    }
}

void FX::_spinning()
{
    for (uint16_t i = 0; i < _stripLength; i++)
    {
        /**
         * Illuminate LEDs up to and including the current LED,
         * Set the others to black.
         */
        _pixels[i] = (i <= _currentPixel) ? _ledColor : CRGB::Black;
    }

    _currentPixel = _getNextPixel();

    if ((_currentPixel == 0) && (_direction == LIGHTS_FORWARD))
    {
        _direction = LIGHTS_REVERSE;
        _currentPixel = _stripLength-1;
    }
    else if ((_currentPixel == 0) && (_direction == LIGHTS_REVERSE))
    {
        _direction = LIGHTS_FORWARD;
    }
    
}

void FX::_cycling()
{
    for (uint16_t i = 0; i < _stripLength; i++)
    {
        /**
         * Illuminate current LED, set the others to black.
         */
        _pixels[i] = (i == _currentPixel) ? _ledColor : CRGB::Black;
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
    _fadeOut();
    _pixels[_currentPixel].setHue(hue);
    _currentPixel = _getNextPixel();

    if ((_currentPixel == 0) && (_direction == LIGHTS_FORWARD))
    {
        _direction = LIGHTS_REVERSE;
        _currentPixel = _stripLength-1;
    }
    else if ((_currentPixel == 0) && (_direction == LIGHTS_REVERSE))
    {
        _direction = LIGHTS_FORWARD;
    }
    
}

void FX::_alternate()
{
    for (uint16_t i = 0; i < _stripLength; i++) {
        _pixels[i] = (_currentPixel%2 == i%2) ? _ledColor : CRGB::Black;
    }
    _currentPixel = _getNextPixel();
}

/**
 * Set all LEDS on/off
 */
void FX::_blinking()
{

    // Lazy way to alternative on/off states for all LEDS
    _currentPixel = (_currentPixel == 1) ? 0 : 1;
}

/**
 * LED starts at top and drops down to the bottom, gradually filling the strand.
 * (similar to Tetris)
 */
void FX::_tetris()
{
    // To be implemented
}

void FX::_fadeIn()
{
    // To be implemented
}

/*
 * Test function. Supposed to fade out all LEDs slowly.
 */
void FX::_fadeOut()
{
    for(int i = 0; i < _stripLength; i++)
    {
      _pixels[i].nscale8(250);
    }
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
            _currentPixel = _stripLength - 1;
        }
        return _currentPixel;
    }

    _currentPixel++;
    if (_currentPixel >= _stripLength) {
        // Back to start
        _currentPixel = 0;
    }

    return _currentPixel;
}

bool FX::_checkTimer()
{
    if (millis() - _previousMillis >= _speed)
    {
        _previousMillis = millis();
        return true;
    }

    return false;
}
