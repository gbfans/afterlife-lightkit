#include "Control.h"
#include <Bounce2.h>

void Control::init()
{

  _enableButton = Bounce2::Button();
  _fireButton   = Bounce2::Button();
  _ventButton   = Bounce2::Button();
  _changeButton = Bounce2::Button();

  _enableButton.attach(ENABLE_BTN_PIN, INPUT);
  _enableButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _enableButton.setPressedState(LOW);     // INDICATE THAT THE HIGH STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
     
  _fireButton.attach(FIRE_BTN_PIN, INPUT);
  _fireButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _fireButton.setPressedState(LOW);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
    
  _ventButton.attach(VENT_BTN_PIN, INPUT);
  _ventButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _ventButton.setPressedState(LOW);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  _changeButton.attach(CHANGE_BTN_PIN, INPUT);
  _changeButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _changeButton.setPressedState(LOW);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  _fireButton.update();
  _enableButton.update();
  _ventButton.update();
  _changeButton.update();

  if (_enableButton.isPressed() 
        && _fireButton.isPressed()
        && _ventButton.isPressed()
        && _changeButton.isPressed()) {

    _mode = STANDALONE;
          
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
        }
        else if (_fireButton.pressed())
        {
          //Start firing
        }
        else if (_fireButton.released())
        {
          //Stop firing
        }
        else if (_ventButton.pressed())
        {
          //Start venting
        }
        else if (_changeButton.pressed())
        {
          //Change TVG mode
        }

    }
    else if (_enableButton.released())
    {
      //Shutdown
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
      break;
    case 2:
      //Red Cyclotron Color (Pack Mode)
      break;
    case 3:
      //Green Cyclotron Color (Slime Mode)
      break;
    case 4:
      //Blue Cyclotron Color (Stasis Mode)
      break;
    case 5:
      //Orange Cyclotron Color (Meson Mode)
      break;
    case 6:
      //Automatic Venting Mode
      break;
    case 7:
      //Change Cyclotron Color
      //UNSUPPORTED
      break;
    case 8:
      //Normal Operation (Idle)
      break;
    case 9:
      //Venting Strobe Only
      break;
    case 10:
      //Test Mode
      break;
    case 11:
      //Venting Action with Change in Speed
      break;
    case 12:
      //Fire (movie version)
      break;
    case 13:
      //Fire (TVG mode with overheat)
      break;
    case 14:
      //Power Down
      break;
    case 15:
      //Standalone Operation
      //UNSUPPORTED
      break;
    default:
      break;
  }
  
}
