#include "MOVE/Map.h"

Map::Map() {}

void Map::RotateLeft90()
{
    switch (_map.dir)
    {
    case SOUTH:
        _map.dir = EAST;
        break;
    case EAST:
        _map.dir = NORTH;
        break;
    case NORTH:
        _map.dir = WEST;
        break;
    case WEST:
        _map.dir = SOUTH;
        break;
    }
}
void Map::RotateRight90()
{
    switch (_map.dir)
    {
    case SOUTH:
        _map.dir = WEST;
        break;
    case EAST:
        _map.dir = SOUTH;
        break;
    case NORTH:
        _map.dir = EAST;
        break;
    case WEST:
        _map.dir = NORTH;
        break;
    }
}
void Map::MovSteep(bool forvard)
{
    int step = forvard ? 1 : -1;
    switch (_map.dir)
    {
    case SOUTH:
        _map.Y += step;
        break;
    case NORTH:
        _map.Y -= step;
        break;
    case EAST:
        _map.X += step;
        break;
    case WEST:
        _map.X -= step;
        break;
    }
}
