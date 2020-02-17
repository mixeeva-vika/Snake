#include <iostream>
#include <conio.h>
namespace
{
#include <windows.h>
}
using namespace std;

struct Point
{
    short x;
    short y;
};

void change(Point& p, char c)
{
    if (c == 'a')
        --p.x;
    if (c == 'w')
        --p.y;
    if (c == 'd')
        ++p.x;
    if (c == 's')
        ++p.y;
}


class PrintHelper
{
    HANDLE _console_handle;
    CONSOLE_CURSOR_INFO info;

public:
    PrintHelper()
    {
        _console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        info.bVisible = false;
        info.dwSize = 100;
        SetConsoleCursorInfo(_console_handle, &info);
    }

    void Print(Point p, char c)
    {
        SetConsoleCursorPosition(_console_handle, { p.x , p.y });
        std::cout.put(c);
    }

    void Clear(Point point)
    {
        Print(point, ' ');
    }
};

int main()
{
    Point point{ 10, 10 };
    PrintHelper pr;
    while(true)
    {
        pr.Print(point, '*');
        char c = _getch();
        pr.Clear(point);
        change(point, c);
    }
    return 0;
}