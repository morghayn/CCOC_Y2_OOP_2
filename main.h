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
        void Set_Console();
        void gotoXY(int x, int y);
        void gotoXY(int x, int y, char text);
        void gotoXY(int x, int y, string text);
        void gotoXY(int x, int y, string text, bool highlight);
        void ShowConsoleCursor(bool showFlag);
        vector< vector<string> > Generate_Two_Dimensional_Vector(string file_name);
        void Draw_Box(string border_title, int x_position, int y_position, bool highlight_box);
    protected:
        string double_line, single_line;
        int console_column_quantity, console_row_quantity, column_spacing, table_start, box_width, box_height;
};

class MainMenu: public Utilities {
    public:
        MainMenu();
        void Add_Menu_Title();
        void Selection_Value(bool key_down);
        void Print();
        void Execute();
        void Update_Button_Description();
        void Update_Boxes();
        void Update_Buttons();
        void Draw_Button(int x_position, int y_position, bool selection_status);

    private:
        bool exit_trigger, primary_selected;
        vector< vector<string> > button_name, button_description;
        int primary, secondary, box_x_position[3], selection[3];
};

class Print_TSV_Table: public Utilities {
    public:
        Print_TSV_Table(string file_name, string table_title);
        void Generate_Table_Vector(string file_name);
        void Determine_Optimal_Column_Positions(); int * Determine_Longest_Cells_In_Columns();
        void Create_Table_Pages();
        void Draw_Table_Descriptors(string table_title, int page_quantity);
        void Draw_Table_Grid();
        void Print_Cells();

    private:
        int selected_page;
        vector< vector<string> > table;
        vector<int> page_range, optimal_cell_position;
};

class Allocate_Fare: public Utilities {
    public:
        Allocate_Fare();
        void Add_Menu_Title();
        void Selection_Value(int key_value);
        void Print();
        void Execute();
        void Update_Boxes();
        void Update_Fields();
        void Text_Input_Functionality(int selected);
        void Draw_Field(int x_position, int y_position, bool selection_status);
        void Draw_Button(int x_position, int y_position, bool selection_status);

    private:
        bool text_input_mode;


        bool exit_trigger, primary_selected;
        vector< vector<string> > field_name;
        int field_value, box_x_position[2], selection;
};

#endif // MAIN_H
