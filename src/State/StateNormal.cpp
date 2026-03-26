#include "StateNormal.h"

StateNormal::StateNormal(DisplayOled &disp)
    : display(&disp), sprite(&disp)//, sound(12), ir(A7)
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
  Draw(dt);
  
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