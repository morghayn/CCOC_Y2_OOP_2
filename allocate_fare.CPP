#include "main.h"
//#include "utilities.cpp"

//int main() {Allocate_Fare Load; return 0;}


Allocate_Fare::Allocate_Fare() {

    int c = 0; exit_trigger = 0; selection = 0;
    field_name = Generate_Two_Dimensional_Vector("tsv/config.tsv");
    Set_Console(); system("CLS"); text_input_mode = false; Print();

    while(exit_trigger == 0) {
        switch(c=getch()) {
            case 72: Selection_Value(72); break;
            case 80: Selection_Value(80); break;
            case 75: Selection_Value(75); break;
            case 77: Selection_Value(77); break;
            case 13: if (selection < 7) text_input_mode = true; else exit_trigger = 1; break;
            case 8:  break;
        } Print();
    }
}


void Allocate_Fare::Print() {
    if (text_input_mode == true) system("CLS");
    Add_Menu_Title();
    Update_Boxes();
    Update_Fields();
    if (text_input_mode == true) Text_Input_Functionality(selection);
}


void Allocate_Fare::Selection_Value(int key_value) {
        switch(key_value) {
            // 72 = Key Up // 80 = Key Down // 75 = Key Left // 77 = Key Right
            case 72: selection--; break;
            case 80: selection++; break;
            case 75: if (selection > 4) selection -= 4; else selection += 4; break;
            case 77: if (selection < 5) selection += 4; else selection -= 4; break;
        }   if (selection < 0) selection = 7; else if (selection > 7) selection = 0;
}


void Allocate_Fare::Add_Menu_Title() {
    string menu_title = "<[   A l l o c a t e   F a r e   ]>";
    gotoXY(0, 1, double_line, 0); gotoXY(0, 5, double_line, 0);
    gotoXY(((console_column_quantity / 2) - (menu_title.length() / 2)), 3, menu_title, 0);
}


void Allocate_Fare::Update_Boxes() {
    bool highlight_box; int box_y_position;


    if (text_input_mode == true) {
        box_y_position = 8; box_width = 114, box_height = 11;
        int box_spacing = ((console_column_quantity - (box_width)) / 2); box_x_position[0] = box_spacing;
        highlight_box = false; Draw_Box("Fare Details", box_x_position[0], box_y_position, highlight_box);

        box_y_position = 22, box_width = 57, box_height = 5;
        box_spacing = ((console_column_quantity - (box_width)) / 2); box_x_position[1] = box_spacing;
        highlight_box = true; Draw_Box("Input Field", box_x_position[1], box_y_position, highlight_box);
    }


    if (text_input_mode == false) {
        box_y_position = 8; box_width = 80, box_height = 19;
        int box_spacing = ((console_column_quantity - (box_width)) / 2); box_x_position[0] = box_spacing;
        highlight_box = true; Draw_Box("Fare Details", box_x_position[0], box_y_position, highlight_box);
    }
}


void Allocate_Fare::Update_Fields() {
    int selection_status = 1, button_x_position = (box_x_position[0] + 4), button_y_position = 10, button_spacing = 5;
    int field_spacing = 4;

    if (text_input_mode == true) {
        int selection_status = 1, x_position = (box_x_position[0] + 4);

        for (int i = 0; i < 7; i++) {
            if (selection == i) selection_status = 1;
            else selection_status = 0;

            int y_position = 10; if (i % 2 != 0) y_position += field_spacing;

            field_value = i; Draw_Field(x_position, y_position, selection_status);
            if(i == 1 || i == 3 || i == 5) x_position += 28;
        }

        field_value = 7;
        if (selection == 7) selection_status = 1; else selection_status = 0;
        Draw_Button(box_x_position[0] + 88, 14, selection_status);
    }



    if (text_input_mode == false) {
        int y_position = 6, selection_status = 1, x_position = (box_x_position[0] + 15);

        for (int i = 0; i < 7; i++) {
            if (selection == i) selection_status = 1;
            else selection_status = 0;

            y_position += field_spacing;
            field_value = i;
            Draw_Field(x_position, y_position, selection_status);

            if(i == 3) {
                x_position += 28; y_position = 6;
            }
        }

        field_value = 7;
        if (selection == 7) selection_status = 1; else selection_status = 0;
        Draw_Button(box_x_position[0] + 43, 22, selection_status);
    }
}

void Allocate_Fare::Text_Input_Functionality(int selected) {

    string test = field_name[3][selected];

    int box_y_position = 24, box_width = 57, box_height = 5;
    int box_spacing = ((console_column_quantity - (box_width)) / 2); box_x_position[1] = box_spacing;


    int c = 0;
    while(c=getch()) {
        if (c == 8 && test.size() > 0) {
            test.erase(test.size() - 1);
            gotoXY(box_x_position[1] + 1, box_y_position, string (box_width - 2, ' '));
            gotoXY(box_x_position[1] + ((box_width - test.length()) / 2), box_y_position, test, 0);
            gotoXY(box_x_position[1] + ((box_width - test.length()) / 2) + test.length(), box_y_position, "_", 0);
        }
        else if (test.size() < 19) {
            stringstream ss;
            ss << test << char(c);
            test = ss.str();
            gotoXY(box_x_position[1] + ((box_width - test.length()) / 2), box_y_position, test, 0);
            gotoXY(box_x_position[1] + ((box_width - test.length()) / 2) + test.length(), box_y_position, "_", 0);
        }
        field_name[3][selected] = test;
    }
}


void Allocate_Fare::Draw_Field(int x_position, int y_position, bool selection_status) {
    int button_width = 22, button_height = 3, button_colour = 8;

    if (selection_status == 1) button_colour = 3; SetConsoleTextAttribute(myConsole, button_colour);
    gotoXY(x_position, y_position + 1);
    cout << string(button_width, '\xDB');
    gotoXY(x_position, y_position);
    cout << string(button_width,'\xDC');
    gotoXY(x_position, (button_height + y_position - 1));
    cout << string(button_width,'\xDF');

    if (selection_status == 1) button_colour = 63; else button_colour = 143;
    SetConsoleTextAttribute(myConsole, button_colour);
    gotoXY(x_position + ((button_width - field_name[3][field_value].length()) / 2), y_position + 1);
    cout << field_name[3][field_value];

    SetConsoleTextAttribute(myConsole, 15);
}


void Allocate_Fare::Draw_Button(int x_position, int y_position, bool selection_status) {
    int button_width = 22, button_height = 3, button_colour = 8;

    if (selection_status == 1) button_colour = 3; SetConsoleTextAttribute(myConsole, button_colour);
    gotoXY(x_position, y_position + 1);
    cout << string (button_width, '\xDB');
    gotoXY(x_position, y_position);
    cout << char('\xDA') << string(button_width - 2,'\xDC') << char('\xBF');
    gotoXY(x_position, (button_height + y_position - 1));
    cout << char('\xC0') << string(button_width - 2,'\xDF') << char('\xD9');

    if (selection_status == 1) button_colour = 63; else button_colour = 143;
    SetConsoleTextAttribute(myConsole, button_colour);
    gotoXY(x_position + ((button_width - field_name[3][field_value].length()) / 2), y_position + 1);
    cout << field_name[3][field_value];

    SetConsoleTextAttribute(myConsole, 15);
}
