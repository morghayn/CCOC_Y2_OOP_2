#include "main.h"
#include "utilities.cpp"
#include "allocate_fare.cpp"
#include "print_tsv_file.cpp"

int main() {MainMenu Load; return 0;}


MainMenu::MainMenu() {
    button_name = Generate_Two_Dimensional_Vector("tsv/config.tsv");
    for(int i = 0; i < 3; i++) selection[i] = 0; selection[1] = 3;
    int c = 0; exit_trigger = 0; primary_selected = true;
    system("CLS"); Set_Console(); Add_Menu_Title(); Print();

    while(exit_trigger == 0) {
        switch(c=getch()) {
            case 72: Selection_Value(0); break;
            case 80: Selection_Value(1); break;
            case 13:
                if (primary_selected == true) {
                    primary_selected = false; if(selection[1] == 3) selection[1] = 0;
                }   else if (primary_selected == false) Execute(); break;
            case 8: primary_selected = true; if(selection[1] < 3) selection[1] = 3; break;
        } Print();
    }
}


void MainMenu::Selection_Value(bool key_down) {
    // selection [0] == primary button
    // selection [1] == secondary button
    // selection [2] == changes selection value for either
    if(primary_selected == true) selection[2] = selection[0]; else selection[2] = selection[1];
    if(key_down == true) selection[2]++; else selection[2]--;
    if(selection[2] == 3) selection[2] = 0; else if(selection[2] == -1) selection[2] = 2;
    if(primary_selected == true) selection[0] = selection[2]; else selection[1] = selection[2];
}


void MainMenu::Execute() {
    switch(selection[0]) {
        case 0:
            switch(selection[1]) {
            case 0: Print_TSV_Table("tsv/drivers.tsv", "<[   D r i v e r   D e t a i l s   ]>"); break;
            case 1: Print_TSV_Table("tsv/vehicles.tsv", "<[   V e h i c l e   D e t a i l s   ]>"); break;
            case 2: Print_TSV_Table("tsv/fares.tsv", "<[   F a r e s   D e t a i l s   ]>"); break;
            } break;
        case 1:
            switch(selection[1]) {
                case 0: break; // Terminate Fare
                case 1: {Allocate_Fare Load; break;} // Allocate Fare
                case 2: break; // Generate Report
            } break;
        case 2:
            switch(selection[1]) {
            case 0: break; // Misc 1
            case 1: break; // Misc 2
            case 2: exit_trigger = 1; break; // Exit
            } break;
    } system("CLS"); Add_Menu_Title();
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
    int box_y_position = 8, highlight_box = 0, box_spacing = (console_column_quantity - (27 * 3)) / 4;
    box_width = 27, box_height = 17; box_x_position[0] = box_spacing;
    for (int i = 1; i < 3; i++) box_x_position[i] = box_x_position[i - 1] + 27 + box_spacing;

    if(primary_selected == true) highlight_box = 1; else highlight_box = 0;
    Draw_Box("Primary Buttons", box_x_position[0], box_y_position, highlight_box);

    if(primary_selected == false) highlight_box = 1; else highlight_box = 0;
    Draw_Box("Secondary Buttons", box_x_position[1], box_y_position, highlight_box);

    highlight_box = 0;
    Draw_Box("Button Description", box_x_position[2], box_y_position, highlight_box);
}


void MainMenu::Update_Buttons() {
    int selection_status = 1, button_x_position = (box_x_position[0] + 4), button_y_position = 10, button_spacing = 5;

    for (int i = 0; i < 3; i++) {
        primary = i, secondary = 0;
        if (selection[0] == i) selection_status = 1; else selection_status = 0;
        Draw_Button(button_x_position, button_y_position + (button_spacing * i), selection_status);
    }

    button_x_position = (box_x_position[1] + 4);
    for (int i = 0; i < 3; i++) {
        primary = selection[0], secondary = i + 1;
        if (selection[1] == i) selection_status = 1; else selection_status = 0;
        Draw_Button(button_x_position, button_y_position + (button_spacing * i), selection_status);
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
    gotoXY(x_position + ((button_width - button_name[primary][secondary].length()) / 2), y_position + 1);
    cout << button_name[primary][secondary];

    SetConsoleTextAttribute(myConsole, 15);
}
