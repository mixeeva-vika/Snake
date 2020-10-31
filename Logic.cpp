#include"Logic.h"
#include <thread>
#include <chrono>
#include <queue>
#include <fstream>
#include <assert.h>

Logic::Logic(int count_of_enemy_, int count_of_block_, int speed_, int power_of_brean_of_enemy_):
	snake(snake_start_position) , 
    count_of_enemy(count_of_enemy_), 
	count_of_block(count_of_block_),
	power_of_brean_of_enemy(power_of_brean_of_enemy_),
	enemy(count_of_enemy)
{
     srand(time(0));
	 GenerateEnemyPosition();
	 block.resize(count_of_block);
	 
}

Logic::~Logic()
{
	Clear();
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
		pr.Print(enemy.Get(i), enemy_symbol);
}

Point Logic::Change(Point p, char c)
{
	Point old_p = p;
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
	} while (snake.PointBelongsToTheSnake(p) || PointBelongsToTheBlock(p) || enemy.PointBelongsToTheEnemy(p)
                                                                || (p == food) || (p == freezingfood.Get()));

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
	{
		Point new_pos;
		do
		{
			new_pos = GeneratePoint();
		} while (DistanceBetweenPoints(new_pos, snake_start_position) < 5);
		for (int j = 0; j < i; ++j)
		{
			assert(!(new_pos == enemy.Get(j)));
		}
		enemy.Set(new_pos, i);
	}
}


char Logic::GenerateNewDirection()
{
    static const std::vector<char> v{'a','w', 'd', 's'};
    int number = rand() % 4;
    return v[number];
}

Point Logic::GenerateNeighborPoint(Point p, Point exceptional_point, bool snake_intersections)
{

	int new_direction = rand() % 4;

	for (int i = 0; i < offset_points.size(); ++i)
	{
		Point new_coordinates = p + offset_points[new_direction];
		if ((PointInsideTheField(new_coordinates)) && (!(new_coordinates == food)) && (!(new_coordinates == freezingfood.Get())) &&
			(!PointBelongsToTheBlock(new_coordinates)) && (!enemy.PointBelongsToTheEnemy(new_coordinates)) && !(new_coordinates == exceptional_point)
			                                                              && (snake_intersections || !snake.PointBelongsToTheSnake(new_coordinates)))
		{

			return new_coordinates;
		}

		new_direction = (new_direction + 1) % 4;
	}
	return p;
}

Point Logic::NewEnemyPosition(Point enemy_coordinates, Point smart_point)
{
	assert(!(smart_point == food));
	assert(!(smart_point == freezingfood.Get()));
	assert(!PointBelongsToTheBlock(smart_point));
	assert(PointInsideTheField(smart_point));
	
	bool smart_point_is_correct = ((!(smart_point == Point{ 0,0 })) && (!enemy.PointBelongsToTheEnemy(smart_point)));
	int step = rand() % 10;
	if (smart_point_is_correct && (step <= power_of_brean_of_enemy))
	{
		return smart_point;
	}
	
	return GenerateNeighborPoint(enemy_coordinates);
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
	pr.Clear(enemy_coordinates);
			
	//assert(DistanceBetweenPoints(enemy_coordinates, new_enemy_coordinates) == 1);
	enemy.Set(new_enemy_coordinates, enemy_idx);
	pr.Print(new_enemy_coordinates, enemy_symbol);
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

int Logic::DistanceBetweenPoints(Point a, Point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

bool Logic::Run()
{
	Clear();
	GenerateBlockPosition();
	DrawTheField();
	food = GenerateFoodPosition();
    bool cond = true;
    bool you_win = false;
    char key = 0; // новое направление, в котором движется змея
    std::thread thr1(&Logic::ThreadFunction1, *this, std::ref(key), std::ref(you_win));
    std::thread thr2(&Logic::ThreadFunction2, *this, std::ref(key), std::ref(cond));

    thr1.join();
    cond = false;
    Clear();
    if(you_win == false)
        pr.Print(level_over_point_position1, level_over1);////нужно сообщить Level
	/////////////////////////
   else
        pr.Print(win_point_position, win);////нужно сообщить Level
    pr.Print(game_over_point_position2, press_enter);
    thr2.join();
	return you_win;
}

void Logic::ThreadFunction1(char& key, bool& you_win)
{
    pr.Print(food, food_symbol);
    pr.Print(snake.Head(), snake_symbol);
    Point new_pos;
	int count = 1;
	char new_dir = 0;
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
			Point new_tail = GenerateNeighborPoint(tail, Change(snake.Head(), new_dir), false);
			if(!(tail == new_tail))
				snake.AddTail(new_tail);
			pr.Print(new_tail, snake_symbol);
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

        new_pos = Change(snake.Head(), new_dir);
        std::this_thread::sleep_for(200ms);
		/////////////////////////////////
		if (freezingfood.NeedToGeneratePoint())
		{
			freezingfood.Set(GenerateFoodPosition());
			pr.Print(freezingfood.Get(), freezingfood.GetSymbol());
		}
		else if (freezingfood.NeedToClear())
		{
			pr.Clear(freezingfood.Get());
			freezingfood.Set(Point{0,0});
		}
		if (new_pos == freezingfood.Get())
		{
			pr.Clear(freezingfood.Get());
			freezingfood.Set(Point{ 0,0 });
			enemy.SetFreezing();
		}
		if ((count % 3 == 0) && (new_dir != 0) && (!enemy.GetFreezing()) && (MoveAllEnemy() == false))
		{
			return;
		}
		
		++count;
		//++count_time_for_freezing_food;
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


bool Logic::PointInsideTheField(Point p)
{
	bool left = (p.x > 0);
	bool right = (p.x < width);
	bool up = (p.y  < height);
	bool down = (p.y > 0);
	if (left && right && up && down)
		return true;
	return false;
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

	std::vector<std::vector<int>> lenght(height);
	for (int i = 0; i < height; ++i)
		lenght[i].resize(width, free_place);

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
			if (!PointInsideTheField(cur_neighbor))
				continue;
			int& neighbor_len = lenght[cur_neighbor.y][cur_neighbor.x];
			if (neighbor_len == block_place)
				continue;
			if ((neighbor_len <= -10)&&(is_first_enemy))
			{
				int idx = fabs(neighbor_len + 10);
				if ((res[idx].x == 0) && (res[idx].y == 0))
				{
					is_first_enemy = false;
					//////////////////////
					assert(!(current_point == food));
					assert(!PointBelongsToTheBlock(current_point));
					assert(!enemy.PointBelongsToTheEnemy(current_point));
					//////////////////////
					res[idx] = current_point;
					neighbor_len = block_place;
					++count;
					//if (count == res.size())
					//	return res;
				}
				continue;
			}
			if (neighbor_len == free_place)
			{
				neighbor_len = cur_len;
				//pr.Print(Point{ cur_neighbor.x , cur_neighbor.y }, '0' + cur_len);
				q.push({ cur_neighbor.x , cur_neighbor.y });
			}
		}
	}
	Point zero = Point{ 0, 0 };
	for (int i = 0; i < res.size(); ++i)
	{
		if (res[i] == zero)
		{
			int a = 0;
		}

	}
	return res;
}

 void Logic::IncreaseSnake()
 {
	 return;
 }