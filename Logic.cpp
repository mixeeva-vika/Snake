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
    bool cond = true;
    bool you_win = false;
    char new_dir = 0; // новое направление, в котором движется змея
    std::thread thr1(&Logic::threadFunction1, *this, std::ref(new_dir), std::ref(you_win));
    std::thread thr2(&Logic::threadFunction2, *this, std::ref(new_dir), std::ref(cond));

    thr1.join();
    cond = false;
    clear();
    if(you_win == false)
        pr.Print(game_over_point_position1, game_over1);
    else
        pr.Print(win_point_position, win);
    pr.Print(game_over_point_position2, game_over2);
    thr2.join();
}

void Logic::threadFunction1(char& new_dir, bool& you_win)
{
    pr.Print(food, food_symbol);
    pr.Print(snake.Head(), snake_symbol);
    Point new_pos;
    while (true)
    {
        new_pos = change(snake.Head(), new_dir);
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
            if (snake.Size() == snake_size_for_win)
            {
                you_win = true;
                return;
            }
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

void Logic::threadFunction2(char& new_dir, bool& cond)
{
    char c;
    while (cond)
    {
        c = _getch();
        if ((c == 'a') || (c == 'w') || (c == 'd') || (c == 's'))
            new_dir = c;
        else
            continue;
    } 
}

void Logic::clear()
{
    for (short i = 1; i < hight - 1; ++i)
    {
        for (short j = 1; j < weight - 1; ++j)
        {
            pr.Clear({ j, i });
        }
    }
}