#pragma once
#include"Snake.h"
#include"PrintHelper.h"

class Logic
{
    const int hight = 35;
    const int weight = 35;
    const char snake_symbol = 219;
    const Point snake_start_position{ 10, 10 };

    Snake snake;
    Point food;
    Point change(Point p, char c);
    Point generate_food_position();
   
public:
    Logic();
    void Run();
};
