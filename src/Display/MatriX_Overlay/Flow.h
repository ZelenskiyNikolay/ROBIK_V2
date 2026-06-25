#pragma once
#include <Arduino.h>
#include "Symbol.h"
#include "Sprites.h"

#define MAX_SYMBOLS 20
class Flow
{
private:
    int _flowDelay;
    int _positionX;
    int _collSymbols;
    Symbol _symbols[MAX_SYMBOLS];

public:
    Flow() :_symbols(){}
    int CollRenderSymbol() { return _collSymbols; }
    int Delay() { return _flowDelay; }
    void Init(int DelayMow, int PosX, int CollSymbols, DisplayOled &display)
    {
        _flowDelay = DelayMow;
        _positionX = PosX;
        _collSymbols = CollSymbols;
        for (int i = 0; i < MAX_SYMBOLS; i++)
        {
            bool b = (random(1, 10) % 2);
            if (b)
                _symbols[i].Init(-8, random(3, 10), Zero_Mini_bitmap, display);
            else
                _symbols[i].Init(-8, random(3, 10), One_Mini_bitmap, display);
        }
    }
    void DelayInc() { _flowDelay--; }
    void FlowRefresh(int DelayMow, int PosX)
    {
        _flowDelay = DelayMow;
        _positionX = PosX;
        _collSymbols = random(1,9);

        for (int i = 0; i < _collSymbols; i++)
            _symbols[i].SymbolRefresh(-8, random(2, 10));
    }
    void PrintFlow()
    {
        for (int i = 0; i < _collSymbols; i++)
            _symbols[i].PrintSymbol(_positionX);
    }
    void MovSymbols()
    {
        for (int i = 0; i < _collSymbols; i++)
            _symbols[i].MovSymbbol();
    }
    bool ClipFlow(int ClipLineY)
    {
        int noClipColl = 0;
        for (int i = 0; i < _collSymbols; i++)
        {
            if (_symbols[i].ClipSymol(ClipLineY))
                noClipColl++;
        }
        if (noClipColl == 0)
            return true;
        else
            return false;
    }
};