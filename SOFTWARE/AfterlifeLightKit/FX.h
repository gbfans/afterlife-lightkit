#include <Ramp.h>
#include "Settings.h"
#define FASTLED_INTERNAL // remove annoying pragma messages
#include "FastLED.h"

#ifndef FX_h
#define FX_h

class FX {
    public:
        // Some kind of initialize
        void init(CRGB *pixels, int stripLength, CRGB ledColor, DIRECTIONS direction, int speed);
        // Set which effect to do
        void setEffect(LIGHT_EFFECTS effect);
        // Change the speed of the animation
        void changeSpeed(unsigned char newSpeed, int delay, ramp_mode rampMode);
        // Call every 1ms to get the next pixel data
        void changeBrightness(unsigned char newSpeed, int delay, ramp_mode rampMode);
        int updateBrightness();
        // Call every 1ms to get the next pixel data
        bool update();
        // Start the effect
        void start();
        // Stop the effect (instantly)
        void stop();
        // Reset the effect states back to the original
        void reset();
    private:
        void _cycling();
        void _spinning();
        void _rainbow();
        void _rainbowScroll();
        void _cylon();
        void _alternate();
        void _fadeall();
        // void _ascend();
        // void _descend();
        // void _ascendDescend();
        // void _fadeIn();
        // void _fadeOut();

        int _getNextPixel();
        bool _checkTimer();

        CRGB *_pixels;
        int _stripLength;
        int _currentPixel = 0;
        int _speed;
        int _brightness;
        unsigned long _previousMillis = 0;
        LIGHT_EFFECTS _effect;
        DIRECTIONS _direction;
        CRGB _ledColor;
        ramp _speedRamp;
        ramp _brightnessRamp;
};

#endif
