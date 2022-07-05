#include "FX.h"

#define FASTLED_INTERNAL //remove annoying pragma messages
#include "FastLED.h"

void FX::init(CRGB *pixels, int stripLength, CRGB ledColor, DIRECTIONS direction, int speed)
{
    _pixels = pixels;
    _stripLength = stripLength;
    _ledColor = ledColor;
    _direction = direction;
    _speed = speed;
    _brightness = 50; //temp value

    // Start at default speed
    _speedRamp.go(_speed);
    _brightnessRamp.go(_brightness);
}

void FX::setEffect(LIGHT_EFFECTS effect)
{
    _effect = effect;
}

void FX::changeSpeed(unsigned char newSpeed, int delay, ramp_mode rampMode)
{
    _speedRamp.go(newSpeed, delay, rampMode, ONCEFORWARD);
}

void FX::changeBrightness(unsigned char newBrightness, int delay, ramp_mode rampMode)
{
    _brightnessRamp.go(newBrightness, delay, rampMode, ONCEFORWARD);
}

int FX::updateBrightness()
{
      return _brightnessRamp.update();
}

bool FX::update()
{
    if (!_checkTimer()) {
        return false;
    }

    _speed = _speedRamp.update();

    /*
     *  If speed is equal to 255, movement should probably just be stopped and LEDs off.
     */

    switch (_effect)
    {
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
        default:
            Serial.print("Unknown: ");
            Serial.println(_effect);
            break;
    }

    return true;
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
    _fadeall();
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

/*
 * Test function. Supposed to fade out LEDs slowly.
 */
void FX::_fadeall()
{
    for(int i = 0; i < _stripLength; i++)
    {
      _pixels[i].nscale8(225);
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
