#include <iostream>
#include <conio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <numeric>

// Ordering is important
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include "utilities.h"
#include "utilities.cpp"
using namespace std;

class TableVectorTest: public Utilities {
    public:
        TableVectorTest();
        vector< vector<string> > Generate_2D_String(string file_name);
        int * Get_List_Longest_Cells(vector< vector<string> > tabel_cell);
        int * Determine_Optimal_Cell_Position(vector< vector<string> > table);
        void Print_CSV_File(string file_name);
        void set_console_size() {console_column_quantity = 120;}
    private:
        int console_column_quantity;
        bool exitTrig;
};

int main() {
    TableVectorTest Load; return 0;
}

TableVectorTest::TableVectorTest() {
   set_console_size();
   // system("MODE CON: COLS=130 LINES=30");

    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

    while (1 == 1) {
        Print_CSV_File("vehicles.csv");
        cin.get(); system("CLS");

        cout << "\n\n\n";

        Print_CSV_File("drivers.csv");
        cin.get(); system("CLS");
    }
}

vector< vector<string> > TableVectorTest::Generate_2D_String(string file_name) {
    vector< vector<string> > table; vector<string> row;
    string delimited_line, cell; ifstream inFile(file_name.c_str());
    while(inFile.good()) {
        getline(inFile,delimited_line); if(delimited_line.empty()) break; row.clear();
        istringstream string_stream(delimited_line);
        while(getline(string_stream, cell, ',')) row.push_back(cell); table.push_back(row);
    } return table;
}

int * TableVectorTest::Get_List_Longest_Cells(vector< vector<string> > table) {
    int * longest_cells = new int[table[0].size()];
    for(int i = 0; i < table[0].size(); i++) longest_cells[i] = 0;
    for(int i = 0; i < table.size(); i++) {
        for(int j = 0; j < table[0].size(); j++)
            if (table[i][j].length() > longest_cells[j]) longest_cells[j] = table[i][j].length();
    } return longest_cells;
}

int * TableVectorTest::Determine_Optimal_Cell_Position(vector< vector<string> > table) {
    int * cell_position = new int[table[1].size()];
    int * list_longest_cells = Get_List_Longest_Cells(table);
    int longest_cells_total = accumulate(list_longest_cells, list_longest_cells + table[0].size(), 0);
    int spacing = (console_column_quantity - longest_cells_total) / (table[0].size() + 1); cell_position[0] = spacing;
    for(int i = 1; i < table[0].size(); i++) cell_position[i] = cell_position[i-1] + spacing + list_longest_cells[i-1];
    return cell_position;
}

void TableVectorTest::Print_CSV_File(string file_name) {
    string horizontal_border(console_column_quantity,'\xC4'); //, blank_row(console_column_quantity,' ');
    vector< vector<string> > table = Generate_2D_String(file_name); int table_print = 2;
    int * cell_position = Determine_Optimal_Cell_Position(table);

    //gotoXY(0, table_print, blank_row, 1);
    gotoXY(0, (table_print - 1), horizontal_border, 0);
    for(int j = 0; j < table[0].size(); j++) gotoXY(cell_position[j], table_print, table[0][j], 0);
    gotoXY(0, (table_print + 1), horizontal_border, 0);


    for (int i = 1; i < table.size(); i++) {
        gotoXY(0, (i * 1) + (i + (table_print+1)), horizontal_border, 0);
        for(int j = 0; j < table[0].size(); j++) gotoXY(cell_position[j], ((i + (i * 1)) + (table_print + 2)), table[i][j], 0);
    } cout << endl << horizontal_border;
}
