#include "main.h"

Utilities::Utilities() {
    myConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Utilities::ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void Utilities::Quick_Console_Size_Change(int width, int height) {
    console_width = width, console_height = height;
    stringstream ss;
    ss << "MODE CON: COLS=" << console_width << "LINES=" << console_height;
    system(ss.str().c_str());

    double_line = string (console_width,'\xCD');
    single_line = string (console_width,'\xC4');

    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    ShowConsoleCursor(0);
}

void Utilities::Set_Console_Size() {
    std::fstream inFile("tsv/console_size.tsv", std::ios_base::in);
    inFile >> console_width >> console_height;
}

void Utilities::Set_Console() {
    Set_Console_Size();

    stringstream ss;
    ss << "MODE CON: COLS=" << console_width << "LINES=" << console_height;
    system(ss.str().c_str());

    double_line = string (console_width,'\xCD');
    single_line = string (console_width,'\xC4');

    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
    ShowConsoleCursor(0);
}

vector< vector<string> > Utilities::Generate_Two_Dimensional_Vector(string file_name) {
    vector<string> imported_cells; vector< vector<string> > imported_rows;
    string delimited_line, cell; ifstream inFile(file_name.c_str());

    while(inFile.good()) {
        getline (inFile,delimited_line);
        if (delimited_line.empty()) break;
        istringstream string_stream(delimited_line);
        while (getline(string_stream, cell, '\t')) imported_cells.push_back(cell);
        imported_rows.push_back(imported_cells);
        imported_cells.clear();
    } return imported_rows;
}

void Utilities::Draw_Box(string border_title, int x_position, int y_position, bool highlight_box) {
    gotoXY(x_position + ((box_width - border_title.length()) / 2), y_position - 1);
    cout << border_title;

    int box_colour = 8;
    if (highlight_box == true) box_colour = 3;
    SetConsoleTextAttribute(myConsole, box_colour);

    gotoXY(x_position, y_position);
    cout << string (box_width, '\xDC');
    gotoXY(x_position, (y_position + box_height - 1));
    cout << string (box_width, '\xDF');

    for (int i = 0; i < (box_height - 2); i++) {
        gotoXY(x_position, i + (y_position + 1));
        cout << char ('\xBA');
        gotoXY(x_position + (box_width - 1), i + (y_position + 1));
        cout << char ('\xBA');
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

void Utilities::Yes_No_Box(string message, string button_one, string button_two) {
    print_message = message;
    button_right = button_one;
    button_left = button_two;
    ///
    int width = 100; int height = 20;
    Quick_Console_Size_Change(width, height);
    box_width = (width - height), box_height = 15;
    int box_y_position = ((height / 2) - (box_height / 2));
    int box_x_position = ((width / 2) - (box_width / 2));
    Draw_Box("Confirmation Box", box_x_position, box_y_position, 1);
    gotoXY(((width / 2) - (print_message.length() / 2)), (box_y_position + (box_height - 11)), print_message);
    bool exit_trigger = false; int c; selection = true;

    while (exit_trigger == false) {
        Print_Buttons(box_y_position, box_height);
        switch(c=getch()) {
            case 75: selection = true; break;
            case 77: selection = false; break;
            case 13: exit_trigger = true; break;
        }   Print_Buttons(box_y_position, box_height);
    }   Set_Console();
    yes_no_box_selection = selection;
};

void Utilities::Print_Buttons(int box_y_position, int box_height) {
    bool is_yes_button, is_no_button;
    selection == true ? (is_yes_button = true) : (is_yes_button = false);
    Draw_A_Button(25, (box_y_position + (box_height - 7)), button_right, is_yes_button);
    selection == false ? (is_no_button = true) : (is_no_button = false);
    Draw_A_Button(55, (box_y_position + (box_height - 7)), button_left, is_no_button);
}

void Utilities::Draw_A_Button(int x_position, int y_position, string button_name, bool selection_status) {
    int button_width = 19, button_height = 3, button_colour = 8;

    if (selection_status == 1) button_colour = 3; SetConsoleTextAttribute(myConsole, button_colour);
    gotoXY (x_position, y_position + 1);
    cout << string (button_width, '\xDB');
    gotoXY(x_position, y_position);
    cout << char ('\xDA') << string (button_width - 2,'\xDC') << char ('\xBF');
    gotoXY(x_position, (button_height + y_position - 1));
    cout << char ('\xC0') << string (button_width - 2,'\xDF') << char ('\xD9');

    if (selection_status == 1) button_colour = 63; else button_colour = 143;
    SetConsoleTextAttribute(myConsole, button_colour);
    gotoXY (x_position + ((button_width - button_name.length()) / 2), y_position + 1);
    cout << button_name;

    SetConsoleTextAttribute(myConsole, 15);
}

int Utilities::Return_Last_Key_Press() {
    return key_press;
}
