#include "MenuController.h"

void MenuController::update(float dt)
{
    if (!isActive)
        return;
    // 1. Опрашиваем ИК-пульт
    ButtonIR btn = IRSensor::getInstance().GetSensorState();

    if (currentMenu->getEditMode())
    {
        if (btn == ButtonUp)
            currentMenu->setAction(PREV);
        else if (btn == ButtonDown)
            currentMenu->setAction(NEXT);
        else if (btn == ButtonOk)
            currentMenu->setAction(OK);
    }
    
    if (!currentMenu->getInfoFlag() && !currentMenu->getEditMode())
    {
        if (btn == ButtonUp)
            currentMenu->prev();
        if (btn == ButtonDown)
            currentMenu->next();

        if (btn == ButtonOk)
        {
            Serial.println("Нажата кнопка ОК!"); // Тест: долетает ли сигнал от ИК?

            int idx = currentMenu->getCurrentIndex();
            MenuItem item = currentMenu->getItem(idx);

            if (item.type == ACTION)
            {
                item.action(); // Выполняем команду
            }
            else if (item.type == SUBMENU)
            {
                if (historyIndex < MAX_DEPTH)
                {
                    menuHistory[historyIndex] = currentMenu; // Запомнили, где стоим
                    historyIndex++;                          // Сдвинули указатель вверх

                    currentMenu = item.nextMenu; // Прыгнули внутрь
                    currentMenu->resetTimer();
                    show();
                }
                else
                {
                    Serial.println("Ошибка: Стек меню переполнен!");
                }
            }
            // else if (item.type == ItemType::NO)
            //     btn = ButtonIR::NOOL;
            else if (item.type == INFO)
            {
                currentMenu->setInfoFlag(true);
                currentMenu->resetTimer();
            }
            else if (item.type == EDIT_ITEM)
                currentMenu->setEditMode(true);
        }
    }
    
    if (btn == ButtonLeft)
    {
        if (currentMenu->getEditMode())
        {
            currentMenu->setEditMode(false);
            return;
        }
        if (currentMenu->getInfoFlag())
        {
            currentMenu->setInfoFlag(false);
            return;
        }
        if (historyIndex > 0)
        {
            historyIndex--;                          // Сдвигаем указатель вниз
            currentMenu = menuHistory[historyIndex]; // Достаем меню из истории
            currentMenu->resetTimer();
        }
        else
        {
            Serial.println("Мы уже в корневом меню, назад идти некуда.");
        }
    }

    // 2. Рисуем то меню, которое сейчас активно
    currentMenu->Draw(dt);
}

void MenuController::back()
{
    if (historyIndex > 0)
    {
        historyIndex--;                          // Сдвигаем указатель вниз
        currentMenu = menuHistory[historyIndex]; // Достаем меню из истории
        currentMenu->resetTimer();
    }
    else
    {
        Serial.println("Мы уже в корневом меню, назад идти некуда.");
    }
}

void MenuController::show()
{
    int header = 0;
    if (currentMenu->IsHeader())
        header = currentMenu->getHeaderSize();
    int availableHeight = 64 - header * currentMenu->getitemHeight();
    Serial.print("availableHeight: ");
    Serial.println(availableHeight);
    Serial.print("Height: ");
    Serial.println(currentMenu->getitemHeight());
    int maxVisibleItems = availableHeight / currentMenu->getitemHeight();
    Serial.print("maxVisibleItems: ");
    Serial.println(maxVisibleItems);
    bool isScrollable = (currentMenu->getItemsCount() > maxVisibleItems);
    currentMenu->setMaxVisibleItems(maxVisibleItems);
    currentMenu->setIsScrollable(isScrollable);
    isActive = true;
}