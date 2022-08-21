#include "Settings.h"
#include <Bounce2.h>
#include "Lights.h"

#ifndef Control_h
#define Control_h

enum CONTROL_MODES {
    STANDALONE,
    CONTROLLED
};

class Control {
    public:
      void init(Lights lights);
      void update();
    private:
      Bounce2::Button _enableButton;
      Bounce2::Button _fireButton;
      Bounce2::Button _ventButton;
      Bounce2::Button _changeButton;
      CONTROL_MODES _mode;
      Lights _lights;
      void _updateControlled();
      void _updateStandalone();
      int _controllerMode;
    
};

#endif
