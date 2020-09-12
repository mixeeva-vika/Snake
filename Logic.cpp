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
	} while (snake.PointBelongsToTheSnake(p) || PointBelongsToTheBlock(p) || enemy.PointBelongsToTheEnemy(p));

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
		if ((PointInsideTheField(new_coordinates)) && (!(new_coordinates == food)) &&
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
		if ((count % 3 == 0) && (new_dir != 0))
		{
			//pr.Print({ 0 ,height + 1}, '!');
			for (int i = 0; i < count_of_enemy; ++i)
			{
				Point cur_enemy = enemy.Get(i);
				//Point cur_enemy = enemy.Get(i);
				//std::cout << '{' << cur_enemy.x <<' '<< cur_enemy.y<< '}' << "  ";
				
				for (int j = 0; j < i; ++j)
				{
					assert(!(enemy.Get(j) == cur_enemy));
				}
			}
			std::cout << endl;
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

Point Logic::ShortestDirectionTowardsTheSnake(Point enemy_point)
{
	std::vector<std::vector<int>> lenght(height);
	for (int i = 0; i < height; ++i)
		lenght[i].resize(width, -3);
	for (short i = 0; i < height; ++i)
	{
		for (short j = 0; j < width; ++j)
		{
			if (PointBelongsToTheBlock({ j, i }) || (Point{j, i} == food))
				lenght[i][j] = -1;
			if (snake.PointBelongsToTheSnake({ j, i }))
				lenght[i][j] = -2;
			bool left = (enemy_point.x - 1 > 0)&&(enemy.PointBelongsToTheEnemy({ enemy_point.x - 1 , enemy_point.y }));
			bool right = (enemy_point.x + 1 < width) && (enemy.PointBelongsToTheEnemy({ enemy_point.x + 1 , enemy_point.y }));
			bool up = (enemy_point.y + 1 < height) && (enemy.PointBelongsToTheEnemy({ enemy_point.x  , enemy_point.y + 1}));
			bool down = (enemy_point.y - 1 > 0) && (enemy.PointBelongsToTheEnemy({ enemy_point.x , enemy_point.y - 1}));
			if(left)
				lenght[enemy_point.y][enemy_point.x - 1] = -1;
			if(right)
				lenght[enemy_point.y][enemy_point.x + 1] = -1;
			if(up)
				lenght[enemy_point.y + 1][enemy_point.x] = -1;
			if(down)
				lenght[enemy_point.y - 1][enemy_point.x] = -1;
		}
	}

	ofstream file; // создаем объект класса ifstream
	file.open("output.txt"); // открываем файл
	if (!file)
	{
		std::cout << "Error with file" << std::endl;
	}
	file << "enemy_point: " << enemy_point.x << " " << enemy_point.y << endl;
	queue<Point> q; // создаем очередь
	q.push(enemy_point); // добавляем первый элемент в очередь(позиция врага в данный момент)
	lenght[enemy_point.y][enemy_point.x] = 0;
	while (!q.empty())
	{
		
		Point tmp = q.front(); // Берем первый элемент в очереди(нужен указатель на первый элемент очередио)
		int cur_len = lenght[tmp.y][tmp.x] + 1;
		q.pop();  // Удаляем первый элемент в очереди
		bool left = (tmp.x - 1 > 0) && (lenght[tmp.y][tmp.x - 1] != -1);
		bool right = (tmp.x + 1 < width) && (lenght[tmp.y][tmp.x + 1] != -1);
		bool up = (tmp.y + 1 < height) && (lenght[tmp.y + 1][tmp.x] != -1);
		bool down = (tmp.y - 1 > 0) && (lenght[tmp.y - 1][tmp.x] != -1);
		if ((left) && (lenght[tmp.y][tmp.x - 1] == -2) ||
			(right) && (lenght[tmp.y][tmp.x + 1] == -2) ||
			(up) && (lenght[tmp.y + 1][tmp.x] == -2) ||
			(down) && (lenght[tmp.y - 1][tmp.x] == -2))
		{
			if ((tmp.x == enemy_point.x) && (tmp.y == enemy_point.y))
			{
				if ((left) && (lenght[tmp.y][tmp.x - 1] == -2))
					return Point{ tmp.x - 1, tmp.y};
				if ((right) && (lenght[tmp.y][tmp.x + 1] == -2))
					return Point{ tmp.x + 1, tmp.y };
				if ((up) && (lenght[tmp.y + 1][tmp.x] == -2))
					return Point{ tmp.x, tmp.y + 1};
				if ((down) && (lenght[tmp.y - 1][tmp.x] == -2))
					return Point{ tmp.x, tmp.y - 1};
			}
			int len = lenght[tmp.y][tmp.x] - 1;
			Point cur = tmp;
			file << "tmp: " << tmp.x << " "<< tmp.y << endl;
			
			while (len != 0)
			{
				bool left_ = (cur.x - 1 > 0) && (lenght[cur.y][cur.x - 1] != -1);
				bool right_ = (cur.x + 1 < width) && (lenght[cur.y][cur.x + 1] != -1);
				bool up_ = (cur.y + 1 < height) && (lenght[cur.y + 1][cur.x] != -1);
				bool down_ = (cur.y - 1 > 0) && (lenght[cur.y - 1][cur.x] != -1);

				file << "len_: " << len << endl;	
				if ((left_) && (len == lenght[cur.y][cur.x - 1]))
				{
					cur = { cur.x - 1 , cur.y };
				}
				else if ((right_) && (len == lenght[cur.y][cur.x + 1]))
				{
					cur = { cur.x + 1 , cur.y };
				}
				else if ((up_) && (len == lenght[cur.y + 1][cur.x]))
				{
					cur = { cur.x , cur.y + 1 };
				}
				else if ((down_) && (len == lenght[cur.y - 1][cur.x]))
				{
					cur = { cur.x , cur.y - 1 };
				}
				else
					throw " Error! No way";

				assert((cur.x != enemy_point.x)||(cur.y != enemy_point.y));
				if ((cur.x < 0) || (cur.y < 0))
				{
					int b = 0;
				}
				file << " x :" << cur.x << " y: " << cur.y << endl;
				file << endl;
				--len;
			}
			
			/////////////////////////////////////////////////////////////////////////
			/*
		    for (int i = 0; i < lenght.size(); ++i)
			{
				for (int j = 0; j < lenght[i].size(); ++j)
				{
					if (lenght[i][j] == -3)
					{
						file << " _ ";
					}
					else if (lenght[i][j] == -1)
					{
						file << " " << block_symbol << " ";
					}
					else if (lenght[i][j] == -2)
					{
						file << " " << snake_symbol << " ";
					}
					else
					{
						if (lenght[i][j] < 10)
							file << " " << lenght[i][j] << " ";
						else if (lenght[i][j] >= 10)
							file << " " << lenght[i][j];
					}
					//file << lenght[i][j] << " ";
				}
				file << std::endl;
			}
			/////////////////////////////////////////////////////////////////////////
			file <<"cur " << cur.x << cur.y << std::endl;
			
			if (cur == enemy_point)
			{
				int a = 0;
				file << "cur == enemy_point" << endl;
			}
			*/
			file.close();
			return cur;
		}

		if ((left) && (lenght[tmp.y][tmp.x - 1] == -3))
		{
			lenght[tmp.y][tmp.x - 1] = cur_len;
			//pr.Print(Point{ tmp.x - 1 , tmp.y }, '0' + cur_len);
			q.push({ tmp.x - 1 , tmp.y });
		}
		if ((right) && (lenght[tmp.y][tmp.x + 1] == -3))
		{
			lenght[tmp.y][tmp.x + 1] = cur_len;
			//pr.Print(Point{ tmp.x + 1 , tmp.y }, '0' + cur_len);
			q.push({ tmp.x + 1, tmp.y });
		}
		if ((up) && (lenght[tmp.y + 1][tmp.x] == -3))
		{
			lenght[tmp.y + 1][tmp.x] = cur_len;
			//pr.Print(Point{ tmp.x, tmp.y + 1 }, '0' + cur_len);
			q.push({ tmp.x, tmp.y + 1});
		}
		if ((down) && (lenght[tmp.y - 1][tmp.x] == -3))
		{
			lenght[tmp.y - 1][tmp.x] = cur_len;
			//pr.Print(Point{ tmp.x , tmp.y - 1}, '0' + cur_len);
			q.push({ tmp.x, tmp.y - 1 });
		}
		

	}	
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