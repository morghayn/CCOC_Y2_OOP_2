#include "main.h"

Change_Console_Size::Change_Console_Size() {
    system("CLS");
    Set_Console();
    int c = 0;
    bool exit_trigger = 0;
    width = console_width;
    min_width = 140;
    max_width = 190;
    Print();
    while(exit_trigger == 0) {
        switch(c=getch()) {
            case 75: Selection_Value(75); break;
            case 77: Selection_Value(77); break;
            case 13: exit_trigger = true; break;
            case 8:  exit_trigger = true; break;
        }
    }
}

void Change_Console_Size::Selection_Value(int key_value) {
    if (key_value == 75) width--;
    if (key_value == 77) width++;
    Print();
}

void Change_Console_Size::Print() {
    int box_y_position = 8; box_width = (max_width - 40), box_height = 3; int box_x_position = 0;
    float myFloat = (box_width / (max_width - min_width));
    float print_distance = (myFloat * (width - min_width));
    int print_int = print_distance;
    gotoXY(0,0); cout << print_int << "\t" << width << "\t" << box_width << "\t" << myFloat;
    gotoXY(box_x_position + 1, box_y_position + 1); cout << string (box_width - 2, ' ');

    if(print_int > box_width - 2) print_int = box_width - 2;
    gotoXY(box_x_position + 1, box_y_position + 1); cout << string (print_int, '\xDB');
    Draw_Box("Primary Buttons", 0, box_y_position, true);
    ///cin.get();
}
