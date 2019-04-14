#ifndef UTILITIES_H
#define UTILITIES_H
#include <iostream>
using namespace std;

class Utilities {
    public:
        void gotoXY(int x, int y);
        void gotoXY(int x, int y, char text);
        void gotoXY(int x, int y, string text, bool highlight);
};

#endif // UTILITIES_H
