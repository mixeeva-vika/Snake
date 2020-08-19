#include"Snake.h"
#include"PrintHelper.h"
#include <assert.h>

Snake::Snake(Point p)
{
    const int size = 1;
    points.resize(size);
    for (int i = 0; i < size; ++i)
    {
        points[i].x = p.x;
        points[i].y = p.y - i;
    }
}

Point Snake::Head()
{
    return points.front();
}

Point Snake::Tail()
{
    return points.back();
}

void Snake::Move(Point p)
{
    if ((points[0].x == p.x) && (points[0].y == p.y))
        return;

    const bool is_right  = ((points[0].x == p.x - 1) && (points[0].y == p.y));
    const bool is_left   = ((points[0].x == p.x + 1) && (points[0].y == p.y));
    const bool is_top    = ((points[0].x == p.x) && (points[0].y == p.y - 1));
    const bool is_bottom = ((points[0].x == p.x) && (points[0].y == p.y + 1));

    assert(is_top || is_bottom || is_right || is_left);

    if ((points.size() > 1) && (points[1].x == p.x) && (points[1].y == p.y))
        return;

    points.emplace(points.begin(), p);
    points.pop_back();

}

void Snake::Add(Point p)
{
    const bool is_right = ((points[0].x == p.x - 1) && (points[0].y == p.y));
    const bool is_left = ((points[0].x == p.x + 1) && (points[0].y == p.y));
    const bool is_top = ((points[0].x == p.x) && (points[0].y == p.y - 1));
    const bool is_bottom = ((points[0].x == p.x) && (points[0].y == p.y + 1));

    assert(is_top || is_bottom || is_right || is_left);
    
    points.emplace(points.begin(), p);
}

void Snake::Cut(Point p)
{
	return;
}

bool Snake::PointBelongsToTheSnake(Point p)
{
    for (int i = 0; i < points.size(); ++i)
    {
        if (p == points[i])
            return true;
    }
    return false;
}

bool Snake::PointIsSecondElemOfSnake(Point p)
{
    if (p == points[1])
        return true;
    return false;
}

int Snake::Size()
{
    return (int)points.size();
}

std::vector<Point> Snake::CutOfTail(Point p)
{
	int start_idx = 0;
	int snake_size = points.size();
	while ((start_idx < snake_size) && !(points[start_idx] == p))
	{
		++start_idx;
	}
	assert(start_idx < snake_size);
	std::vector<Point> tail(snake_size - start_idx);
	for (int j = 0; j < tail.size(); ++j)
	{
		tail[j] = points[start_idx + j];
	}
	points.resize(start_idx);

	return tail;
}

const std::vector<Point>& Snake::GetPoints()
{
	return points;
}