#include"Logic.h"
#include <thread>
#include <chrono>

Logic::Logic(): snake(snake_start_position) 
{
     srand(time(0));
     draw_a_field();
     food = generate_food_position();
}

void Logic::draw_a_field()
{
    
    for(short i = 0; i < weight; ++i)
        pr.Print(Point{ i, 0 }, '-');
    for (short i = 0; i < weight; ++i)
        pr.Print(Point{ i, hight }, '-');
    for (short i = 0; i < hight; ++i)
        pr.Print(Point{ 0, i }, '|');
    for (short i = 0; i < hight; ++i)
        pr.Print(Point{ weight, i }, '|');
}

Point Logic::change(Point p, char c)
{
    if (c == 'a')
    {
        p.x -= (p.x == 1) ? 0 : 1;
    }
    if (c == 'w')
    {
        p.y -= (p.y == 1) ? 0 : 1;
    }
    if (c == 'd')
    {
        p.x += (p.x == (weight - 1)) ? 0 : 1; 
    }
    if (c == 's')
        p.y += (p.y == (hight - 1)) ? 0 : 1;
    return p;
}

Point Logic::generate_food_position()
{
    Point p;
    do
    {
        p.x = rand() % (weight - 1) + 1;
        p.y = rand() % (hight - 1) + 1;
    } while (snake.PointBelongsToTheSnake(p));
    
    return p;
}


char Logic::generate_new_direction()
{
    std::vector<char> v{'a','w', 'd', 's'};
    int number = rand() % 4;
    return v[number];
}

void Logic::Run()
{
    int step = 0;
    pr.Print(food, food_symbol);
    pr.Print(snake.Head(), snake_symbol);
    Point new_pos;
    char new_dir;
    while (true)
    {
        //char c = _getch();
        if (step % 10 == 0)
        {
            new_dir = generate_new_direction();
        }
        new_pos = change(snake.Head(), new_dir);
        step++;
        std::this_thread::sleep_for(250ms);
        if (new_pos == snake.Head())
            continue;
        if (snake.PointBelongsToTheSnake(new_pos))
        {
            if (snake.PointIsSecondElemOfSnake(new_pos))
                continue;
            else
                return;
        }
        if (food == new_pos)
        {
            snake.Add(new_pos);
            food = generate_food_position();
            pr.Print(food, food_symbol);
        }
        else
        {
            Point tail = snake.Tail();
            snake.Move(new_pos);
            pr.Clear(tail);
        }

        pr.Print(new_pos, snake_symbol);
       
    }
}