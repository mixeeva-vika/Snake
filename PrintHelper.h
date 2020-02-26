#pragma once
#include <iostream>
#include <conio.h>
#include"Point.h"
namespace
{
#include <windows.h>
}
using namespace std;

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