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

    // Start at default speed
    _speedRamp.go(_speed);
}

void FX::setEffect(LIGHT_EFFECTS effect)
{
    _effect = effect;
}

void FX::changeSpeed(int newSpeed, int delay, ramp_mode rampMode)
{
    _speedRamp.go(newSpeed, delay, rampMode, ONCEFORWARD);
}

bool FX::update()
{
    if (!_checkTimer()) {
        return false;
    }

    _speed = _speedRamp.update();

    switch (_effect)
    {
        case CYCLING:
            _cycling();
            break;
        case SPINNING:
            _spinning();
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
