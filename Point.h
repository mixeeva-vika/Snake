#pragma once
struct Point
{
    short x;
    short y;

    bool operator==(const Point& p) const
    {
        if ((x == p.x) && (y == p.y))
            return true;
        return false;
    }
	Point operator + (const Point& p)
	{
		Point point;
		point.x = x + p.x;
		point.y = y + p.y;
		return point;
	}
	void operator = (const Point& p)
	{
		x = p.x;
		y = p.y;
	}

};