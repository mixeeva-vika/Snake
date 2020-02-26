#pragma once
#include"Snake.h"
#include"PrintHelper.h"

class Logic
{
    const char snake_symbol = 219;
    const Point snake_start_position{ 10, 10 };
    Point change(Point p, char c);
public:
    void Run();
};
