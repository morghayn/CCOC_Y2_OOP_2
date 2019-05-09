#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <numeric>

#define _WIN32_WINNT 0x0500
#include <windows.h>

using namespace std;

class Utilities {
    public:
        COORD cursor_position;
        Utilities();
        void Get_Colour();
        void Set_Console_Sizes();
        void Set_Console();
        void Set_Console(int width, int height);
        void ShowConsoleCursor(bool showFlag);

        vector< vector<string> > Generate_Two_Dimensional_Vector(string file_name);
        void Draw_Box(string border_title, int x_position, int y_position, bool highlight_box);
        void Yes_No_Box(string message, string button_one, string button_two, bool is_confirmation_box);
        void Print_Yes_No_Box_Buttons(int box_y_position, int box_height, bool is_confirmation_box);
        void Draw_A_Button(int x_position, int y_position, string button_name, bool selection_status);

        void gotoXY(int x, int y);
        void gotoXY(int x, int y, char text);
        void gotoXY(int x, int y, string text);
        void gotoXY(int x, int y, string text, bool highlight);
        int Last_Key_Press();

    protected:
        int console_width, console_height, box_width, box_height, key_press, console_colour, text_colour;
        string double_line, single_line, button_right, button_left, print_message, Get_Todays_Date();
        bool selection, yes_no_box_selection;
        HANDLE myConsole;
};

class MainMenu: public Utilities {
    public:
        MainMenu();
        void Print_Descriptors();
        void Print();
        void Handle_Key_Press();
        void Selection_Value();
        void Execute();
        void Update_Button_Description();
        void Update_Boxes();
        void Update_Buttons();
        void Draw_Button(int x_position, int y_position, bool selection_status);

    private:
        bool exit_trigger, primary_selected;
        vector< vector<string> > button_name, button_descriptions;
        int primary, secondary, box_x_position[3], selection[3];
};

class Print_TSV_Table: public Utilities {
    public:
        Print_TSV_Table(string file_name, string title, bool selection_table);
        void Prepare_Table();
        void Handle_Key_Press();
        void Change_Page_Request(int key_value, bool is_table_load);
        void Change_Record_Selection(int key_value);
        void Print_Descriptors();
        void Print_Table_Grid(int x);
        void Print_Cells(int x);

        void Create_Table_Pages();
        void Determine_Column_Spacing();
        int * Determine_Longest_Cells_In_Columns();

        int Get_Record_Location(int record_location);
        void Highlight_Selected_Record(int record_location);
        void Remove_Last_Highlighted_Record(int record_location);
        vector<string> Return_Highlighted_Record(); void Submit_Status_Change();

    private:
        int current_page, selected_record, total_record_count, page_quantity, column_space, column_quantity, record_quantity;
        string table_title; string table_file_name;
        vector<int> page_range, column_position;
        vector< vector<string> > table;
        bool selector, exit_trigger;
};

class Allocate_Fare: public Utilities {
    public:
        Allocate_Fare();
        void Prepare_Fields();
        void Handle_Key_Press();
        void Fare_Creation_Process();
        void Print(); void Print_Descriptors();

        void Set_Controls_Format();
        void Set_Input_Format(); void Input_Functionality(); void Handle_Input(int last_key_press); void Output_Input();
        void Handle_Wheelchair_Input();
        void Validate_Continue_Button();
        void Draw_Fields(int x_position, int y_position, bool status, bool is_not_button); // (and button)

    private:
        int field_value, selection, box_space, box_y_position;
        bool text_input_mode, exit_trigger, validation[8];
        vector<string> box_titles, fields; string input;
        vector<int> field_size;
        float Revenue();
};

class Terminate_Fare: public Utilities {
    public:
        Terminate_Fare();
        void Transfer_Fare_To_Log();
        void Remove_From_Active_Fares(string file_name);
        void Update_Status_To_Inactive(string file_name, int fare_column, int file_column);
        void Print_Update_To_File(string file_name, int record_quantity, int column_quantity, vector< vector<string> > table);
        void Check_Driver_Vehicle_Log();
        void Add_Entries_For_Today(string details_file_name, string log_file_name, bool is_driver_log);
        void Update_Driver_Vehicle_Entry(bool is_driver_log, int fare_column, int file_column);
        string Update_Revenue(vector< vector<string> > table);
        string Update_Kilometers(vector< vector<string> > table, int kilometer_column);

    private:
        vector<string> selected_fare;
        int record_to_update;
};

class Change_Console_Size: public Utilities {
    public:
        Change_Console_Size();
        void Selection_Value(int key_value);
        void Set_Console_Width();
        void Print_Descriptors();
        void Print();

    private:
        int width, maximum_width, minimum_width;

};

class Change_Console_Colour: public Utilities {
    public:
        Change_Console_Colour();
        void Prepare_Control();
        void Handle_Key_Press();
        void Print();
        int String_To_Int(string string_to_convert);
        void Draw_Fields(int x_position, int y_position, bool status, bool is_not_button);
        void Set_Console_Colour();

    private:
        int selection, loop_value, box_space, box_y_position;
        bool exit_trigger;
        vector<string> fields; string input;
        vector<int> field_colour, text_colour;
};

#endif // MAIN_H
