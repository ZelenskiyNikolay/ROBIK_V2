#include "Menu.h"

Menu::Menu()
{
    count = 0;
    currentIndex = 0;
    itemHeight = 10;
    itemTextSize = 1;
    cursorBitmap = nullptr;
    isBitmapCursor = false;
    isFrameMode = false;
    isInverse = false;
    isHeaderMenu = false;
    titleSize = 0;
    isInverseHeader = false;
    isScrollable = false;
    topIndex = 0;
    maxVisibleItems = 4;
    sliderHeight = 10;
    display = nullptr;
    action = NO_ACTION;
}
Menu::Menu(DisplayOled &disp) : display(&disp) {}

void Menu::Clear()
{
    count = 0;          // Обнуляем количество элементов (старые данные просто перезапишутся)
    currentIndex = 0;   // Сбрасываем курсор в начало
    topIndex = 0;       // Сбрасываем скролл
    isHeaderMenu = false; // Если новый экран будет без хедера
    
    // На всякий случай зануляем экшены std::function, чтобы освободить ресурсы
    for (int i = 0; i < MAX_MENU_ITEMS; i++)
    {
        items[i].title = nullptr;
        items[i].action = nullptr;
        items[i].nextMenu = nullptr;
        items[i].type = NO;
    }
}

void Menu::next()
{
    if (isScrollable)
    {
        // Если уже на самом последнем элементе — просто игнорируем нажатие
        if (currentIndex >= count - 1)
            return;

        currentIndex++;

        // Считаем, сколько максимум пунктов влезает на экран
        // maxVisibleItems = (64 - (isHeaderMenu ? headerHeight : 0)) / itemHeight;
        if (currentIndex >= topIndex + maxVisibleItems)
        {
            topIndex++; // Окно просмотра смещается вниз
        }
        timer = 0; // Перерисовка
    }
    else
    {
        if (count == 0)
            return;
        currentIndex = (currentIndex + 1) % count;
        timer = 0; // перерисовка
    }
}

void Menu::prev()
{
    if (isScrollable)
    {
        // Если уже на самом первом элементе (в самом верху) — игнорируем
        if (currentIndex <= 0)
            return;

        currentIndex--;

        // Если курсор поднялся выше, чем первая видимая строка на экране
        if (currentIndex < topIndex)
        {
            topIndex--; // Окно просмотра смещается вверх
        }
        timer = 0; // Перерисовка
    }
    else
    {
        if (count == 0)
            return;
        currentIndex = (currentIndex - 1 + count) % count;
        timer = 0; // перерисовка
    }
}

bool Menu::addItem(const char *title, Menu *nextMenu)
{
    if (count >= MAX_MENU_ITEMS)
    {
        // Массив полон
        return false;
    }
    items[count] = {title};
    if (nextMenu != nullptr)
    {
        items[count].type = SUBMENU;
        items[count].nextMenu = nextMenu;
    }
    else
        items[count].type = NO;
    count++;
    return true;
}
bool Menu::addItem(const char *title, std::function<void()> action)
{
    if (count >= MAX_MENU_ITEMS)
    {
        // Массив полон
        return false;
    }
    items[count].title = {title};
    if (action != nullptr)
    {
        items[count].action = {action};
        items[count].type = ACTION;
    }
    else
        items[count].type = NO;
    count++;
    return true;
}
bool Menu::addItem(const char *title, std::function<void()> action, ItemType type)
{
    if (count >= MAX_MENU_ITEMS)
    {
        // Массив полон
        return false;
    }
    items[count].title = {title};
    if (action != nullptr)
    {
        items[count].action = {action};
        items[count].type = type;
    }
    else
        items[count].type = NO;
    count++;
    return true;
}
void Menu::ProcessDraw(int i)
{
    int row = header + i * itemTextSize;
    if (isScrollable)
        row -= topIndex;
    if (i == currentIndex)
    {
        if (isEditMode)
        {
            items[currentIndex].action();
            if (isEditModeTitel)
                sprintf(buffer, ">%s %s", items[i].title, editModeEmpty); // Активный пункт
            else
                sprintf(buffer, "> %s", editModeEmpty); // Активный пункт
        }
        else
        {
            if (isBitmapCursor)
                sprintf(buffer, "  %s ", items[i].title); // Активный пункт
            else
                sprintf(buffer, "%s%s ", cursorSign, items[i].title); // Активный пункт
            if (isFrameMode)
            {
                int pointY = (row * 10) - 2;
                display->drawRect(0, pointY, screenWidth, itemHeight * itemTextSize, SSD1306_WHITE);
            }
            else if (isInverse)
            {
                int pointY = (row * 10) - 2;
                display->fillRect(0, pointY, screenWidth, itemHeight * itemTextSize, SSD1306_WHITE);
            }
        }
    }
    else
        sprintf(buffer, "%s%s ", cursorEmpty, items[i].title); // Активный пункт

    if (isInverse && i == currentIndex)
        display->drawText(buffer, 0, row * 10, itemTextSize, SSD1306_BLACK);
    else
        display->drawText(buffer, 0, row * 10, itemTextSize);
    if (isBitmapCursor && i == currentIndex)
        display->drawBitmap(0, (row * 10) - 1, cursorBitmap, cursorBitmapX, cursorBitmapY, SSD1306_WHITE);
    else if (isBitmapCursor && noCursorBitmap != nullptr)
        display->drawBitmap(0, (row * 10) - 1, noCursorBitmap, noCursorBitmapX, noCursorBitmapY, SSD1306_WHITE);
}
void Menu::Draw(float dt)
{
    if (isInfoSelected)
    {
        items[currentIndex].action();
        return;
    }

    timer -= dt;
    if (timer > 0)
        return;
    timer = UpDraw;

    display->clear();

    if (isHeaderMenu && headerTitle != nullptr)
    {
        sprintf(buffer, "%s ", headerTitle); // Заголовок, если он есть
        header = 1 * titleSize;

        if (isInverseHeader)
        {
            display->fillRect(0, 0, screenWidth, header * itemHeight - 1, SSD1306_WHITE);
            display->drawText(buffer, 1, 1, titleSize, SSD1306_BLACK);
        }
        else
            display->drawText(buffer, 0, 0, titleSize);
    }

    if (isScrollable)
    {
        int renderedLines = 0;
        for (int i = topIndex; i < count; i++)
        {
            if (renderedLines >= maxVisibleItems + 1)
                break; // Экран кончился, хватит рисовать
            ProcessDraw(i);
            renderedLines++;
        }
        display->fillRect(screenWidth - 9, header * itemHeight, 10, 64 - header * itemHeight, SSD1306_BLACK);
        display->fillTriangle(screenWidth - 8, header * itemHeight + 2, screenWidth - 2,
                              header * itemHeight + 2, screenWidth - 5, header * itemHeight + 5, SSD1306_WHITE);
        display->fillTriangle(screenWidth - 8, 64 - 2, screenWidth - 2,
                              64 - 2, screenWidth - 5, 64 - 5, SSD1306_WHITE);
        display->drawLine(screenWidth - 5, header * itemHeight + 5, screenWidth - 5,
                          64 - 5, SSD1306_WHITE);
        int headerY = 0;
        if (isHeaderMenu)
            headerY = header * itemHeight;
        int startY = headerY + 5;                             // Стартовая точка (низ верхнего треугольника)
        int trackHeight = ((64 - 5) - startY) - sliderHeight; // Полная длина рельсы

        int sliderY = startY; // По умолчанию в самом верху

        // Защита: двигаем только если есть куда мотать (count > maxVisibleItems)
        if (count > 1)
        {
            sliderY = startY + (trackHeight * currentIndex) / (count - 1);
        }
        display->fillRect(screenWidth - 6, sliderY, 3, sliderHeight, SSD1306_WHITE);
    }
    else
    {
        for (size_t i = 0; i < count; i++)
            ProcessDraw(i);
    }
}

void Menu::setCursor(const char *sign, const char *empty)
{
    cursorSign = sign;

    if (empty == nullptr)
    {
        // 1. Считаем длину новой стрелки
        size_t len = strlen(sign);

        // Предохранитель, чтобы не выйти за рамки нашего буфера пробелов
        if (len > 7)
            len = 7;

        // 2. Забиваем буфер пробелами
        for (size_t i = 0; i < len; i++)
        {
            cursorEmpty[i] = ' ';
        }
        cursorEmpty[len] = '\0'; // Не забываем про проклятый нуль-терминатор!
    }
    else
    {
        size_t len = strlen(empty);
        if (len > 7)
            len = 7;

        for (size_t i = 0; i < len; i++)
        {
            cursorEmpty[i] = empty[i];
        }
        cursorEmpty[len] = '\0'; // Не забываем про проклятый нуль-терминатор!
    }
}