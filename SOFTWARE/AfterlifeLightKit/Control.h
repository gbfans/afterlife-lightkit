#include "Settings.h"
#include <Bounce2.h>

#ifndef Control_h
#define Control_h

enum CONTROL_MODES {
    STANDALONE,
    CONTROLLED
};

class Control {
    public:
      void init();
      void update();
    private:
      Bounce2::Button _enableButton;
      Bounce2::Button _fireButton;
      Bounce2::Button _ventButton;
      Bounce2::Button _changeButton;
      CONTROL_MODES _mode;
      void _updateControlled();
      void _updateStandalone();
      int _controllerMode;
    
};

#endif
