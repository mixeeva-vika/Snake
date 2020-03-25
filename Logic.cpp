#include"Logic.h"
#include <thread>
#include <chrono>

Logic::Logic(): snake(snake_start_position) , enemy(count_of_enemy)
{
     srand(time(0));
	 GenerateEnemyPosition();
	 block.resize(count_of_block);
	 for (int i = 0; i < block.size(); ++i)
		 block[i] = { 0, 0 };
	 GenerateBlockPosition();
     DrawTheField();
     food = GenerateFoodPosition();
}

void Logic::DrawTheField()
{ 
    for(short i = 0; i < width; ++i)
        pr.Print(Point{ i, 0 }, '-');
    for (short i = 0; i < width; ++i)
        pr.Print(Point{ i, height }, '-');
    for (short i = 0; i < height; ++i)
        pr.Print(Point{ 0, i }, '|');
    for (short i = 0; i < height; ++i)
        pr.Print(Point{ width , i }, '|');
	for (int i = 0; i < count_of_block; ++i)
		pr.Print(block[i], block_symbol);
	for (int i = 0; i < count_of_enemy; ++i)
		pr.Print(enemy.Get(i), block_symbol);
}

Point Logic::Change(Point p, char c)
{
    if (c == 'a')
    {
		bool a = PointBelongsToTheBlock({ p.x - 1, p.y });
		if (a)
		{
			int h = 0;
		}
		p.x -= ((p.x == 1) || (PointBelongsToTheBlock({p.x - 1, p.y}))) ? 0 : 1;
    }
    if (c == 'w')
    {
		bool b = PointBelongsToTheBlock({ p.x, p.y - 1});
		if (b)
		{
			int h = 0;
		}
        p.y -= ((p.y == 1) || (PointBelongsToTheBlock({ p.x, p.y - 1 }))) ? 0 : 1;
    }
    if (c == 'd')
    {
		bool c = PointBelongsToTheBlock({ p.x + 1, p.y });
		if (c)
		{
			int h = 0;
		}
        p.x += ((p.x == (width - 1)) || (PointBelongsToTheBlock({ p.x + 1, p.y})))? 0 : 1;
    }
	if (c == 's')
	{
		bool d = PointBelongsToTheBlock({ p.x, p.y + 1 });
		if (d)
		{
			int h = 0;
		}
		p.y += ((p.y == (height - 1)) || (PointBelongsToTheBlock({ p.x, p.y + 1 }))) ? 0 : 1;
	}
    return p;
}

Point Logic::GeneratePoint()
{
	Point p;
	do
	{
		p.x = rand() % (width - 1) + 1;
		p.y = rand() % (height - 1) + 1;
	} while (snake.PointBelongsToTheSnake(p) && PointBelongsToTheBlock(p) && enemy.PointBelongsToTheEnemy(p));

	return p;
}

Point Logic::GenerateFoodPosition()
{
   return GeneratePoint();
}

bool Logic::PointBelongsToTheBlock(Point p)
{
	for (int i = 0; i < block.size(); ++i)
	{
		if (p == block[i])
			return true;
	}
	return false;
}

void Logic::GenerateBlockPosition()
{
	for(int i = 0; i < count_of_block; ++i)
	    block[i] = GeneratePoint();
}

void Logic::GenerateEnemyPosition()
{
	for (int i = 0; i < count_of_enemy; ++i)
		enemy.Set(GeneratePoint(), i);
}


char Logic::GenerateNewDirection()
{
    static const std::vector<char> v{'a','w', 'd', 's'};
    int number = rand() % 4;
    return v[number];
}

bool Logic::MoveEnemy(int idx)
{
	Point p = enemy.Get(idx);
	char dir;
	Point new_p;
	while (true)
	{
		dir = GenerateNewDirection();
		new_p = Change(p, dir);
		if ((new_p == food) && (PointBelongsToTheBlock(p)))
			continue;
		else
		{
			if (snake.PointBelongsToTheSnake(new_p))
			{
				if (new_p == snake.Head())
					return false;
				else
				{
					ClearTailOfSnake(new_p);
				}
			}
			pr.Clear(p);
			enemy.Set(new_p, idx);
			pr.Print(new_p, enemy_symbol);
			return true;
		}
	}
}

bool Logic::MoveAllEnemy()
{
	for (int i = 0; i < enemy.Size(); ++i)
	{
		if (MoveEnemy(i) == false)
			return false;
	}
	return true;
}

void Logic::Run()
{
    bool cond = true;
    bool you_win = false;
    char new_dir = 0; // новое направление, в котором движется змея
    std::thread thr1(&Logic::ThreadFunction1, *this, std::ref(new_dir), std::ref(you_win));
    std::thread thr2(&Logic::ThreadFunction2, *this, std::ref(new_dir), std::ref(cond));

    thr1.join();
    cond = false;
    Clear();
    if(you_win == false)
        pr.Print(game_over_point_position1, game_over1);
    else
        pr.Print(win_point_position, win);
    pr.Print(game_over_point_position2, game_over2);
    thr2.join();
}

void Logic::ThreadFunction1(char& new_dir, bool& you_win)
{
    pr.Print(food, food_symbol);
    pr.Print(snake.Head(), snake_symbol);
    Point new_pos;
	int count = 1;
    while (true)
    {
        new_pos = Change(snake.Head(), new_dir);
        std::this_thread::sleep_for(200ms);
		/////////////////////////////////
		if (count % 3 == 0)
		{
			if (MoveAllEnemy() == false)
				return;
		}
		/////////////////////////////////
		++count;
        if (new_pos == snake.Head())
            continue;
        if (snake.PointBelongsToTheSnake(new_pos))
        {
            if (snake.PointIsSecondElemOfSnake(new_pos))
                continue;
            else
                return;
        }
		
		if (enemy.PointBelongsToTheEnemy(new_pos))
			return;

        if (food == new_pos)
        {
            snake.Add(new_pos);
            if (snake.Size() == snake_size_for_win)
            {
                you_win = true;
                return;
            }
            food = GenerateFoodPosition();
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

void Logic::ThreadFunction2(char& new_dir, bool& cond)
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

void Logic::Clear()
{
    for (short i = 1; i < height; ++i)
    {
        for (short j = 1; j < width; ++j)
        {
            pr.Clear({ j, i });
        }
    }
}

void Logic::ClearTailOfSnake(Point p)
{
	std::vector<Point> remove_points = snake.CutOfTail(p);
	for (int i = 0; i < remove_points.size(); ++i)
		pr.Clear(remove_points[i]);
}
