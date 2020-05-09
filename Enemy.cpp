#include"Enemy.h"
#include"PrintHelper.h"

Enemy::Enemy(int size)
{
	points.resize(size);
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
	points[idx] = p;
}

const std::vector<Point>& Enemy::GetPoints()
{
	return points;
}