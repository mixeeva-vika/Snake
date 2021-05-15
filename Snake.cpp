#include"Snake.h"
#include"PrintHelper.h"
#include <assert.h>

Snake::Snake(Field& field_, EventManager& event_manager_) : field(field_), event_manager(event_manager_)
{
	event_manager.SubscribeOnEvent(this, InitializeTheGame);
	event_manager.SubscribeOnEvent(this, EnemyCrossWithSnake);
	event_manager.SubscribeOnEvent(this, MoveUp);
	event_manager.SubscribeOnEvent(this, MoveLeft);
	event_manager.SubscribeOnEvent(this, MoveRight);
	event_manager.SubscribeOnEvent(this, MoveDown);
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
	if (!field.InTheField(p) || (field.Get(p) == Objects::Block))
		return;
	if(field.Get(p) == Objects::Food)
	{
		Add(p);
		if (Size() == snake_size_for_win)
		{
			event_manager.PushEvent(new Event(Win));
			return;
		}
		event_manager.PushEvent(new Event(SnakeEatFood));
		return;
	}
	if (field.Get(p) == Objects::FoodFreezing)
	{
		event_manager.PushEvent(new Event(SnakeEatFoodFreezing));
	}
	if (field.Get(p) == Objects::FoodCanNotEatSnake)
	{
		event_manager.PushEvent(new Event(SnakeEatFoodCanNotEatSnake));
	}
	if (p == Head())
		return;
	if (field.Get(p) == Objects::Snake)
	{
		if (PointIsSecondElemOfSnake(p))
			return;
		else
		{
			event_manager.PushEvent(new Event(Losing));
			return;
		}
	}
	if (field.Get(p) == Objects::Enemy)
	{
		event_manager.PushEvent(new Event(Losing));
		return;
	}

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

void Snake::CutOfTail(Point p)
{
	while(true)
	{
		Point last = points.back();
		field.Set(last, Objects::Empty);
		points.pop_back();
		if (last == p)
			break;
	}
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



void Snake::Init()
{
	if (points.empty())
	{
		points.push_back(field.GeneratePoint());
		field.Set(Head(), Objects::Snake);
	}
	return;
}
void Snake::OnEvent(Event* et)
{
	if (et->GetEventType() == EventType::InitializeTheGame)
	{
		Init();
		return;
	}
	if (et->GetEventType() == EventType::EnemyCrossWithSnake)
	{ 
		auto et_with_point = dynamic_cast<EventWithPoint*>(et);
		Point intersection_point = et_with_point->GetPoint();
		if (intersection_point == Head())
		{
			event_manager.PushEvent(new Event(Losing));
		}
		else
		{
		    CutOfTail(intersection_point);
		}
		return;
	}
	
	if (et->GetEventType() == EventType::MoveUp)
	{
		Move(Head() + Point{0, -1});
		return;
	}
	if (et->GetEventType() == EventType::MoveLeft)
	{
		Move(Head() + Point{ -1, 0 });
		return;
	}
	if (et->GetEventType() == EventType::MoveRight)
	{
		Move(Head() + Point{ 1, 0 });
		return;
	}
	if (et->GetEventType() == EventType::MoveDown)
	{
		Move(Head() + Point{ 0, 1 });
		return;
	}
	throw;
}

void Snake::Action()
{

}