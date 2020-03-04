#pragma once
#include <iostream>
#include"Point.h"
namespace win
{
#include <conio.h>
#include <windows.h>
}
using namespace std;

class PrintHelper
{
    win::HANDLE _console_handle;
    win::CONSOLE_CURSOR_INFO info;

public:
    PrintHelper()
    {
        using namespace win;
        _console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        info.bVisible = false;
        info.dwSize = 100;
        SetConsoleCursorInfo(_console_handle, &info);
    }

    void Print(Point p, char c)
    {
        win::SetConsoleCursorPosition(_console_handle, { p.x , p.y });
        std::cout.put(c);
    }

    void Clear(Point point)
    {
        Print(point, ' ');
    }
};