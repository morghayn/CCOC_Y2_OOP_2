#include "main.h"

Change_Console_Size::Change_Console_Size() {
    system("CLS"); Set_Console(); width = console_width;
    system("CLS"); Set_Console(100, 20);
    bool exit_trigger = 0; minimum_width = 145; maximum_width = 190;
    Print_Descriptors(); Print();
    while(exit_trigger == 0) {
        switch(key_press=getch()) {
            case 75: Selection_Value(75); break;
            case 77: Selection_Value(77); break;
            case 13: Set_Console_Width(); exit_trigger = true; break;
            case 8:  exit_trigger = true; break;
        }
    }
}

void Change_Console_Size::Selection_Value(int key_value) {
    if (key_value == 75) width--; else if (key_value == 77) width++;
    if (width < minimum_width) width = minimum_width; else if (width > maximum_width) width = maximum_width;
    Print();
}

void Change_Console_Size::Print() {
    box_width = 92, box_height = 3; int box_y_position = 10, box_x_position = ((console_width / 2) - (box_width / 2));

    gotoXY(box_x_position + 1, box_y_position + 1); cout << string(90, ' ');
    gotoXY(box_x_position + 1, box_y_position + 1); cout << string(((width - minimum_width) * 2), '\xDB');
    stringstream ss; ss << "Console Width: " << width;
    Draw_Box(ss.str(), box_x_position, box_y_position, true);
}

void Change_Console_Size::Print_Descriptors() {
    string menu_title = "<[   C h a n g e   C o n s o l e   S i z e   ]>";
    gotoXY(0, 1, double_line, 0); gotoXY(0, 5, double_line, 0);
    gotoXY(((console_width / 2) - (menu_title.length() / 2)), 3, menu_title, 0);

    string footer_message = "( Use [ARROW KEYS] to change size, [ENTER] to confirm or [BACK-SPACE] to exit. )";
    gotoXY(0, console_height - 3, single_line, 0);
    gotoXY(((console_width / 2) - (footer_message.length() / 2)), console_height - 3, footer_message, 0);
}

void Change_Console_Size::Set_Console_Width() {
    ofstream console_size; console_size.open("tsv/system/console_size.tsv", ios_base::trunc);
        console_size << width << "\n" << 30;
        console_size.close();
}
