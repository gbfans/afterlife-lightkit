#include "Control.h"
#include <Bounce2.h>

#define ENABLE_BTN_PIN D7 //Enable lights (ACTIVE HIGH)
#define FIRE_BTN_PIN D0 //Trigger firing (ACTIVE LOW)
#define VENT_BTN_PIN D6 //Trigger venting (ACTIVE LOW
#define CHANGE_BTN_PIN D5 //Change between video game modes (ACTIVE LOW)
#define SHIFT_PIN 15 //Enable logic shifter (ACTIVE HIGH)

void Control::init()
{

  _enableButton = Bounce2::Button();
  _fireButton   = Bounce2::Button();
  _ventButton   = Bounce2::Button();
  _changeButton = Bounce2::Button();

  _enableButton.attach(ENABLE_BTN_PIN, INPUT);
  _enableButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _enableButton.setPressedState(HIGH);     // INDICATE THAT THE HIGH STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
     
  _fireButton.attach(FIRE_BTN_PIN, INPUT);
  _fireButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _fireButton.setPressedState(HIGH);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
    
  _ventButton.attach(VENT_BTN_PIN, INPUT);
  _ventButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _ventButton.setPressedState(HIGH);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  _changeButton.attach(CHANGE_BTN_PIN, INPUT);
  _changeButton.interval(5);     // Use a debounce interval of 5 milliseconds
  _changeButton.setPressedState(HIGH);     // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON

  delay(100);

  _fireButton.update();
  _enableButton.update();
  _ventButton.update();
  _changeButton.update();

  if (_enableButton.isPressed() 
        && _fireButton.isPressed()
        && _ventButton.isPressed()
        && _changeButton.isPressed()) {
    
    _mode = STANDALONE; 
    Serial.println("Standalone Mode");

  }
  else
  {
    _mode = CONTROLLED;
    Serial.println("Controlled Mode");       
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
      _updateStandalone();
    }
    else 
    {
      _updateControlled();    
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
          Serial.println("Start Up");
        }
        else if (_fireButton.pressed())
        {
          //Start firing
          Serial.println("Start Firing");
        }
        else if (_fireButton.released())
        {
          //Stop firing
          Serial.println("End Firing");
        }
        else if (_ventButton.pressed())
        {
          //Start venting
          Serial.println("Vent");
        }
        else if (_changeButton.pressed())
        {
          //Change TVG mode
          Serial.println("Change");
        }

    }
    else if (_enableButton.released())
    {
      //Shutdown
      Serial.println("Shut Down");
    }
  
}

void Control::_updateControlled()
{

  _controllerMode =(_enableButton.isPressed() ? 1 : 0)  + (_fireButton.isPressed() ? 2 : 0) + (_ventButton.isPressed() ? 4 : 0) + (_changeButton.isPressed() ? 8 : 0);
  
  switch (_controllerMode) {
    case 0:
      //Off
      Serial.println("Pack Off");
      break;
    case 1:
      //Power Up
      Serial.println("Power Up");
      break;
    case 2:
      //Red Cyclotron Color (Pack Mode)
      Serial.println("Red Cyclotron");
      break;
    case 3:
      //Green Cyclotron Color (Slime Mode)
      Serial.println("Green Cyclotron");
      break;
    case 4:
      //Blue Cyclotron Color (Stasis Mode)
      Serial.println("Blue Cyclotron");
      break;
    case 5:
      //Orange Cyclotron Color (Meson Mode)
      Serial.println("Orange Cyclotron");
      break;
    case 6:
      //Automatic Venting Mode
      Serial.println("Firing with Automatic Venting");
      break;
    case 7:
      //Change Cyclotron Color
      Serial.println("Change Cyclotron Color");
      //UNSUPPORTED
      break;
    case 8:
      //Normal Operation (Idle)
      Serial.println("Idle Operation");
      break;
    case 9:
      //Venting Strobe Only
      Serial.println("Vent Strobe");
      break;
    case 10:
      //Test Mode
      Serial.println("Test Mode");
      break;
    case 11:
      //Venting Action with Change in Speed
      Serial.println("Firing with change in speed.");
      break;
    case 12:
      //Fire (movie version)
      Serial.println("Firing with no effects.");
      break;
    case 13:
      //Fire (TVG mode with overheat)
      Serial.println("Fire in TVG Mode with Overheating");
      break;
    case 14:
      //Power Down
      Serial.println("Power Down");
      break;
    case 15:
      //Standalone Operation
      Serial.println("Standalone Mode");
      //UNSUPPORTED
      break;
    default:
      Serial.println("No mode detected");
      break;
  }
  
}
