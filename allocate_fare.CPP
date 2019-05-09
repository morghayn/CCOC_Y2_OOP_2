#include "main.h"

Allocate_Fare::Allocate_Fare() {
    Set_Console(); system("CLS");
    Prepare_Fields();
    while(exit_trigger == false) {
        Print();
        key_press=getch();
        Handle_Key_Press();
    }
}

void Allocate_Fare::Prepare_Fields() {
    vector< vector<string> > transfer_data = Generate_Two_Dimensional_Vector("tsv/system/config.tsv");
    for (int i = 0; i < 8; i++) {fields.push_back(transfer_data[3][i]); validation[i] = false;}
    for (int i = 0; i < 6; i++) {
        int x; box_titles.push_back("Input " + fields[i]);
        istringstream(transfer_data[4][i]) >> x;
        field_size.push_back(x);
    }   text_input_mode = false, exit_trigger = false; selection = 0;
}

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
                if (validation[7] == false) {
                    Yes_No_Box("You must validate all fields before you can continue.", "Confirm", "", true);
                    return;
                }
                Fare_Creation_Process();
                system("CLS"); Set_Console(); Print();
            } break;
        }
    }   if (selection < 0) selection = 7; else if (selection > 7) selection = 0;
    // 72 = Up // 80 = Down // 75 = Left // 77 = Right // 8 = Backspace // 13 = Enter
}

void Allocate_Fare::Fare_Creation_Process() {
    // selecting driver and vehicle
    Print_TSV_Table drivers("tsv/drivers.tsv", "<[   S e l e c t   D r i v e r   ]>", true);
    if (drivers.Last_Key_Press() == 8) return;
    Print_TSV_Table vehicles("tsv/vehicles.tsv", "<[   S e l e c t   V e h i c l e   ]>", true);
    if (vehicles.Last_Key_Press() == 8) return;

    // changing status of driver and vehicle to active then returning selected record from each
    drivers.Submit_Status_Change(); vehicles.Submit_Status_Change();
    vector<string> selected_driver = drivers.Return_Highlighted_Record(), selected_vehicle = vehicles.Return_Highlighted_Record();

    // printing fair details, driver number and vehicle registration to active_fares.tsv
    ofstream active_fare; active_fare.open("tsv/active_fares.tsv", ios_base::app);
    for (int i = 0; i < 7; i++) active_fare << fields[i] << "\t";
    active_fare << selected_driver[0] << "\t" << selected_vehicle[1] << "\t" << Revenue() << "\t" << endl; active_fare.close();
    exit_trigger = 1; // exiting allocation process
}

float Allocate_Fare::Revenue() {
    float calc; istringstream(fields[5]) >> calc;
    calc = calc * 6.5; calc += 5;
    return calc;
}

void Allocate_Fare::Print_Descriptors() {
    string menu_title = "<[   A l l o c a t e   F a r e   ]>";
    gotoXY(0, 1, double_line, 0); gotoXY(0, 5, double_line, 0);
    gotoXY(((console_width / 2) - (menu_title.length() / 2)), 3, menu_title, 0);

    string footer_message = "( Press [BACK-SPACE] to return to menu or [ENTER] to make selection )";
    gotoXY(0, console_height - 2, single_line, 0);
    gotoXY(((console_width / 2) - (footer_message.length() / 2)), console_height - 2, footer_message, 0);
}


/// ~-~-~-~-~ <[  P r i n t i n g   S e l e c t i o n   F o r m a t  ]> ~-~-~-~-~

void Allocate_Fare::Print() {
    if (text_input_mode == true) {system("CLS"); Set_Console(100, 20);}
    Print_Descriptors();
    if (text_input_mode == false) Set_Controls_Format();
    else Set_Input_Format();
}

void Allocate_Fare::Set_Controls_Format() {
    box_y_position = 8, box_width = 80, box_height = 19, box_space = ((console_width - (box_width)) / 2);
    Draw_Box("Fare Details", box_space, box_y_position, true);

    int y_position = 6, x_position = (box_space + 15), field_spacing = 4; bool status = true;
    for (int i = 0; i < 7; i++) {
        if (selection == i) status = true; else status = false;
        field_value = i;
        y_position += field_spacing;
        Draw_Fields(x_position, y_position, status, true);
        if(i == 3) {x_position += 28; y_position = 6;}
    }

    field_value = 7; if (selection == 7) status = true; else status = false;
    Draw_Fields(box_space + 43, 22, status, false);
}

void Allocate_Fare::Draw_Fields(int x_position, int y_position, bool status, bool is_not_button) { // (and button)
    Get_Colour(); int width = 22, height = 3, colour = console_colour; if (field_value < 8 && validation[field_value] == false) colour = 8; if (status == 1) colour = 0;
    SetConsoleTextAttribute (myConsole, colour);
    gotoXY (x_position, y_position + 1); cout << string (width, '\xDB');

    gotoXY (x_position, y_position);
    if (is_not_button == true) cout << string (width,'\xDC');
    else cout << char('\xDA') << string (width - 2,'\xDC') << char('\xBF');

    gotoXY(x_position, (height + y_position - 1));
    if (is_not_button == true) cout << string (width,'\xDF');
    else cout << char('\xC0') << string (width - 2,'\xDF') << char('\xD9');

    if (field_value < 8 && validation[field_value] == false) colour = 143; else colour = text_colour; if (status == 1) colour = 15;
    SetConsoleTextAttribute (myConsole, colour);
    gotoXY (x_position + ((width - fields[field_value].length()) / 2), y_position + 1); cout << fields[field_value] << " ";
    SetConsoleTextAttribute(myConsole, 15);
}


/// ~-~-~-~-~ <[  P r i n t i n g   T e x t   I n p u t   F o r m a t  ]> ~-~-~-~-~

void Allocate_Fare::Set_Input_Format() {
    if (selection != 6) {
        box_width = 75, box_height = 5, box_y_position = (console_height / 2); box_space = ((console_width - (box_width)) / 2);
        Draw_Box(box_titles[selection], box_space, box_y_position, true);
        Input_Functionality();
    }   else Handle_Wheelchair_Input();
    Validate_Continue_Button(); text_input_mode = false; system("CLS"); Set_Console(); Print();
}

void Allocate_Fare::Validate_Continue_Button() {
    for (int i = 0; i < 7; i++) {
        if (validation[i] == true) validation[7] = true; else validation[7] = false;
        if (validation[i] == false) return;
    }
}

void Allocate_Fare::Input_Functionality() {
    input = fields[selection]; int last_key_press = 0;
    if (selection > 2) input = ""; Output_Input();

    while ((key_press=getch())) {
        if (key_press == 13) break;
        Handle_Input(last_key_press);
        Output_Input();
        last_key_press = key_press;
    }

    if (fields[selection] == input);
    else if (input == "" && selection < 3) validation[selection] = false;
    else if (input == "" && selection > 2) {validation[selection] = false; fields[selection] = "Invalid Entry";}
    else {fields[selection] = input; validation[selection] = true;}
}

void Allocate_Fare::Output_Input() {
    gotoXY (box_space + 1, (box_y_position + 2), string (box_width - 2, ' '));
    gotoXY (box_space + ((box_width - input.length()) / 2), (box_y_position + 2), input);
    gotoXY (box_space + ((box_width - input.length()) / 2) + input.length(), (box_y_position + 2), "_");
}

void Allocate_Fare::Handle_Input(int last_key_press) {
    if (last_key_press == 32 && key_press == 32) return; // do nothing if space was inputted twice
    else if (input == "0" && input.size() == 1 && key_press != 8 && selection != 3) return; // do nothing if numeric size is 1 and last value was '0'
    else if (key_press == 8 && input.size() > 0) input.erase(input.size() - 1); // if backspace erase input by one

    else if (input.size() < field_size[selection] && key_press != 8) {
        if (selection < 3) if (!((key_press > 64 && key_press < 123) || (key_press == 32))) return; // alphabetical check
        if (selection > 2) if (!(key_press > 47 && key_press < 58)) return; // numeric check
        stringstream ss; ss << input << char(key_press); input = ss.str(); // add char to input
    }
}

void Allocate_Fare::Handle_Wheelchair_Input() {
    Yes_No_Box("Does the fare need to be wheelchair accessible?", "No", "Yes", false);
    if (yes_no_box_selection == true) input = "No"; else input = "Yes";
    validation[selection] = true; fields[selection] = input;
}
