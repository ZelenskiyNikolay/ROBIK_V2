#include "StateNormal.h"

StateNormal::StateNormal(DisplayOled &disp)
    : display(&disp), sprite(&disp)
{
}

void StateNormal::enter()
{
  display->clear();
  timer = 0;
  isDrawingBattery = true;
  //sound.SoundStop();
}
void StateNormal::update(float dt)
{
  IrLogic();
  Draw(dt);
  
}


void StateNormal::IrLogic()
{
    ButtonIR tmp = IRSensor::getInstance().GetSensorState();
    switch (tmp)
    {
    case Button1:
        SoundManager::getInstance().Play("Sound/Hello/Dima.wav");
        break;
    case Button2:
        SoundManager::getInstance().Play("Sound/Hello/Aliona.wav");
        break;
    case Button3:
        SoundManager::getInstance().Play("Sound/Hello/Mama.wav");
        break;
    case ButtonHash:
        EventBus::push({EVENT_CHANGE_STATE, STATE_START});
        break;
    

    case ButtonUp:
        MovementModule::getInstance().NewMov(MotionState::FORWARD,0.5f,0.5f);
        break;
    case ButtonDown:
        MovementModule::getInstance().NewMov(MotionState::BACKWARD,0.5f,0.5f);
        break;
    case ButtonLeft:
        MovementModule::getInstance().NewMov(MotionState::TURN_LEFT90);
        break;
    case ButtonRight:
        MovementModule::getInstance().NewMov(MotionState::TURN_RIGHT90);
        break;

    default:
        break;
    }
}

void StateNormal::Draw(float dt)
{
  timer -= dt;
  if (IsOpen)
  {
    if (timer < 0)
    {
      IsOpen = false;
      timer = Close_Eyes;
      sprite.Draw(Emotions::BLINK);
      return;
    }
  }
  else
  {
    if (timer < 0)
    {
      IsOpen = true;
      timer = Open_Eyes;

      sprite.Draw(Emotions::NORMAL);
      return;
    }
  }
}

