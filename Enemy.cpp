#include"Enemy.h"
#include"PrintHelper.h"

Enemy::Enemy(int size, Field& field_): field(field_)
{
	points.resize(size, {0, 0});
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