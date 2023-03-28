#include "Arduino.h"

#ifndef Control_h
#define Control_h

enum CONTROL_MODES {
    MODE_STANDALONE,
    MODE_CONTROLLED
};

enum CONTROL_STATES {
  STATE_OFF, // 0: Off
  STATE_POWER_UP, // 1: Power Up
  STATE_RED_CYCLOTRON, // 2: Red Cyclotron Color
  STATE_GREEN_CYCLOTRON, // 3: Green Cyclotron Color
  STATE_BLUE_CYCLOTRON, // 4: Blue Cyclotron Color
  STATE_ORANGE_CYCLOTRON, // 5: Orange Cyclotron Color
  STATE_AUTOMATIC_VENTING, // 6: Automatic Venting Mode
  STATE_CHANGE_CYCLOTRON_COLOR, // 7: Change Cyclotron Colors (Standalone mode only)
  STATE_IDLE, // 8: Normal Operation (Idle) (both modes)
  STATE_VENT_STROBE, // 9: Venting Strobe Only
  STATE_TEST_MODE, // 10: Test Mode
  STATE_VENTING_ACTION, // 11: Venting Action with Interaction (both modes)
  STATE_FIRE_MOVIE, // 12: Fire (movie version)
  STATE_FIRE_TVG, // 13: Fire (TVG, overheat version) (Both modes)
  STATE_POWER_DOWN, // 14: Power Down (both modes)
  STATE_STANDALONE, // 15: Delay -> Power Up -> Idle (Standalone mode only)
  STATE_NONE, // 16: None (ie State has not yet been read)
};

class GBFansControl {
    public:
      void init();
      void update();
      bool changed();
      unsigned long duration();
      bool isMode(CONTROL_MODES mode);
      bool isState(CONTROL_STATES state);
      bool wasState(CONTROL_STATES state);
      CONTROL_MODES getMode();
      CONTROL_STATES getCurrentState();
      CONTROL_STATES getPreviousState();
    private:
      CONTROL_MODES _mode;
      CONTROL_STATES _currentState;
      CONTROL_STATES _previousState;
      bool _isChanged = false;
      int _controllerMode;
      unsigned long _lastDebounceTime = 0;
      unsigned long _currentMillis = 0;
      int _lastInputFrame = 0;
      int _inputFramesReceived = 0;
};

#endif
