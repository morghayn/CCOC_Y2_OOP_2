#include "main.h"

//int main() {Print_TSV_Table Load; return 0;}


Print_TSV_Table::Print_TSV_Table(string file_name, string table_title) {
    Set_Console(); selected_page = 0;
    table = Generate_Two_Dimensional_Vector(file_name);
    Determine_Optimal_Column_Positions();
    Create_Table_Pages();

    bool exit_trigger = 0, exit_switch;
    int page_quantity = page_range.size() - 1, c = 0;

    while(exit_trigger == 0) {
        system("CLS"); exit_switch = 0;
        Draw_Table_Descriptors(table_title, page_quantity);
        Draw_Table_Grid();
        Print_Cells();

        while(exit_switch == 0) {
            switch(c=getch()) {
                case 75: selected_page--; if (selected_page == -1) selected_page = page_quantity - 1; break;
                case 77: selected_page++; if (selected_page == page_quantity) selected_page = 0; break;
                case 8: exit_trigger = 1; break;
            } exit_switch = 1;
        }
    }
}


void Print_TSV_Table::Determine_Optimal_Column_Positions() {
    vector<int> column_position; int * longest_cells = Determine_Longest_Cells_In_Columns();
    int total_of_longest_cells = accumulate(longest_cells, longest_cells + table[0].size(), 0);
    column_spacing = (console_column_quantity - total_of_longest_cells) / (table[0].size() + 1);

    column_position.push_back(column_spacing);
    for(int i = 1; i < table[0].size(); i++) column_position.push_back(column_position[i-1] + column_spacing + longest_cells[i-1]);
    optimal_cell_position = column_position;
}


int * Print_TSV_Table::Determine_Longest_Cells_In_Columns() {
    int * longest_cells = new int[table[0].size()];
    for(int i = 0; i < table[0].size(); i++) longest_cells[i] = 0;
    for(int i = 0; i < table.size(); i++)
        for(int j = 0; j < table[0].size(); j++)
            if (table[i][j].length() > longest_cells[j]) longest_cells[j] = table[i][j].length();
    return longest_cells;
}


void Print_TSV_Table::Create_Table_Pages() {
    vector<int> index; index.push_back(1); int records = (table.size() - 1);
    int max_records_per_page = ((console_row_quantity - 9) / 2);

    for(int i = 0; records > 0; i++) { // determining page quantity and record range for each page
        if (records >= max_records_per_page) {
            records -= max_records_per_page;
            index.push_back(max_records_per_page + index[i]);
        } else if (records >= 1) {index.push_back(records + index[i]); break;}
    }   page_range = index; // returning page quantity and range for each page
}


void Print_TSV_Table::Draw_Table_Descriptors(string table_title, int page_quantity) {
    gotoXY(0, 1, double_line, 0); // printing title
    gotoXY(((console_column_quantity / 2) - (table_title.length() / 2)), 1, table_title, 1);

    stringstream ss; // printing footer
    gotoXY(0, console_row_quantity - 2, single_line, 0);
    gotoXY(10, console_row_quantity - 2, "( Press [BACK-SPACE] to return to menu )", 0);
    ss << "( [<-] Page " << (selected_page + 1) << " of " << page_quantity << " [->] )";
    gotoXY(console_column_quantity - 35, console_row_quantity - 2, ss.str(), 0);
}


void Print_TSV_Table::Draw_Table_Grid() {
    char grid_1 = '\xB3', grid_2 = '\xC5', grid_3 = '\xC2', grid_4 = '\xC1';
    int column_divider_position = ((column_spacing / 2) + 1), x = 1; // determining optimal placement of column divider line

    for (int i = page_range[selected_page]; i < page_range[selected_page + 1]; i++) {
        gotoXY(0, ((x * 1) + table_start + x), single_line, 0);
        for(int j = 1; j < table[0].size(); j++) {
            if (x != 1) gotoXY((optimal_cell_position[j] - column_divider_position), ((x * 1) + table_start + x), grid_2);
            gotoXY((optimal_cell_position[j] - column_divider_position), ((x * 1) + table_start + x + 1), grid_1);
        } x++;
    } cout << endl << single_line;

    for(int i = 1; i < table[0].size(); i++) {
        gotoXY((optimal_cell_position[i] - column_divider_position), table_start + 2, grid_3);
        gotoXY((optimal_cell_position[i] - column_divider_position), (((page_range[selected_page + 1] - page_range[selected_page]) * 2) + table_start + 2), grid_4);
    }
}


void Print_TSV_Table::Print_Cells() {
    int x = 1;
    for(int i = 0; i < table[0].size(); i++)
        gotoXY(optimal_cell_position[i], (table_start + 1), table[0][i]);
    for (int i = page_range[selected_page]; i < page_range[selected_page + 1]; i++) {
        for(int j = 0; j < table[0].size(); j++) gotoXY(optimal_cell_position[j], ((x * 1) + table_start + x + 1), table[i][j]);
        x++;
    }
}
