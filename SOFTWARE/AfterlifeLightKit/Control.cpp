#include "Control.h"
#include <Bounce2.h>
#include "Lights.h"

void Control::init(Lights lights)
{

  _lights = lights;

  _enableButton = Bounce2::Button();
  _fireButton   = Bounce2::Button();
  _ventButton   = Bounce2::Button();
  _changeButton = Bounce2::Button();

  _enableButton.attach(ENABLE_BTN_PIN, INPUT_PULLUP);
  _enableButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _enableButton.setPressedState(HIGH);     // INDICATE THAT THE HIGH STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
     
  _fireButton.attach(FIRE_BTN_PIN, INPUT);
  _fireButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _fireButton.setPressedState(HIGH);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
    
  _ventButton.attach(VENT_BTN_PIN, INPUT_PULLUP);
  _ventButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _ventButton.setPressedState(HIGH);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  _changeButton.attach(CHANGE_BTN_PIN, INPUT_PULLUP);
  _changeButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _changeButton.setPressedState(HIGH);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  _fireButton.update();
  _enableButton.update();
  _ventButton.update();
  _changeButton.update();

  if (_enableButton.isPressed() 
        && _fireButton.isPressed()
        && _ventButton.isPressed()
        && _changeButton.isPressed()) {

    _mode = STANDALONE;
    _fireButton.setPressedState(LOW);
    _ventButton.setPressedState(LOW);
    _changeButton.setPressedState(LOW);
    
  }
  else
  {
    _mode = CONTROLLED;        
  }
}

void Control::update()
{

  _fireButton.update();
  _enableButton.update();
  _ventButton.update();
  _changeButton.update();

  if(_fireButton.changed() || _enableButton.changed() || _ventButton.changed() || _changeButton.changed()){
    if(_mode == STANDALONE)
    {
      void _updateStandalone();
    }
    else 
    {
      void _updateControlled();    
    }
  }

}

void Control::_updateStandalone()
{

    if (_enableButton.isPressed())
    {

        if (_enableButton.pressed())
        {
          //Startup
          _lights.setState(START);
        }
        else if (_fireButton.pressed())
        {
          //Start firing
          _lights.setState(FIRING);
        }
        else if (_fireButton.released())
        {
          //Stop firing
          _lights.setState(IDLE);
        }
        else if (_ventButton.pressed())
        {
          //Start venting
          _lights.setState(VENTING);
        }
        else if (_changeButton.pressed())
        {
          //Change TVG mode
          _lights.setMode(SLIME); //Handle check and switch
        }

    }
    else if (_enableButton.released())
    {
      //Shutdown
      _lights.setState(SHUTDOWN);
    }
  
}

void Control::_updateControlled()
{

  _controllerMode =(_enableButton.isPressed() ? 1 : 0)  + (_fireButton.isPressed() ? 2 : 0) + (_ventButton.isPressed() ? 4 : 0) + (_changeButton.isPressed() ? 8 : 0);
  
  switch (_controllerMode) {
    case 0:
      //Off
      break;
    case 1:
      //Power Up
      _lights.setState(START);
      break;
    case 2:
      //Red Cyclotron Color (Pack Mode)
      _lights.setMode(CLASSIC);
      break;
    case 3:
      //Green Cyclotron Color (Slime Mode)
      _lights.setMode(SLIME);
      break;
    case 4:
      //Blue Cyclotron Color (Stasis Mode)
      _lights.setMode(STASIS);
      break;
    case 5:
      //Orange Cyclotron Color (Meson Mode)
      _lights.setMode(MESON);
      break;
    case 6:
      //Automatic Venting Mode
      _lights.setState(FIRING);
      break;
    case 7:
      //Change Cyclotron Color
      //Standalone Mode Only
      //Do Nothing
      break;
    case 8:
      //Normal Operation (Idle)
       _lights.setState(IDLE);
      break;
    case 9:
      //Venting Strobe Only
      break;
    case 10:
      //Test Mode 
      //Do Nothing
      break;
    case 11:
      //Venting
      _lights.setState(VENTING);
      break;
    case 12:
      //Shut Down
      _lights.setState(SHUTDOWN);
      break;
    case 13:
      //Fire (TVG mode with overheat)
      _lights.setState(FIRING);
      break;
    case 14:
      //Power Down
       _lights.setState(INACTIVE);
      break;
    case 15:
      //Standalone Operation
      //UNSUPPORTED
      break;
    default:
      break;
  }
  
}
