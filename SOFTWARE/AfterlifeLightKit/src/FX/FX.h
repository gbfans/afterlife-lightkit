#include <Ramp.h>
#include "../../Settings.h"
#define FASTLED_INTERNAL // remove annoying pragma messages
#include "FastLED.h"

#ifndef FX_h
#define FX_h

class FX {
    public:
        // Some kind of initialize
        void init(CRGB *pixels, int stripLength, CRGB ledColor, DIRECTIONS direction, int speed);
        // Set which effect to do
        void setEffect(LIGHT_EFFECTS effect, bool reset = false);
        // Set the direction (which will also reset the current pixel)
        void setReverse(bool isReverse = true);
        // Set the Speed (instantly) of the animation
        void changeSpeed(int newSpeed);
        // Gradually change the speed of the animation
        void changeSpeed(int newSpeed, int delay, ramp_mode rampMode);
        // Set the brightness (instantly) of the animation
        void changeBrightness(int newBrightness);
        // Get the current Brightness (used by FastLED directly)
        int updateBrightness();
        // Call every 1ms to get the next pixel data
        bool update(bool force = false);
        // Stop the effect (instantly switch off all LEDs)
        void stop();
        // Force all LEDs on
        void allOn();
        // Reset the effect states back to the original
        void reset();
    private:
        void _clear();
        void _allOn();
        void _cycling();
        void _spinning();
        void _rainbow();
        void _rainbowScroll();
        void _cylon();
        void _alternate();
        void _blinking();
        void _tetris();
        void _descend();
        // void _ascend();
        // void _ascendDescend();


        // Get the next Pixel in the sequence
        int _getNextPixel();
        // Get the last Pixel (typically the last in the strip)
        int _getLastPixel();
        // Get the first Pixel (typically the first in the strip)
        int _getFirstPixel();
        // Check whether the timer has elapsed
        bool _checkTimer();

        // Return the current LED Colour with brightness applied
        CRGB _getLedColor();

        CRGB *_pixels;
        int _stripLength;
        int _currentPixel = 0;
        int _speed;
        int _originalSpeed;
        int _brightness;
        int _originalBrightness;
        unsigned long _previousMillis = 0;
        LIGHT_EFFECTS _effect;
        DIRECTIONS _direction;
        CRGB _ledColor;
        rampInt _speedRamp;

        /**
         * Animation-specific variables
         */
         // Used for the TETRIS animation to track how many LEDs are filled
         int _tetrisProgress = 0;
         // Used for the DESCEND animation to keep track of how many loops have occurred
         int _descendLoopCount = 0;
};

#endif
