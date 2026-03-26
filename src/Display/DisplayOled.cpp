#include "DisplayOled.h"
#include <Adafruit_GFX.h>

DisplayOled::DisplayOled(Adafruit_SSD1306 &d)
    : display(&d)
{
}

void DisplayOled::clear()
{
    display->clearDisplay();
}

void DisplayOled::drawText(const char *text, int x, int y, int size)
{
    display->setTextColor(SSD1306_WHITE);
    display->setTextSize(size);
    display->setCursor(x, y);
    display->print(text);
    NeedUpdate = true;
}

void DisplayOled::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    display->drawLine(x0, y0, x1, y1, color);
    NeedUpdate = true;
}
void DisplayOled::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    display->fillCircle(x0, y0, r, color);
    NeedUpdate = true;
}
void DisplayOled::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    display->drawCircle(x0, y0, r, color);
    NeedUpdate = true;
}
void DisplayOled::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    display->drawRect(x, y, w, h, color);
    NeedUpdate = true;
}
void DisplayOled::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    display->fillRect(x, y, w, h, color);
    NeedUpdate = true;
}
void DisplayOled::setTextSize(uint8_t size)
{
    display->setTextSize(size);
}
void DisplayOled::setTextColor(uint16_t color)
{
    display->setTextColor(color);
}
void DisplayOled::setCursor(int16_t x, int16_t y)
{
    display->setTextColor(x,y);
}
void DisplayOled::println(char str)
{
    display->println(str);
}
void DisplayOled::update()
{
    if (NeedUpdate)
    {
        display->display();
        NeedUpdate = false;
    }
}
