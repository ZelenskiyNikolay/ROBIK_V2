#include "StateStartV2.h"

StateStartV2::StateStartV2(DisplayOled &disp)
    : display(&disp), sprite(&disp)
{
}
void Null(void) {}
void StateStartV2::enter()
{
    display->clear();
    timer = 0;

    display->drawText("  ROBIK", 0, 0, 2);
    display->drawText("              v2.5+", 0, 20, 1);

    display->drawText("press 1 to Start.", 0, 46, 1);
    display->drawText("press # to menu.", 0, 56, 1);

    menu1 = new Menu(*display);
    menu2 = new Menu(*display);

    menu1->addHeader("Main MENU:", 2);
    menu1->addItem("Sensors test.", menu2);
    menu1->addItem("Settings.", Null);
    menu1->addItem("Exit MENU.", [this](){this->exitMenu();});

    
    menu2->addHeader("  SENSORS TEST:");
    menu2->enableInverseHeader();
    menu2->addItem("Battary sensor.");
    menu2->addItem("Compass sensor.");
    menu2->addItem("Ultrasonic sensor.");
    menu2->addItem("Back MENU.", [this](){back();});

    menu = new MenuController(menu1);
}
void StateStartV2::update(float dt)
{
    if (!menu->IsActive())
    {
        IrLogic();
        Draw(dt);
    }
    else
        menu->update(dt);
}
void StateStartV2::IrLogic()
{

    ButtonIR tmp = IRSensor::getInstance().GetSensorState();
    switch (tmp)
    {
    case Button1:
        EventBus::push({EVENT_CHANGE_STATE, STATE_NORMAL});
        break;
    case ButtonHash:
        timer = 0;
        menu->show();
        break;

    default:
        break;
    }
}
void StateStartV2::Draw(float dt)
{
    timer -= dt;
    if (timer < 0)
    {
        if (!menu->IsActive())
        {
            display->clear();

            display->drawText("  ROBIK", 0, 0, 2);
            display->drawText("              v2.5+", 0, 20, 1);

            display->drawText("press 1 to Start.", 0, 46, 1);
            display->drawText("press # to menu.", 0, 56, 1);
        }
        timer = 1000;
    }
}
StateStartV2::~StateStartV2() {
    // 1. Первым удаляем контроллер, так как он больше не должен управлять экраном
    if (menu != nullptr) {
        delete menu; 
        menu = nullptr;
    }

    // 2. Вторым удаляем само меню, которое было передано в контроллер
    if (menu1 != nullptr) {
        delete menu1;
        menu1 = nullptr;
    }
    if (menu2 != nullptr) {
        delete menu2;
        menu2 = nullptr;
    }
}