#include"Logic.h"
#include <thread>
#include <chrono>
#include <queue>
#include <fstream>
#include <assert.h>
#include<iostream>

Logic::Logic(int count_of_enemy_, int count_of_block_, int speed_, int power_of_brean_of_enemy_):
	snake(snake_start_position) , 
    count_of_enemy(count_of_enemy_), 
	count_of_block(count_of_block_),
	power_of_brean_of_enemy(power_of_brean_of_enemy_),
	enemy(count_of_enemy, field),
	freezingfood(field)
{
     srand(static_cast<unsigned int>(time(0)));
	 block.resize(count_of_block); 
}

Logic::~Logic()
{
	field.Clear();
}

void Logic::GenerateBlockPosition()
{
	for (int i = 0; i < count_of_block; ++i)
	{
		block[i] = field.GeneratePoint();
		field.Set(block[i], Objects::Block);
	}
}

void Logic::GenerateEnemyPosition()
{
	for (int i = 0; i < count_of_enemy; ++i)
	{
		Point new_pos;
		do
		{
			new_pos = field.GeneratePoint();
		} while (new_pos.Distance(snake_start_position) < 5);
		for (int j = 0; j < i; ++j)
		{
			assert(new_pos != enemy.Get(j));
		}
		enemy.Set(new_pos, i);
	}
}

void Logic::InitializeTheGame()
{
	field.Clear();
	field.DrawTheFieldBoundary();
	GenerateBlockPosition();
	GenerateEnemyPosition();
	food = field.GeneratePoint();
	field.Set(food, Objects::Food);
	field.Set(snake.Head(), Objects::Snake);
}

Point Logic::MovePoint(Point p, char c)
{
	Point diff;
    if (c == 'a')
		diff = { -1, 0 };
    if (c == 'w')
		diff = { 0, -1 };
    if (c == 'd')
		diff = { 1, 0 };
	if (c == 's')
		diff = { 0, 1 };
	Point new_point = p + diff;
	if (field.InTheField(new_point) && (field.Get(new_point) != Objects::Block))
		return new_point;
    return p;
}

Point Logic::GenerateNeighborPoint(Point p, bool snake_intersections, Point exceptional_point)
{
	int new_direction = rand() % 4;

	for (int i = 0; i < offset_points.size(); ++i)
	{
		Point new_point = p + offset_points[new_direction];
		if(field.InTheField(new_point) && (new_point != exceptional_point) &&
			((field.Get(new_point) == Objects::Empty) || (snake_intersections && field.Get(new_point) == Objects::Snake)))
			return new_point;
		new_direction = (new_direction + 1) % 4;
	}
	return p;
}

Point Logic::NewEnemyPosition(Point enemy_coordinates, Point smart_point)
{
	bool smart_point_is_correct = ((smart_point != Point{ 0,0 }) && (field.Get(smart_point) != Objects::Enemy));
	int step = rand() % 10;
	if (smart_point_is_correct && (step <= power_of_brean_of_enemy))
		return smart_point;
	return GenerateNeighborPoint(enemy_coordinates, snake.GetCanBeEaten());
}

void Logic::ClearTailOfSnake(Point p)
{
	std::vector<Point> remove_points = snake.CutOfTail(p);
	for (int i = 0; i < remove_points.size(); ++i)
		field.Set(remove_points[i], Objects::Empty);
}

std::vector<Point> Logic::ShortestDirectionTowardsTheSnake()
{
	std::vector<Point> enemys = enemy.GetPoints();
	std::vector<Point> snake_points = snake.GetPoints();
	queue<Point> q;
	std::vector<Point> res(enemys.size(), Point{ 0,0 });
	int count = 0;
	int snake_place = 0;
	int block_place = -1;
	int free_place = -3;
	auto field_size = field.GetFieldSize();
	std::vector<std::vector<int>> lenght(field_size.second, std::vector<int>(field_size.first, free_place));
	lenght[food.y][food.x] = block_place;
	lenght[freezingfood.Get().y][freezingfood.Get().x] = block_place;
	for (short i = 0; i < block.size(); ++i)
	{
		lenght[block[i].y][block[i].x] = block_place;
	}
	for (short i = 0; i < snake.Size(); ++i)
	{
		lenght[snake_points[i].y][snake_points[i].x] = snake_place;
		q.push(snake_points[i]);
	}

	for (int i = 0; i < enemys.size(); ++i)
	{
		assert(lenght[enemys[i].y][enemys[i].x] == free_place);
		lenght[enemys[i].y][enemys[i].x] = -10 - i;
	}
	
	while (!q.empty())
	{
		Point current_point = q.front(); // Берем первый элемент в очереди(нужен указатель на первый элемент очередио)
		int cur_len = lenght[current_point.y][current_point.x] + 1;
		q.pop();  // Удаляем первый элемент в очереди
		bool is_first_enemy = true;
		for (int i = 0; i < offset_points.size(); ++i)
		{
			Point cur_neighbor = current_point + offset_points[i];
			if (!field.InTheField(cur_neighbor))
				continue;
			int& neighbor_len = lenght[cur_neighbor.y][cur_neighbor.x];
			if (neighbor_len == block_place)
				continue;
			if ((neighbor_len <= -10) && (is_first_enemy))
			{
				int idx = static_cast<int>(fabs(neighbor_len + 10));
				if ((res[idx].x == 0) && (res[idx].y == 0))
				{
					is_first_enemy = false;
					
					//assert((field.Get(current_point) == Objects::Empty) || (field.Get(current_point) == Objects::Snake));
					if (field.Get(current_point) != Objects::Empty)
					{
						auto e = field.Get(current_point);
						int a = 0;
					}
					res[idx] = current_point;
					neighbor_len = block_place;
					++count;
				}
				continue;
			}
			if (neighbor_len == free_place)
			{
				neighbor_len = cur_len;
				q.push({ cur_neighbor.x , cur_neighbor.y });
			}
		}
	}
	return res;
}

bool Logic::MoveEnemy(int enemy_idx, Point smart_point) 
{
	Point enemy_coordinates = enemy.Get(enemy_idx);
	Point new_enemy_coordinates = NewEnemyPosition(enemy_coordinates, smart_point);
	if (snake.PointBelongsToTheSnake(new_enemy_coordinates))
	{
		if (new_enemy_coordinates == snake.Head())
			return false;
		else
			ClearTailOfSnake(new_enemy_coordinates);
	}
	enemy.Set(new_enemy_coordinates, enemy_idx);
	return true;
}

bool Logic::MoveAllEnemy()
{
	std::vector<Point> smart_position = ShortestDirectionTowardsTheSnake();
	for (int i = 0; i < enemy.Size(); ++i)
	{
		if (MoveEnemy(i, smart_position[i]) == false)
			return false;
	}
	return true;
}

bool Logic::FieldCheck()
{
	auto field_size = field.GetFieldSize();
	for (short i = 1; i < field_size.first; ++i)
	{
		for (short j = 1; j < field_size.second; ++j)
		{
			if (field.Get(Point{ i, j }) == Objects::Enemy)
			{
				bool flag = false;
				for (int k = 0; k < enemy.Size(); ++k)
				{
					if (enemy.Get(k) == Point{ i, j })
					{
						flag = true;
					}
				}
				if (flag == false)
				{
					int a = 0;
					//return false;
				}
			}
		}
	}
	for (int i = 0; i < enemy.Size(); ++i)
	{
		if (field.Get(enemy.Get(i)) != Objects::Enemy)
		{
			int a = 0;
		}
	}
	return true;
}

void Logic::ThreadFunction1(char& key, bool& you_win)
{
	InitializeTheGame();
    Point new_pos;
	int count = 1;
	char new_dir = 0;
	ofstream file("out.txt");
    while (true)
    {
		if (key == 0)
			continue;
		if (key == 'n')
		{
			you_win = true;
			return;
		}
		if (key == 'i')
		{
			Point tail = snake.Tail();
			Point new_tail = GenerateNeighborPoint(tail, false, MovePoint(snake.Head(), new_dir));
			if(tail != new_tail)
				snake.AddTail(new_tail);
			field.Set(new_tail, Objects::Snake);
			if (snake.Size() == snake_size_for_win)
			{
				you_win = true;
				return;
			}
			key = new_dir;
		}
		if ((key != 'n') && (key != 'i'))
		{
			new_dir = key;
		}
        new_pos = MovePoint(snake.Head(), new_dir);
        std::this_thread::sleep_for(200ms);
		/////////////////freezingfood////////////////
		if (freezingfood.NeedToGeneratePoint())
		{
			freezingfood.Set(field.GeneratePoint());
		}
		else if (freezingfood.NeedToClear())
		{
			freezingfood.Set(Point{0,0});
		}
		if (new_pos == freezingfood.Get())
		{
			freezingfood.Set(Point{ 0,0 });
			enemy.SetFreezing();
		}
		/////////////////////////////////////////////////
		//////////////////food_can_not_eat_snake//////////
		if (food_can_not_eat_snake.NeedToGeneratePoint())
		{
			food_can_not_eat_snake.Set(field.GeneratePoint());
			field.Set(food_can_not_eat_snake.Get(), Objects::FoodCanNotEatSnake);
		}
		else if (food_can_not_eat_snake.NeedToClear())
		{
			field.Set(food_can_not_eat_snake.Get(), Objects::Empty);
			food_can_not_eat_snake.Set(Point{ 0,0 });
		}
		if (new_pos == food_can_not_eat_snake.Get())
		{
			field.Set(food_can_not_eat_snake.Get(), Objects::Empty);
			food_can_not_eat_snake.Set(Point{ 0,0 });
		}
		/////////////////////////////////////////////////
		if ((count % 3 == 0) && (new_dir != 0) && (!enemy.GetFreezing()) && (MoveAllEnemy() == false))
		{
			return;
		}
		++count;
        if (new_pos == snake.Head())
            continue;
        if (field.Get(new_pos) == Objects::Snake)
        {
            if (snake.PointIsSecondElemOfSnake(new_pos))
                continue;
            else
                return;
        }
		
		if (field.Get(new_pos) == Objects::Enemy)
			return;

        if (food == new_pos)
        {
            snake.Add(new_pos);
            if (snake.Size() == snake_size_for_win)
            {
                you_win = true;
                return;
            }
            food = field.GeneratePoint();
			field.Set(food, Objects::Food);
        }
        else
        {
            Point tail = snake.Tail();
            snake.Move(new_pos);
			field.Set(tail, Objects::Empty);
        }
		field.Set(new_pos, Objects::Snake);
    }
	
}

void Logic::ThreadFunction2(char& key, bool& cond)
{
    char c;
    while (cond)
    {
        c = _getch();
        if ((c == 'a') || (c == 'w') || (c == 'd') || (c == 's') || (c == 'n') || (c == 'i'))
            key = c;
        else
            continue;
    } 
}

 bool Logic::Run()
 {
	 bool cond = true;
	 bool you_win = false;
	 char key = 0; // новое направление, в котором движется змея
	 std::thread thr1(&Logic::ThreadFunction1, this, std::ref(key), std::ref(you_win));
	 std::thread thr2(&Logic::ThreadFunction2, this, std::ref(key), std::ref(cond));
	 thr1.join();
	 cond = false;
	 if (you_win == false)
		 field.PrintLevelOver();
	 else
		 field.PrintWin();
	 thr2.join();
	 return you_win;
 }
