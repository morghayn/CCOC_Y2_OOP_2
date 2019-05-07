#include "main.h"

Allocate_Fare::Allocate_Fare() {
    Set_Console();
    system("CLS");
    Prepare_Fields();
    exit_trigger = false;
    while(exit_trigger == false) {
        Print();
        key_press=getch();
        Handle_Key_Press();
    }
}

void Allocate_Fare::Prepare_Fields() {
    vector< vector<string> > transfer_data = Generate_Two_Dimensional_Vector("tsv/system/config.tsv");
    for (int i = 0; i < 8; i++) fields.push_back(transfer_data[3][i]);
    for (int i = 0; i < 7; i++) box_titles.push_back(fields[i] + " Input:");
    for (int i = 0; i < 6; i++) {
        int x;
        istringstream(transfer_data[4][i]) >> x;
        field_size.push_back(x);
        ///
        ///if (transfer_data[5][i] == "1") is_alpha.push_back(true);
        ///else is_alpha.push_back(false);
    }   text_input_mode = false; selection = 0;
};

void Allocate_Fare::Handle_Key_Press() {
    switch(key_press) {
        case 72: selection--; break;
        case 80: selection++; break;
        case 75: if (selection > 4) selection -= 4; else selection += 4; break;
        case 77: if (selection < 5) selection += 4; else selection -= 4; break;
        case 8:  exit_trigger = true; break;
        case 13: {
            if (selection < 7) text_input_mode = true;
            if (selection == 7) {
                Fare_Creation_Process();
                system("CLS");
                Set_Console();
                Print();
            } break;
        }
    }   if (selection < 0) selection = 7; else if (selection > 7) selection = 0;
    /// 72 = Up // 80 = Down // 75 = Left // 77 = Right // 8 = Backspace // 13 = Enter
}

void Allocate_Fare::Fare_Creation_Process() {
    /// Selecting Driver and Vehicle
    Print_TSV_Table drivers("tsv/drivers.tsv", "<[   S e l e c t   D r i v e r   ]>", true);
    if (drivers.Return_Last_Key_Press() == 8) return;
    Print_TSV_Table vehicles("tsv/vehicles.tsv", "<[   S e l e c t   V e h i c l e   ]>", true);
    if (vehicles.Return_Last_Key_Press() == 8) return;

    /// Changing status of driver and vehicle to active then returning selected record from each
    drivers.Submit_Status_Change();
    vehicles.Submit_Status_Change();
    vector<string> selected_driver = drivers.Return_Highlighted_Record();
    vector<string> selected_vehicle = vehicles.Return_Highlighted_Record();

    /// Printing fair details, driver number and vehicle registration to active_fares.tsv
    ofstream active_fare; active_fare.open("tsv/active_fares.tsv", ios_base::app);
    for (int i = 0; i < 7; i++) active_fare << fields[i] << "\t";
    active_fare << selected_driver[0] << "\t" << selected_vehicle[1] << "\t";
    active_fare << endl; active_fare.close();
    exit_trigger = 1;
}

void Allocate_Fare::Print_Descriptors() {
    Print_Menu_Title();
    Print_Footer();
}

void Allocate_Fare::Print_Menu_Title() {
    string menu_title = "<[   A l l o c a t e   F a r e   ]>";
    gotoXY(0, 1, double_line, 0); gotoXY(0, 5, double_line, 0);
    gotoXY(((console_width / 2) - (menu_title.length() / 2)), 3, menu_title, 0);
}

void Allocate_Fare::Print_Footer() {
    string footer_message = "( Press [BACK-SPACE] to return to menu or [ENTER] to make selection )";
    gotoXY(0, console_height - 2, single_line, 0);
    gotoXY(((console_width / 2) - (footer_message.length() / 2)), console_height - 2, footer_message, 0);
}

/// ~-~-~-~-~ <[  P r i n t i n g   S e l e c t i o n   F o r m a t  ]> ~-~-~-~-~
void Allocate_Fare::Print_Selection_Box() {
    box_y_position = 8; box_width = 80, box_height = 19;
    box_space = ((console_width - (box_width)) / 2);
    Draw_Box("Fare Details", box_space, box_y_position, true);
}

void Allocate_Fare::Print_Selection_Controls() {
    int y_position = 6, selection_status = 1, x_position = (box_space + 15), field_spacing = 4;
    for (int i = 0; i < 7; i++) {
        if (selection == i) selection_status = 1; else selection_status = 0;
        y_position += field_spacing; field_value = i;
        Draw_Fields_And_Button(x_position, y_position, selection_status, 1);
        if(i == 3) {x_position += 28; y_position = 6;}
    }
    field_value = 7;
    if (selection == 7) selection_status = 1; else selection_status = 0;
    Draw_Fields_And_Button(box_space + 43, 22, selection_status, 0);
}

void Allocate_Fare::Print() {
    if (text_input_mode == true) {
        system("CLS");
        Quick_Console_Size_Change(130, 20);
    }   Print_Descriptors();
    if (text_input_mode == false) {
        Print_Selection_Box();
        Print_Selection_Controls();
    }
    if (text_input_mode == true) Format_Print_For_Text_Input();
}

/// ~-~-~-~-~ <[  P r i n t i n g   T e x t   I n p u t   F o r m a t  ]> ~-~-~-~-~
void Allocate_Fare::Format_Print_For_Text_Input() {
    box_width = 75, box_height = 5;
    box_y_position = (console_height / 2);
    box_space = ((console_width - (box_width)) / 2);
    Draw_Box(box_titles[selection], box_space, box_y_position, true);
    ///
    Text_Input_Functionality();
    Set_Console();
    Print();
}

void Allocate_Fare::Text_Input_Functionality() {
    input = fields[selection];

    if (selection == 6) { /// Handling Wheelchair
        if (input == "Yes") input = "No"; else input = "Yes";
        fields[selection] = input;
        text_input_mode = false;
        return;
    }

    if (selection > 2) input = "1";
    Output_Text_Input();
    int last_key_press = 0;

    while ((key_press=getch())) {
        if (key_press == 13) {
            text_input_mode = false;
            break;
        }
        Handle_Text_Input(box_y_position, last_key_press);
        Output_Text_Input();
        last_key_press = key_press;
    }

    if (fields[selection] == input || input == ""); else fields[selection] = input;
}

void Allocate_Fare::Output_Text_Input() {
    gotoXY (box_space + ((box_width - input.length()) / 2), (box_y_position + 2), input, 0);
    gotoXY (box_space + ((box_width - input.length()) / 2) + input.length(), (box_y_position + 2), "_", 0);
}

void Allocate_Fare::Handle_Text_Input(int box_y_position, int last_key_press) {
    if (last_key_press == 32 && key_press == 32) return; /// do nothing if space was inputted twice
    else if (input == "0" && input.size() == 1 && key_press != 8) return; /// do nothing if numeric size is 1 and last value was '0'

    else if (key_press == 8 && input.size() > 0) {
        input.erase(input.size() - 1);
        gotoXY (box_space + 1, (box_y_position + 2), string (box_width - 2, ' '));
    }
    else if (input.size() < field_size[selection] && key_press != 8) {
        if (selection < 3) if (!((key_press > 64 && key_press < 123) || (key_press == 32))) return; // must be alphabetical
        if (selection > 2) if (!(key_press > 47 && key_press < 58)) return; // must be numeric
        stringstream ss; ss << input << char(key_press);
        input = ss.str();
    }
}

/// ~-~-~-~-~ <[  D r a w i n g   F i e l d s   A n d   B u t t o n s   ]> ~-~-~-~-~
void Allocate_Fare::Draw_Fields_And_Button(int x_position, int y_position, bool selection_status, bool is_not_button) {
    int button_width = 22, button_height = 3, button_colour = 8;
    if (selection_status == 1) button_colour = 3;
    SetConsoleTextAttribute (myConsole, button_colour);

    gotoXY (x_position, y_position + 1);
    cout << string (button_width, '\xDB');

    gotoXY (x_position, y_position);
    if (is_not_button == true) cout << string (button_width,'\xDC');
    else cout << char('\xDA') << string (button_width - 2,'\xDC') << char('\xBF');

    gotoXY(x_position, (button_height + y_position - 1));
    if (is_not_button == true) cout << string (button_width,'\xDF');
    else cout << char('\xC0') << string (button_width - 2,'\xDF') << char('\xD9');

    if (selection_status == 1) button_colour = 63; else button_colour = 143;
    SetConsoleTextAttribute (myConsole, button_colour);
    gotoXY (x_position + ((button_width - fields[field_value].length()) / 2), y_position + 1);
    cout << fields[field_value];

    SetConsoleTextAttribute(myConsole, 15);
} /// 217 lines before validation accomplished
