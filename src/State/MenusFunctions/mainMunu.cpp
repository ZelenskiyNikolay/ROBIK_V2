#include "../StateStartV2.h"

void StateStartV2::exitMenu()
{
    StateStartV2::menu->hide();
    timer = 0;
}
void StateStartV2::back()
{
  menu->back();
}