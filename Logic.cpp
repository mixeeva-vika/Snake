#include"Logic.h"
#include <thread>
#include <chrono>
#include <queue>
#include <fstream>
#include <assert.h>

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
	{
		Point new_pos;
		do
		{
			new_pos = GeneratePoint();
		} while (DistanceBetweenPoints(new_pos, snake_start_position) < 5);
		
		enemy.Set(new_pos, i);
	}
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
	int step = rand() % 10;
	while (true)
	{
		
		if (step < 8)
		{
			dir = GenerateNewDirection();
			new_p = Change(p, dir);
		}
		else
			new_p = ShortestDirectionTowardsTheSnake(p);
			
		if ((new_p == food) || (PointBelongsToTheBlock(new_p)) || (PointBelongsToTheBlock(new_p)))
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
			
			//assert(DistanceBetweenPoints(p, new_p) == 1);
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

int Logic::DistanceBetweenPoints(Point a, Point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void Logic::Run()
{
    bool cond = true;
    bool you_win = false;
    char new_dir = 0; // ����� �����������, � ������� �������� ����
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
		if ((count % 3 == 0) && (new_dir != 0))
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

	ofstream file; // ������� ������ ������ ifstream
	file.open("output.txt"); // ��������� ����
	if (!file)
	{
		std::cout << "Error with file" << std::endl;
	}
	file << "enemy_point: " << enemy_point.x << " " << enemy_point.y << endl;
	queue<Point> q; // ������� �������
	q.push(enemy_point); // ��������� ������ ������� � �������(������� ����� � ������ ������)
	lenght[enemy_point.y][enemy_point.x] = 0;
	while (!q.empty())
	{
		
		Point tmp = q.front(); // ����� ������ ������� � �������(����� ��������� �� ������ ������� ��������)
		int cur_len = lenght[tmp.y][tmp.x] + 1;
		q.pop();  // ������� ������ ������� � �������
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

