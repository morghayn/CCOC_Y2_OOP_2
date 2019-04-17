#include "utilities.h"

HANDLE myConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;


void Utilities::gotoXY(int x, int y) {
	CursorPosition.X = x; CursorPosition.Y = y;
	SetConsoleCursorPosition(myConsole,CursorPosition);
}


void Utilities::gotoXY(int x, int y, char text) {
	CursorPosition.X = x; CursorPosition.Y = y;
	SetConsoleCursorPosition(myConsole,CursorPosition);
	cout << text;
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


void Utilities::ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}


vector< vector<string> > Utilities::Generate_Two_Dimensional_Vector(string file_name) {
    vector<string> imported_cells; vector< vector<string> > imported_rows;
    string delimited_line, cell; ifstream inFile(file_name.c_str());

    while(inFile.good()) {
        getline(inFile,delimited_line); if(delimited_line.empty()) break;
        istringstream string_stream(delimited_line);
        while(getline(string_stream, cell, '\t')) imported_cells.push_back(cell);
        imported_rows.push_back(imported_cells); imported_cells.clear();
    } return imported_rows;
}


void Utilities::Set_Console() {
    console_column_quantity = 140, console_row_quantity = 30, vertical_outset = 3;
    stringstream ss; ss << "MODE CON: COLS=" << console_column_quantity << "LINES=" << console_row_quantity;
    string console_size = ss.str(); system(console_size.c_str());
    double_line = string (console_column_quantity,'\xCD');
    single_line = string (console_column_quantity,'\xC4');

    ShowConsoleCursor(0); HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}
