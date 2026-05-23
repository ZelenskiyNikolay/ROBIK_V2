#pragma once

#include "Display/DisplayOled.h"
#include "Sensors/IRSensor.h"
#include "Menu.h"

class MenuController
{
private:
    Menu *currentMenu;              // Какое меню сейчас на экране
    static const int MAX_DEPTH = 8; // Максимальная вложенность меню
    Menu *menuHistory[MAX_DEPTH];   // Массив-история
    int historyIndex = 0;           // Указатель вершины стека
    bool isActive = false;
public:
    MenuController(Menu *root) : currentMenu(root) {}
    void update(float dt);
    void back();
    bool IsActive(){return isActive;}
    void show();
    void hide(){isActive = false;}
};