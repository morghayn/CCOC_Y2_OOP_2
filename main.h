#ifndef MAIN_H
#define MAIN_H

#include <conio.h>
#include <numeric>

#include "utilities.h"
#include "utilities.cpp"

using namespace std;

class MainMenu: public Utilities {
    public:
        MainMenu();
        void Add_Menu_Title();
        void Selection_Value(bool key_down);
        void Execute();
        void Print();

        void Update_Button_Description();
        void Update_Buttons();  void Draw_Button(int x_position, int y_position, bool selection_status);
        void Update_Boxes();    void Draw_Box(string border_title, int x_position, int y_position, bool selection_status);
    private:
        bool exit_trigger, primary_selected;
        vector< vector<string> > button_name, button_description;
        int ran_1, ran_2, box_x_position[3], selection[3];
};


class Print_TSV_Table: public Utilities {
    private:
        int selected_page; vector< vector<string> > table; vector<int> optimal_cell_position; vector<int> page_index;
    public:
        Print_TSV_Table(); void Print_Table(string file_name);
        void Generate_Table_Vector(string file_name);
        void Determine_Optimal_Column_Positions(); int * Determine_Longest_Cells_In_Columns();
        void Determine_Page_Indexing();
        void Draw_Table_Descriptors(string table_title, int page_quantity);
        void Draw_Table_Grid();
        void Print_Cells();
};


#endif // MAIN_H
