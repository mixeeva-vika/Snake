#pragma once
#include<cmath>
struct Point
{
    short x;
    short y;

    bool operator==(const Point& p) const
    {
		return ((x == p.x) && (y == p.y));
    }
	bool operator!=(const Point& p) const
	{
		return ((x != p.x) || (y != p.y));
	}
	Point operator + (const Point& p)
	{
		return Point{ x + p.x , y + p.y };
	}
	void operator = (const Point& p)
	{
		x = p.x;
		y = p.y;
	}
	int Distance(const Point& p)
	{
		return static_cast<int>(sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y)));
	}
};