#pragma once

enum Direction{
    EAST,
    WEST,
    SOUTH,
    NORTH,
};

struct MAP
{
    long X = 0;
    long Y = 0;
    Direction dir = SOUTH;
};


class Map
{
public:
    static Map &getInstance()
    {
        static Map instance;
        return instance;
    }
    void RotateLeft90();
    void RotateRight90();
    void MovSteep(bool forvard = true);
    MAP GetMap(){return _map;}
private:
    Map();
    MAP _map;
};