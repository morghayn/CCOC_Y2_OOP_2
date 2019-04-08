#ifndef UTILITIES_H
#define UTILITIES_H
#include <iostream>
using namespace std;

class Utilities {
    public:
        void Draw(int style, int col, int row, int length,int amount);
        void gotoXY(int x, int y);
        void gotoXY(int x, int y, string text, bool highlight);
        void ShowConsoleCursor(bool showFlag);
};

#endif // UTILITIES_H
