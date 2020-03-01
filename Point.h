#pragma once
struct Point
{
    short x;
    short y;

    bool operator==(Point p)
    {
        if ((x == p.x) && (y == p.y))
            return true;
        return false;
    }

};