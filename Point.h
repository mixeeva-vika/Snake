#pragma once
struct Point
{
    short x;
    short y;

    bool operator==(Point p)
    {
        if ((x == p.x) && (y == p.y))
            return true;
        return false;
    }
	Point operator + (Point p)
	{
		Point point;
		point.x = x + p.x;
		point.y = y + p.y;
		return point;
	}
};