#pragma once
#include <Arduino.h>
#include "Display/DisplayOled.h"
#include "Sprites.h"

#define MINI_SPRITE_SIZE 4
#define BIG_SPRITE_SIZE 8

class Symbol
{
private:
    int _positionY;
    int _speed;
    bool _visible;
    unsigned char *_sprite;
    int _size;
    DisplayOled *disp;

public:
    Symbol() {}
    bool Is_Visible() { return _visible; }
    void Init(int PosY, int Speed, uint8_t *bitmap, DisplayOled &display, int size = MINI_SPRITE_SIZE)
    {
        disp = &display;
        _positionY = PosY;
        _speed = Speed;
        _sprite = bitmap;
        _visible = true;
        _size = size;
    }
    void SymbolRefresh(int PosY, int Speed)
    {
        _positionY = PosY;
        _speed = Speed;
        bool b = (random(1, 10) % 2);
        if (_speed < 6)
        {
            if (b)
                _sprite = Zero_Mini_bitmap;
            else
                _sprite = One_Mini_bitmap;
            _size = MINI_SPRITE_SIZE;
        }
        else
        {
            if (b)
                _sprite = Zero_bitmap;
            else
                _sprite = One_bitmap;
            _size = BIG_SPRITE_SIZE;
        }
        _visible = true;
    }
    bool PrintSymbol(int X)
    {
        if (!_visible || disp == nullptr || _sprite == nullptr)
            return false;
        if (_visible)
        {
            disp->drawBitmap(X, _positionY, _sprite, _size, _size, SSD1306_WHITE);
            return true;
        }
        else
            return false;
    }
    void MovSymbbol() { _positionY += _speed; }
    bool ClipSymol(int ClipLine)
    {
        if (_positionY > ClipLine)
            _visible = false;
        return _visible;
    }
};