#include"Snake.h"
#include"PrintHelper.h"
#include <assert.h>

Snake::Snake(Point p, Field& field_) : field(field_)
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

bool Snake::CheckProximityOfPoints(Point p1, Point p2)
{
	const bool is_right  = ((p2.x == p1.x - 1) && (p2.y == p1.y));
	const bool is_left   = ((p2.x == p1.x + 1) && (p2.y == p1.y));
	const bool is_top    = ((p2.x == p1.x)     && (p2.y == p1.y - 1));
	const bool is_bottom = ((p2.x == p1.x)     && (p2.y == p1.y + 1));
	return (is_top || is_bottom || is_right || is_left);
}

void Snake::Move(Point p)
{
	if ((can_be_eaten == false) && (field.Get(p) == Objects::Enemy))
		return;
    if ((points[0].x == p.x) && (points[0].y == p.y))
        return;

    assert(CheckProximityOfPoints(p, points[0]));

    if ((points.size() > 1) && (points[1].x == p.x) && (points[1].y == p.y))
        return;
	field.Set(Tail(), Objects::Empty);
    points.emplace(points.begin(), p);
    points.pop_back();
	
    field.Set(p, Objects::Snake);
}

void Snake::Add(Point p)
{
    assert(CheckProximityOfPoints(p, points[0]));
    
    points.emplace(points.begin(), p);
	field.Set(p, Objects::Snake);
}

void Snake::AddTail(Point p)
{
	assert(Size() > 0);
	Point tail = points[Size() - 1];
	assert(CheckProximityOfPoints(p, tail));
	points.emplace_back(p);

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

bool Snake::GetFreezing()
{
	return freezing;
}
void Snake::SetFreezing(bool freez)
{
	freezing = freez;
}
bool Snake::GetCanBeEaten()
{
	if (can_be_eaten == false)
	{
		unsigned int cur_time = clock();
		if (cur_time - start_time >= time_can_not_be_eaten)
		{
			can_be_eaten = true;
		}
	}
	return can_be_eaten;
}
void Snake::SetCanBeEaten()
{
	can_be_eaten = false;
	start_time = clock();
}