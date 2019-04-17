#ifndef UTILITIES_H
#define UTILITIES_H
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#define _WIN32_WINNT 0x0500
#include <windows.h>

using namespace std;

class Utilities {
    public:
        void Set_Console();
        void gotoXY(int x, int y);
        void gotoXY(int x, int y, char text);
        void gotoXY(int x, int y, string text, bool highlight);
        void ShowConsoleCursor(bool showFlag);
        vector< vector<string> > Generate_Two_Dimensional_Vector(string file_name);
    protected:
        string double_line, single_line;
        int console_column_quantity, console_row_quantity, column_spacing, vertical_outset;
};

#endif // UTILITIES_H
