#include"Enemy.h"
#include"PrintHelper.h"
#include <assert.h>
#include <queue>

Enemy::Enemy(Field& field_, EventManager& event_manager_, int count_of_enemy_, int power_of_brean_of_enemy_)
	: field(field_),
	event_manager(event_manager_), 
	count_of_enemy(count_of_enemy_), 
	power_of_brean_of_enemy(power_of_brean_of_enemy_)
{
	event_manager.SubscribeOnEvent(this, InitializeTheGame);
	event_manager.SubscribeOnEvent(this, SnakeEatFoodFreezing);
	event_manager.SubscribeOnEvent(this, SnakeEatFoodCanNotEatSnake);
}

bool Enemy::PointBelongsToTheEnemy(Point p)
{
	for (int i = 0; i < points.size(); ++i)
	{
		if (p == points[i])
			return true;
	}
	return false;
}

Point Enemy::Get(int idx)
{
	return points[idx];
}

int Enemy::Size()
{
	return (int)points.size();
}

void Enemy::Set(Point p, int idx)
{
	if (points[idx] != Point{0, 0})
	{
		field.Set(points[idx], Objects::Empty);
	}
	points[idx] = p;
	field.Set(p, Objects::Enemy);
}

const std::vector<Point>& Enemy::GetPoints()
{
	return points;
}

bool Enemy::GetFreezing()
{
	if (freezing == true)
	{
		unsigned int cur_time = clock();
		if (cur_time - start_freezing_time >= freezing_time)
		{
			freezing = false;
		}
	}
	return freezing;
}

void Enemy::SetFreezing()
{
	start_freezing_time = clock();
	freezing = true;
}

bool Enemy::GetCanEat()
{
	return can_be_eaten;
}
void Enemy::SetCanEat(bool can_be_eaten_)
{
	can_be_eaten = can_be_eaten_;
}

std::vector<Point> Enemy::ShortestDirectionTowardsTheSnake()
{
	queue<Point> q;
	int snake_place = 0;
	int block_place = -1;
	int free_place = -3;
	auto field_size = field.GetFieldSize();
	std::vector<std::vector<int>> lenght(field_size.second, std::vector<int>(field_size.first, free_place));
	for (short i = 1; i < field_size.second; ++i)
	{
		for (short j = 1; j < field_size.first; ++j)
		{
			switch (field.Get(Point{ j, i }))
			{
			case Objects::Snake: lenght[i][j] = snake_place; q.push(Point{j,i}); break;
			case Objects::Empty: lenght[i][j] = free_place; break;
			case Objects::Enemy: lenght[i][j] = free_place; break;
			case Objects::Block: lenght[i][j] = block_place; break;
			case Objects::Food: lenght[i][j] = block_place; break;
			case Objects::FoodCanNotEatSnake: lenght[i][j] = block_place; break;
			case Objects::FoodFreezing: lenght[i][j] = block_place; break;

			}
		}
	}     
	for (int i = 0; i < points.size(); ++i)
	{
		assert(lenght[points[i].y][points[i].x] == free_place);
		lenght[points[i].y][points[i].x] = -10 - i;
	}

	std::vector<Point> res(points.size(), Point{ 0,0 });
	std::vector<Point> offset_points = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };
	int count = 0;
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


Point Enemy::GenerateNeighborPoint(Point p, bool snake_intersections, Point exceptional_point)
{
	int new_direction = rand() % 4;

	for (int i = 0; i < offset_points.size(); ++i)
	{
		Point new_point = p + offset_points[new_direction];
		if (field.InTheField(new_point) && (new_point != exceptional_point) &&
			((field.Get(new_point) == Objects::Empty) || (snake_intersections && field.Get(new_point) == Objects::Snake)))
			return new_point;
		new_direction = (new_direction + 1) % 4;
	}
	return p;
}

Point Enemy::NewEnemyPosition(Point enemy_coordinates, Point smart_point)
{
	bool smart_point_is_correct = ((smart_point != Point{ 0,0 }) && (field.Get(smart_point) != Objects::Enemy) && (SnakeGetCanBeEaten() || field.Get(smart_point) != Objects::Snake));
	int step = rand() % 10;
	if (smart_point_is_correct && (step <= power_of_brean_of_enemy))
		return smart_point;
	return GenerateNeighborPoint(enemy_coordinates, SnakeGetCanBeEaten());
}

bool Enemy::MoveEnemy(int enemy_idx, Point smart_point)
{
	Point enemy_coordinates = Get(enemy_idx);
	Point new_enemy_coordinates = NewEnemyPosition(enemy_coordinates, smart_point);
	//assert(snake.GetCanBeEaten() || field.Get(new_enemy_coordinates) != Objects::Snake);
	if (field.Get(new_enemy_coordinates) == Objects::Snake)
	{
		event_manager.PushEvent(new EventWithPoint(EnemyCrossWithSnake, new_enemy_coordinates));
	}
	Set(new_enemy_coordinates, enemy_idx);
	return true;
}

bool Enemy::MoveAllEnemy()
{
	std::vector<Point> smart_position = ShortestDirectionTowardsTheSnake();
	for (int i = 0; i < Size(); ++i)
	{
		if (MoveEnemy(i, smart_position[i]) == false)
			return false;
	}
	return true;
}


bool Enemy::SnakeGetCanBeEaten()
{
	if (snake_can_be_eaten == false)
	{
		unsigned int cur_time = clock();
		if (cur_time - start_time_for_snake_can_not_be_eaten >= time_snake_can_not_be_eaten)
		{
			snake_can_be_eaten = true;
		}
	}
	return snake_can_be_eaten;
}
void Enemy::SnakeSetCanBeEaten()
{
	snake_can_be_eaten = false;
	start_time_for_snake_can_not_be_eaten = clock();
}


void Enemy::Init()
{
	for (int i = 0; i < count_of_enemy; i++)
	{
		Point cur = field.GeneratePoint();
		points.push_back(cur);
		field.Set(cur, Objects::Enemy);
	}
}

void Enemy::OnEvent(Event* et)
{
	if (et->GetEventType() == EventType::InitializeTheGame)
	{
		Init();
		return;
	}

	if (et->GetEventType() == EventType::SnakeEatFoodFreezing)
	{
		SetFreezing();
		return;
	}

	if (et->GetEventType() == EventType::SnakeEatFoodCanNotEatSnake)
	{
		SnakeSetCanBeEaten();
		return;
	}

	throw;
}

bool Enemy::NeedToMove()
{
	unsigned int cur_time = clock();
	if (!GetFreezing() && ((cur_time - start_stop_time >= time_for_move)))
	{
		start_stop_time = cur_time;
		return true;
	}
	return false;
}

void Enemy::Action()
{
	if (NeedToMove())
	{
		MoveAllEnemy();
	}
	
	return;
}


