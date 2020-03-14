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
    const Point game_over_point_position1{ weight / 2 - 5, hight / 2 };
    const string game_over1 = "Game Over";
    const Point game_over_point_position2{ weight / 2 - 9, hight/2 + 2};
    const string game_over2 = "Press Enter to exit";
    const int snake_size_for_win = 10;
    const Point win_point_position = { weight / 2 - 15, hight / 2 };
    const string win = "Congratulations! This game is yours";

    Snake snake;
    Point food;
    PrintHelper pr;


    Point change(Point p, char c);
    Point generate_food_position();
    void draw_a_field();
    char generate_new_direction();
    void threadFunction1(char& new_dir, bool& you_win);
    void threadFunction2(char& new_dir, bool& cond);
    void clear();
public:
    Logic();
    void Run();
    
};
