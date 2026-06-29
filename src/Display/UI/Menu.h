#pragma once
#include <functional>
#include "Display/DisplayOled.h"
#include "Sensors/IRSensor.h"

// Ограничение: максимум 20 пунктов в одном меню
#define MAX_MENU_ITEMS 20

enum Action
{
    NO_ACTION,
    NEXT,
    PREV,
    OK
};
enum ItemType
{
    NO,
    ACTION,
    SUBMENU,
    INFO,
    EDIT_ITEM
};
class Menu; // Такой класс существует
// Структура одного пункта меню
struct MenuItem
{
    const char *title; // Название пункта
    ItemType type;

    Menu *nextMenu = nullptr;               // Сразу даем дефолтное значение
    std::function<void()> action = nullptr; // Сразу даем дефолтное значение
};

class Menu
{
public:
    Menu();
    Menu(DisplayOled &disp);
    void SetDisplay(DisplayOled &disp) { display = &disp; }
    void Draw(float dt);
    void next();
    void prev();
    bool addItem(const char *title, Menu *nextMenu = nullptr);
    bool addItem(const char *title, std::function<void()> action); // void (*action)() = nullptr);
    bool addItem(const char *title, std::function<void()> action, ItemType type);
    void addHeader(const char *Title, int size = 1)
    {
        headerTitle = Title;
        isHeaderMenu = true;
        titleSize = size;
    }
    void setCursor(const char *sign, const char *empty = nullptr);
    void setCursor(unsigned char *bitmap, unsigned char *empty = nullptr)
    {
        cursorBitmap = bitmap;
        isBitmapCursor = true;
        if (empty != nullptr)
            noCursorBitmap = empty;
    }

    void enableFrame() { isFrameMode = true; }
    void enableInverse() { isInverse = true; }
    void enableInverseHeader() { isInverseHeader = true; }

    void resetTimer(float val = 0) { timer = val; }
    int getCurrentIndex() { return currentIndex; }
    MenuItem getItem(int index) { return items[index]; }

    int getItemsCount() { return count; }
    void setMaxVisibleItems(int val)
    {
        maxVisibleItems = val;
        int trackHeight = (64 - 5) - (titleSize * itemHeight + 5);
        if (count > 0)
            sliderHeight = (trackHeight * maxVisibleItems) / count;
        else
            sliderHeight = trackHeight;
    }
    void setIsScrollable(bool val) { isScrollable = val; }
    int getHeaderSize() { return titleSize; }
    int getitemHeight() { return itemHeight; }
    void setItemHeight(int val) { itemHeight = val; }
    void setItemsSize(int size) { itemTextSize = size; }
    bool IsHeader() { return isHeaderMenu; }
    void setCursorBitmapSize(int X = 8, int Y = 10)
    {
        cursorBitmapX = X;
        cursorBitmapY = Y;
    }
    void setNoCursorBitmapSize(int X = 8, int Y = 10)
    {
        noCursorBitmapX = X;
        noCursorBitmapY = Y;
    }
    void setInfoFlag(bool info) { isInfoSelected = info; }
    bool getInfoFlag() { return isInfoSelected; }

    void setEditMode(bool EditMode) { isEditMode = EditMode; }
    bool getEditMode() { return isEditMode; }

    void setEditModeEmpty(const char *Val, bool Is_Title_Print = true)
    {
        sprintf(editModeEmpty, "%s", Val);
        isEditModeTitel = Is_Title_Print;
    }
    Action getAction()
    {
        Action tmp = action;
        action = NO_ACTION;
        return tmp;
    }
    void setAction(Action a) { action = a; }
    void Clear();
private:
    void ProcessDraw(int i);
    MenuItem items[MAX_MENU_ITEMS]; // Статический массив в памяти класса (без new/delete)
    size_t count = 0;               // Текущее количество добавленных пунктов
    int currentIndex = 0;           // Выбранный сейчас пункт
    float timer;
    const float UpDraw = 1000;

    int itemHeight = 10; // Высота строки
    int itemTextSize = 1;
    unsigned char *cursorBitmap = nullptr;
    int cursorBitmapX = 8;
    int cursorBitmapY = 10;
    unsigned char *noCursorBitmap = nullptr;
    int noCursorBitmapX = 8;
    int noCursorBitmapY = 10;
    bool isBitmapCursor = false;
    const char *cursorSign = "->";
    char cursorEmpty[8] = "  ";
    bool isFrameMode = false; // По умолчанию рамочки нет
    bool isInverse = false;   // инверсный режим отображения выбора

    bool isHeaderMenu = false;    // Меню с заголовком или без
    const char *headerTitle;      // Хранит заголовок
    int titleSize = 0;            // Размер заголовка
    bool isInverseHeader = false; // инверсный режим отображения заголовка

    bool isScrollable = false; // Меню прокручивается
    int topIndex = 0;
    int maxVisibleItems;
    int sliderHeight;

    char buffer[64];
    int header = 0;
    int screenWidth = 127; // Ширина OLED дисплея

    bool isInfoSelected = false;
    bool isEditMode = false;
    bool isEditModeTitel = true;
    char editModeEmpty[32] = ">TEST<";
    Action action = NO_ACTION;

    DisplayOled *display;
};