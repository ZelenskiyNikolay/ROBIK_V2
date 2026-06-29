#pragma once
#include "Display/DisplayOled.h"
#include "Sensors/IRSensor.h"
#include "Menu.h"
#include "Display/UI/MenuClick.h"
#include "Module/PowerModule/I2C_Master.h"

#define MENU_MEMORY_SIZE 10
class MenuController
{
private:
    MenuController(){}
    Menu *currentMenu;              // Какое меню сейчас на экране
    static const int MAX_DEPTH = 10; // Максимальная вложенность меню
    Menu *menuHistory[MAX_DEPTH];   // Массив-история
    int historyIndex = 0;           // Указатель вершины стека
    bool isActive = false;
    void ClickUpdate(ButtonIR btn);

    DisplayOled *display;
public:
    static MenuController &getInstance()
    {
        static MenuController instance;
        return instance;
    }

    Menu Menus[MENU_MEMORY_SIZE];
    void SetCurentMenu(int index){currentMenu = &Menus[index];}
    Menu* GetCurentMenu(){return currentMenu;}
    void Init(DisplayOled &disp);
    void update(float dt);
    void back();
    void EditModeBack(){currentMenu->setEditMode(false);}
    bool IsActive(){return isActive;}
    void show();
    void hide(){isActive = false;}
    MenuClick click;
    bool IsInit = false;
    Action getAction(){if (currentMenu) return currentMenu->getAction();return NO_ACTION;}
    void setEditModeEmpty(const char *Val, bool Is_Title_Print = true)
    {if (currentMenu) currentMenu->setEditModeEmpty(Val, Is_Title_Print);}
    void setEditMode(bool EditMod){if (currentMenu) currentMenu->setEditMode(EditMod);}
    void resetTimer(float val = 0){currentMenu->resetTimer(val);}
    void ClearAll();
    void Clear(int index){Menus[index].Clear();}
};