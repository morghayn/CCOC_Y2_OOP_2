#include "main.h"


int main() {MainMenu Load; return 0;}


MainMenu::MainMenu() {
    int c = 0; exit_trigger = 0; primary_selected = true;
    for(int i = 0; i < 3; i++) selection[i] = 0; selection[1] = 3;

    button_name = Generate_Two_Dimensional_Vector("tsv/buttons.tsv");
    system("CLS"); Set_Console(); Add_Menu_Title(); Print();

    while(exit_trigger == 0) {
        c = 0;
        switch(c=getch()) {
        case 72: Selection_Value(0); break;
        case 80: Selection_Value(1); break;
        case 13:
            primary_selected = false; if(selection[1] == 3) selection[1] = 0; break;
        case 8:
            primary_selected = true; if(selection[1] < 3) selection[1] = 3; break;
        } Print();
    }
}


void MainMenu::Selection_Value(bool key_down) {
    // selection [0] == selected primary button
    // selection [1] == selected secondary button
    // selection [2] == used to change value of either primary or secondary button selection
    if(primary_selected == true) selection[2] = selection[0]; else selection[2] = selection[1];
    if(key_down == true) selection[2]++; else selection[2]--;
    if(selection[2] == 3) selection[2] = 0; else if(selection[2] == -1) selection[2] = 2;
    if(primary_selected == true) selection[0] = selection[2]; else selection[1] = selection[2];
}


void MainMenu::Execute() {
    switch(selection[0]) {
    case 0:
        switch(selection[0]) {
        case 0: // Display Driver Details
        case 1: // Display Vehicle Details
        case 2: // Display Active Fares
        break;
        }
    case 1:
        switch(selection[1]) {
        case 0: // Terminate Fare
        case 1: // Allocate Fare
        case 2: // Generate Report
        break;
        }
    case 2:
        switch(selection[2]) {
        case 0: // Misc 1
        case 1: // Misc 2
        case 2: // Misc 3
        break;
        }
    }
}


void MainMenu::Add_Menu_Title() {
    string menu_title = "<[   C a l l - O u t   D i s p a t c h   S e r v i c e s   ]>";
    gotoXY(0, 1, double_line, 0); gotoXY(0, 5, double_line, 0);
    gotoXY(((console_column_quantity / 2) - (menu_title.length() / 2)), 3, menu_title, 0);
}


void MainMenu::Print() {
    Update_Button_Description();
    Update_Boxes();
    Update_Buttons();
}


void MainMenu::Update_Button_Description() {
    gotoXY(94, 11, "This is a sample", 0);
    gotoXY(94, 12, "button description", 0);
}


void MainMenu::Update_Boxes() {
    int box_y_position = 8, button_box_width; int highlight_box = 0;
    int box_spacing = (console_column_quantity - (27 * 3)) / 4; box_x_position[0] = box_spacing;
    for (int i = 1; i < 3; i++) box_x_position[i] = box_x_position[i - 1] + 27 + box_spacing;
    Draw_Box("Button Description", box_x_position[2], box_y_position, 0);

    if(primary_selected == true) highlight_box = 1;
    else highlight_box = 0;
    Draw_Box("Primary Buttons", box_x_position[0], box_y_position, highlight_box);

    if(primary_selected == false) highlight_box = 1;
    else highlight_box = 0;
    Draw_Box("Secondary Buttons", box_x_position[1], box_y_position, highlight_box);
}


void MainMenu::Draw_Box(string border_title, int x_position, int y_position, bool selection_status) {
    int box_width = 27, box_height = 17, box_colour = 8;
    gotoXY(x_position + ((box_width - border_title.length()) / 2), y_position - 1);
    cout << border_title;
    if (selection_status == 1) box_colour = 3; SetConsoleTextAttribute(myConsole, box_colour);

    gotoXY(x_position, y_position);
    cout << string(box_width, '\xDC');
    gotoXY(x_position, (y_position + box_height - 1));
    cout << string(box_width, '\xDF');
    for (int i = 0; i < 15; i++) {
        gotoXY(x_position, i + (y_position + 1));
        cout << char('\xBA');
        gotoXY(x_position + (box_width - 1), i + (y_position + 1));
        cout << char('\xBA');
    }   SetConsoleTextAttribute(myConsole, 15);
}


void MainMenu::Update_Buttons() {
    int selection_status = 1, x_position = (box_x_position[0] + 4), y_position = 10, button_spacing = 5;
    for (int i = 0; i < 3; i++) {
        ran_1 = i, ran_2 = 0; if (selection[0] == i) selection_status = 1; else selection_status = 0;
        Draw_Button(x_position, y_position + (button_spacing * i), selection_status);
    }   x_position = (box_x_position[1] + 4);
    for (int i = 0; i < 3; i++) {
        ran_1 = selection[0], ran_2 = i + 1; if (selection[1] == i) selection_status = 1; else selection_status = 0;
        Draw_Button(x_position, y_position + (button_spacing * i), selection_status);
    }
}


void MainMenu::Draw_Button(int x_position, int y_position, bool selection_status) {
    int button_width = 19, button_height = 3, button_colour = 8;
    if (selection_status == 1) button_colour = 3; SetConsoleTextAttribute(myConsole, button_colour);
    gotoXY(x_position, y_position + 1);
    cout << string (button_width, '\xDB');
    gotoXY(x_position, y_position);
    cout << char('\xDA') << string(button_width - 2,'\xDC') << char('\xBF');
    gotoXY(x_position, (button_height + y_position - 1));
    cout << char('\xC0') << string(button_width - 2,'\xDF') << char('\xD9');

    if (selection_status == 1) button_colour = 63; else button_colour = 143;
    SetConsoleTextAttribute(myConsole, button_colour);
    gotoXY(x_position + ((button_width - button_name[ran_1][ran_2].length()) / 2), y_position + 1);
    cout << button_name[ran_1][ran_2];
    SetConsoleTextAttribute(myConsole, 15);
}
