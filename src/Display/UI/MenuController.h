#pragma once
#include "Display/DisplayOled.h"
#include "Sensors/IRSensor.h"
#include "Display/UI/Menu.h"
#include "Display/UI/MenuClick.h"
#include "Module/PowerModule/I2C_Master.h"

class MenuController
{
private:
    Menu *currentMenu;              // Какое меню сейчас на экране
    static const int MAX_DEPTH = 8; // Максимальная вложенность меню
    Menu *menuHistory[MAX_DEPTH];   // Массив-история
    int historyIndex = 0;           // Указатель вершины стека
    bool isActive = false;
    void ClickUpdate(ButtonIR btn);
public:
    MenuController(Menu *root) : currentMenu(root),click() {}
    void update(float dt);
    void back();
    void EditModeBack(){currentMenu->setEditMode(false);}
    bool IsActive(){return isActive;}
    void show();
    void hide(){isActive = false;}
    MenuClick click;
};