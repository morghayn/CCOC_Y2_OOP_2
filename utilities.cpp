#include <windows.h>
#include "utilities.h"

HANDLE myConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void Utilities::Draw(int style, int col, int row, int length,int amount) {
	char tl = '\xDA', tr = '\xBF', bl = '\xC0', br = '\xD9', side = '\xB3', edge = '\xC4';
    if (style == 2) {length = length-2; tl = '\xDB', tr = '\xDB', bl = '\xDB', br = '\xDB', side = '\xDB', edge = '\xDB';}

    string Line(length,edge); gotoXY(col,row); cout << tl << Line << tr;
	for (int i = 1; i <amount-1;i++) {
		gotoXY(col,row+i); if (style == 2) cout << side << side; else cout << side;
        gotoXY(col+length,row+i); if (style == 2) cout << side << side; else cout << " " << side;
	}
	gotoXY(col,(amount+row)-1); cout << bl << Line << br;
}

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

void Utilities::ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}
