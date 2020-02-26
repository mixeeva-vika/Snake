#include"Logic.h"

Point Logic::change(Point p, char c)
{
    if (c == 'a')
        --p.x;
    if (c == 'w')
        --p.y;
    if (c == 'd')
        ++p.x;
    if (c == 's')
        ++p.y;
    return p;
}

void Logic::Run()
{
    Snake snake(snake_start_position);
    snake.Print();

    PrintHelper pr;
    while (true)
    {
        char c = _getch();
        Point new_pos = change(snake.Head(), c);
        Point tail = snake.Tail();
        snake.Move(new_pos);
        pr.Print(new_pos, snake_symbol);
        pr.Clear(tail);
    }
}