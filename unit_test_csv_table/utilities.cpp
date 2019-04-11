#include <windows.h>
#include "utilities.h"

HANDLE myConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;


void Utilities::gotoXY(int x, int y) {
	CursorPosition.X = x; CursorPosition.Y = y;
	SetConsoleCursorPosition(myConsole,CursorPosition);
}

void Utilities::gotoXY(int x, int y, string text, bool highlight) {
	CursorPosition.X = x; CursorPosition.Y = y;
	SetConsoleCursorPosition(myConsole,CursorPosition);

    int myColour = 15;
    if(highlight == 1) {myColour = 240;}
    SetConsoleTextAttribute(myConsole, myColour);
	cout << text;
    SetConsoleTextAttribute(myConsole, 15);
}
