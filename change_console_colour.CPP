#include "main.h"

Change_Console_Colour::Change_Console_Colour() {
    Set_Console(); system("CLS");
    Prepare_Control();
    while(exit_trigger == false) {
        Print();
        key_press=getch();
        Handle_Key_Press();
    }
}

void Change_Console_Colour::Prepare_Control() {
    vector< vector<string> > transfer_data = Generate_Two_Dimensional_Vector("tsv/system/config.tsv");
    for (int i = 0; i < 8; i++) {
        fields.push_back(transfer_data[6][i]);
        field_colour.push_back(String_To_Int(transfer_data[7][i]));
        text_colour.push_back(String_To_Int(transfer_data[8][i]));
    }   exit_trigger = false; selection = 0;
}

int Change_Console_Colour::String_To_Int(string string_to_convert) {int x; istringstream(string_to_convert) >> x; return x;}

void Change_Console_Colour::Handle_Key_Press() {
    switch(key_press) {
        case 72: selection--; break;
        case 80: selection++; break;
        case 75: if (selection > 4) selection -= 4; else selection += 4; break;
        case 77: if (selection < 5) selection += 4; else selection -= 4; break;
        case 8:  exit_trigger = true; break;
        case 13: Set_Console_Colour(); exit_trigger = true; break;
    }   if (selection < 0) selection = 7; else if (selection > 7) selection = 0;
}

void Change_Console_Colour::Set_Console_Colour() {
    ofstream console_size; console_size.open("tsv/system/console_colour.tsv", ios_base::trunc);
        console_size << field_colour[selection] << "\n" << text_colour[selection];
        console_size.close();
}

void Change_Console_Colour::Print() {
    string menu_title = "<[   C h a n g e   C o n s o l e   C o l o u r   ]>";
    gotoXY(0, 1, double_line, 0); gotoXY(0, 5, double_line, 0);
    gotoXY(((console_width / 2) - (menu_title.length() / 2)), 3, menu_title, 0);

    string footer_message = "( Press [BACK-SPACE] to return to menu or [ENTER] to make selection )";
    gotoXY(0, console_height - 2, single_line, 0);
    gotoXY(((console_width / 2) - (footer_message.length() / 2)), console_height - 2, footer_message, 0);

    box_y_position = 8, box_width = 80, box_height = 19, box_space = ((console_width - (box_width)) / 2);
    Draw_Box("Set Console Colour", box_space, box_y_position, true);

    bool status = true; int y_position = 6, x_position = (box_space + 15), field_spacing = 4;
    for (int i = 0; i < 8; i++) {
        if (selection == i) status = true; else status = false;
        loop_value = i;
        y_position += field_spacing;
        Draw_Fields(x_position, y_position, status, true);
        if(i == 3) {x_position += 28; y_position = 6;}
    }
}


void Change_Console_Colour::Draw_Fields(int x_position, int y_position, bool status, bool is_not_button) { // (and button)
    int width = 22, height = 3, colour = field_colour[loop_value]; if (status == 1) colour = 0;
    SetConsoleTextAttribute (myConsole, colour);
    gotoXY (x_position, y_position + 1); cout << string (width, '\xDB');

    gotoXY (x_position, y_position);
    if (is_not_button == true) cout << string (width,'\xDC');
    else cout << char('\xDA') << string (width - 2,'\xDC') << char('\xBF');

    gotoXY(x_position, (height + y_position - 1));
    if (is_not_button == true) cout << string (width,'\xDF');
    else cout << char('\xC0') << string (width - 2,'\xDF') << char('\xD9');

    colour = text_colour[loop_value]; if (status == 1) colour = 15;
    SetConsoleTextAttribute (myConsole, colour);
    gotoXY (x_position + ((width - fields[loop_value].length()) / 2), y_position + 1); cout << fields[loop_value] << " ";
    SetConsoleTextAttribute(myConsole, 15);
}
