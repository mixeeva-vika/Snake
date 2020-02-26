#include"Snake.h"
#include "assert.h"
#include"PrintHelper.h"

Snake::Snake(Point p)
{
    const int size = 3;
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

    if ((points[1].x == p.x) && (points[1].y == p.y))
        return;

    vector<Point>::iterator it;
    it = points.begin();
    points.emplace(it, p);
    points.pop_back();

}

void Snake::Add(Point p)
{
    return;
}

void Snake::Print()
{
    PrintHelper pr;
    for (auto &point:points)
        pr.Print(point, 219);
}