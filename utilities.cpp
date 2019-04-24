#include "main.h"

HANDLE myConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursor_position;


void Utilities::ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}


void Utilities::Set_Console() {
    console_column_quantity = 140, console_row_quantity = 30; stringstream ss;
    ss << "MODE CON: COLS=" << console_column_quantity << "LINES=" << console_row_quantity;
    system(ss.str().c_str());

    double_line = string (console_column_quantity,'\xCD');
    single_line = string (console_column_quantity,'\xC4');

    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    ShowConsoleCursor(0);
    table_start = 3;
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


void Utilities::Draw_Box(string border_title, int x_position, int y_position, bool highlight_box) {
    gotoXY(x_position + ((box_width - border_title.length()) / 2), y_position - 1);
    cout << border_title; int box_colour = 8;

    if (highlight_box == true) box_colour = 3; SetConsoleTextAttribute(myConsole, box_colour);
    gotoXY(x_position, y_position);
    cout << string(box_width, '\xDC');
    gotoXY(x_position, (y_position + box_height - 1));
    cout << string(box_width, '\xDF');

    for (int i = 0; i < (box_height - 2); i++) {
        gotoXY(x_position, i + (y_position + 1));
        cout << char('\xBA');
        gotoXY(x_position + (box_width - 1), i + (y_position + 1));
        cout << char('\xBA');
    }   SetConsoleTextAttribute(myConsole, 15);
}


void Utilities::gotoXY(int x, int y) {
	cursor_position.X = x; cursor_position.Y = y;
	SetConsoleCursorPosition(myConsole,cursor_position);
}

void Utilities::gotoXY(int x, int y, char text) {
	cursor_position.X = x; cursor_position.Y = y;
	SetConsoleCursorPosition(myConsole,cursor_position); cout << text;
}

void Utilities::gotoXY(int x, int y, string text) {
	cursor_position.X = x; cursor_position.Y = y;
	SetConsoleCursorPosition(myConsole,cursor_position); cout << text;
}

void Utilities::gotoXY(int x, int y, string text, bool highlight) {
	cursor_position.X = x; cursor_position.Y = y;
	SetConsoleCursorPosition(myConsole,cursor_position);

    int myColour = 15; if(highlight == 1) myColour = 240;
    SetConsoleTextAttribute(myConsole, myColour); cout << text;
    SetConsoleTextAttribute(myConsole, 15);
}
