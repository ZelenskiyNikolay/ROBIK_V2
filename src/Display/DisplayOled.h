#pragma once
#include <Adafruit_SSD1306.h>

class DisplayOled {
public:
    DisplayOled(Adafruit_SSD1306& d);
    bool NeedUpdate;

    void drawText(const char* text, int x, int y, int size = 1);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void setTextColor(uint16_t color);
    void setTextSize(uint8_t size);
    void setCursor(int16_t x, int16_t y);
    void println(char str);
    void setContrast(uint8_t value);

    void clear();
    void update();

private:
    Adafruit_SSD1306* display;
};