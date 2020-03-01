#include"Logic.h"
#include<time.h>

Logic::Logic(): snake(snake_start_position) 
{
     srand(time(0));
     food = generate_food_position();
}

Point Logic::change(Point p, char c)
{
    if (c == 'a')
    {
        p.x -= (p.x == 0) ? 0 : 1;
    }
    if (c == 'w')
    {
        p.y -= (p.y == 0) ? 0 : 1;
    }
    if (c == 'd')
    {
        p.x += (p.x == weight) ? 0 : 1; // ++p.x;
    }
    if (c == 's')
        p.y += (p.y == hight) ? 0 : 1;//++p.y;
    return p;
}

Point Logic::generate_food_position()
{
    Point p;
    do
    {
        p.x = rand() % weight;
        p.y = rand() % hight;
    } while (snake.PointBelongsToTheSnake(p));
    
    return p;
}

void Logic::Run()
{
    PrintHelper pr;
    pr.Print(food, '*');
    snake.Print();

    while (true)
    {
        char c = _getch();
        Point new_pos = change(snake.Head(), c);
        if (new_pos == snake.Head())
            continue;
        if (snake.PointBelongsToTheSnake(new_pos))
            return;
        if (food == new_pos)
        {
            snake.Add(new_pos);
            food = generate_food_position();
            pr.Print(food, '*');
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