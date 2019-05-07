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
        void Set_Console();
        void Set_Console_Size();
        void gotoXY(int x, int y);
        void gotoXY(int x, int y, char text);
        void gotoXY(int x, int y, string text);
        void gotoXY(int x, int y, string text, bool highlight);
        void Quick_Console_Size_Change(int width, int height);
        void ShowConsoleCursor(bool showFlag);
        vector< vector<string> > Generate_Two_Dimensional_Vector(string file_name);
        void Draw_Box(string border_title, int x_position, int y_position, bool highlight_box);
        void Yes_No_Box(string message, string button_one, string button_two);
        void Print_Buttons(int box_y_position, int box_height);
        void Draw_A_Button(int x_position, int y_position, string button_name, bool selection_status);
        int Return_Last_Key_Press();

    protected:
        HANDLE myConsole;
        string double_line, single_line;
        int console_width, console_height, box_width, box_height, key_press;
        bool selection; bool yes_no_box_selection;
        string button_right, button_left, print_message;
};

class Print_TSV_Table: public Utilities {
    public:
        Print_TSV_Table(string file_name, string title, bool selection_table);
        void Determine_Column_Spacing(); int * Determine_Longest_Cells_In_Columns();
        void Create_Table_Pages();
        void Print_Table_Descriptors();
        void Print_Table_Grid(int x);
        void Print_Cells(int x);
        void Change_Page_Request(int key_value, bool is_table_load);
        ///
        void Change_Record_Selection(int key_value);
        int Get_Record_Location(int record_location);
        void Highlight_Selected_Record(int record_location);
        void Remove_Last_Highlighted_Record(int record_location);
        vector<string> Return_Highlighted_Record();
        void Submit_Status_Change();

    private:
        int current_page, selected_record, total_record_count, page_quantity, column_space, column_quantity, record_quantity;
        bool selector, exit_trigger; string table_title; string table_file_name;
        vector< vector<string> > table; vector<int> page_range, column_position;
};

class Allocate_Fare: public Utilities {
    public:
        Allocate_Fare();
        void Prepare_Fields();
        void Handle_Key_Press();
        void Fare_Creation_Process();
        void Print();

        void Print_Descriptors();
        void Print_Menu_Title();
        void Print_Footer();

        void Print_Selection_Box();
        void Print_Selection_Controls();
        void Format_Print_For_Text_Input();
        void Text_Input_Functionality();
        void Handle_Text_Input(int box_y_position, int last_key_press);
        void Draw_Fields_And_Button(int x_position, int y_position, bool selection_status, bool is_not_button);
        void Output_Text_Input();

    private:
        int field_value, selection, box_space, box_y_position;
        bool text_input_mode, exit_trigger, button_validation[7];
        string input; vector<string> box_titles, fields;
        vector<int> field_size; vector<bool> is_alpha;
};

class Change_Console_Size: public Utilities {
    public:
        Change_Console_Size();
        void Selection_Value(int key_value);
        void Print();

    private:
        float width, max_width, min_width;

};

class MainMenu: public Utilities {
    public:
        MainMenu();
        void Print_Descriptors();
        void Print_Title();
        void Print_Footer();
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

#endif // MAIN_H
