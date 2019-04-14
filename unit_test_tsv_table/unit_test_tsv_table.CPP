#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <numeric>
#include <conio.h>

// Ordering is important
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "utilities.h"
#include "utilities.cpp"
using namespace std;

class Print_TSV_Table: public Utilities {
    private:
        string double_line, single_line;
        vector< vector<string> > table; vector<int> optimal_cell_position; vector<int> page_index;
        int console_column_quantity, console_row_quantity, cell_spacing, vertical_outset, selected_page;
    public:
        Print_TSV_Table(); void Print_Table(string file_name);
        void Set_Console();
        void Generate_Table_Vector(string file_name);
        void Determine_Optimal_Cell_Positions(); int * Determine_Longest_Cells_In_Column();
        void Page_Indexing();
        void Extra_Table_Styling(string table_title, int page_quantity);
        void Draw_Table_Grid();
        void Print_Cells();
};

int main() {Print_TSV_Table Load; return 0;}

void Print_TSV_Table::Set_Console() {
    console_column_quantity = 140; console_row_quantity = 30; vertical_outset = 2;
    stringstream ss; ss << "MODE CON: COLS=" << console_column_quantity << "LINES=" << console_row_quantity;
    string console_size = ss.str(); system(console_size.c_str());
    double_line = string (console_column_quantity,'\xCD');
    single_line = string (console_column_quantity,'\xC4');

    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

Print_TSV_Table::Print_TSV_Table() {
    Set_Console();
    while (1 == 1) {
        Print_Table("vehicles.tsv");
        Print_Table("drivers.tsv");
        Print_Table("500_records.tsv");
    }
}

void Print_TSV_Table::Print_Table(string file_name) {
    system("CLS"); bool exit_trigger = 0; selected_page = 0;
    Generate_Table_Vector(file_name);
    Determine_Optimal_Cell_Positions();
    Page_Indexing(); int page_quantity = page_index.size() - 1;

    while(exit_trigger == 0) {
        system("CLS"); bool exit_switch = 0; int c = 0;
        Extra_Table_Styling("[   Sample Table Title   ]", page_quantity);
        Draw_Table_Grid(); Print_Cells();

        while(exit_switch == 0) {
            c = 0;
            switch(c=getch()) {
            case 75: selected_page--; if (selected_page == -1) selected_page = page_quantity - 1; exit_switch = 1; break;
            case 77: selected_page++; if (selected_page == page_quantity) selected_page = 0; exit_switch = 1; break;
            case 27: exit_switch = 1; exit_trigger = 1; break;
            default: break;
            }
        }
    }
}

void Print_TSV_Table::Generate_Table_Vector(string file_name) {
    vector<string> imported_cells;
    vector< vector<string> > imported_rows;
    string delimited_line, cell; ifstream inFile(file_name.c_str());

    while(inFile.good()) {
        getline(inFile,delimited_line); if(delimited_line.empty()) break;
        istringstream string_stream(delimited_line);
        while(getline(string_stream, cell, '\t')) imported_cells.push_back(cell);
        imported_rows.push_back(imported_cells); imported_cells.clear();
    } table = imported_rows;
}

void Print_TSV_Table::Determine_Optimal_Cell_Positions() {
    vector< int > cell_position;
    int * longest_cells = Determine_Longest_Cells_In_Column();
    int longest_cells_total = accumulate(longest_cells, longest_cells + table[0].size(), 0);
    cell_spacing = (console_column_quantity - longest_cells_total) / (table[0].size() + 1);

    cell_position.push_back(cell_spacing);
    for(int i = 1; i < table[0].size(); i++)
        cell_position.push_back(cell_position[i-1] + cell_spacing + longest_cells[i-1]);
    optimal_cell_position = cell_position;
}

int * Print_TSV_Table::Determine_Longest_Cells_In_Column() {
    int * longest_cells = new int[table[0].size()];
    for(int i = 0; i < table[0].size(); i++) longest_cells[i] = 0;

    for(int i = 0; i < table.size(); i++) {
        for(int j = 0; j < table[0].size(); j++) if (table[i][j].length() > longest_cells[j]) longest_cells[j] = table[i][j].length();
    } return longest_cells;
}

void Print_TSV_Table::Page_Indexing() {
    int records = (table.size() - 1); int max_records_per_page = ((console_row_quantity - 9) / 2);
    vector<int> index; index.push_back(1);

    for(int i = 0; records > 0; i++) {
        if (records >= max_records_per_page) {
            records -= max_records_per_page; index.push_back(max_records_per_page + index[i]);
        } else if (records >= 1) {
            index.push_back(records + index[i]); break;
        }
    } page_index = index;
}

void Print_TSV_Table::Extra_Table_Styling(string table_title, int page_quantity) {
    gotoXY(0, 1, double_line, 0);
    gotoXY(0, console_row_quantity - 2, single_line, 0);
    gotoXY(10, console_row_quantity - 2, "( Press [ESC] to return to menu )", 0);
    gotoXY(((console_column_quantity / 2) - (table_title.length() / 2)), 1, table_title, 1);
    stringstream ss; ss << "( [<-] Page " << (selected_page + 1) << " of " << page_quantity << " [->] )";
    gotoXY((console_column_quantity - 35), console_row_quantity - 2, ss.str(), 0);
}

void Print_TSV_Table::Draw_Table_Grid() {
    char grid_1 = '\xB3', grid_2 = '\xC5', grid_3 = '\xC2', grid_4 = '\xC1';
    int divider_location = (cell_spacing / 2) + 1;

    int x = 1; for (int i = page_index[selected_page]; i < page_index[selected_page + 1]; i++) {
        gotoXY(0, (x * 1) + (x + (vertical_outset + 1)), single_line, 0);
        for(int j = 1; j < table[0].size(); j++) {
           if (x != 1) gotoXY((optimal_cell_position[j] - divider_location), (x * 1) + (x + (vertical_outset + 1)), grid_2);
            gotoXY((optimal_cell_position[j] - divider_location), ((x + (x * 1)) + (vertical_outset + 2)), grid_1);
        } x++;
    }   cout << endl << single_line;

    for(int i = 1; i < table[0].size(); i++) {
        gotoXY((optimal_cell_position[i] - divider_location), vertical_outset + 3, grid_3);
        gotoXY((optimal_cell_position[i] - divider_location), ((page_index[selected_page + 1] - page_index[selected_page]) * 2) + (vertical_outset + 3), grid_4);
    }
}

void Print_TSV_Table::Print_Cells() {
    for(int i = 0; i < table[0].size(); i++) gotoXY(optimal_cell_position[i], vertical_outset + 2, table[0][i], 0);
    int x = 1; for (int i = page_index[selected_page]; i < page_index[selected_page + 1]; i++) {
        for(int j = 0; j < table[0].size(); j++) gotoXY(optimal_cell_position[j], ((x + (x * 1)) + (vertical_outset + 2)), table[i][j], 0);
        x++;
    }
}
