#pragma once
#include"Snake.h"
#include"PrintHelper.h"

class Logic
{
    const short hight = 25;
    const short weight = 45;
    const char snake_symbol = 219;
    const char food_symbol = '*';
    const Point snake_start_position{ 10, 10 };

    Snake snake;
    Point food;
    PrintHelper pr;

    Point change(Point p, char c);
    Point generate_food_position();
    void draw_a_field();
    char generate_new_direction();
   
public:
    Logic();
    void Run();
};
