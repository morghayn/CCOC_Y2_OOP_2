#include "main.h"


//int main() {Print_TSV_Table Load; return 0;}


Print_TSV_Table::Print_TSV_Table() {
    while (1 == 1) {
        Print_Table("tsv/vehicles.tsv");
        Print_Table("tsv/drivers.tsv");
        Print_Table("tsv/500_records.tsv");
    }
}


void Print_TSV_Table::Print_Table(string file_name) {
    system("CLS"); Set_Console();
    bool exit_trigger = 0; selected_page = 0;

    table = Generate_Two_Dimensional_Vector(file_name);
    Determine_Optimal_Column_Positions();
    Determine_Page_Indexing();
    int page_quantity = page_index.size() - 1;

    while(exit_trigger == 0) {
        system("CLS"); bool exit_switch = 0; int c = 0;
        Draw_Table_Descriptors("[   Sample Table Title   ]", page_quantity);
        Draw_Table_Grid();
        Print_Cells();

        while(exit_switch == 0) {
            c = 0;
            switch(c=getch()) { // Case 75 = Key_Left, Case 77 = Key_Right, Case 27 = ESC Key;
            case 75: selected_page--; if (selected_page == -1) selected_page = page_quantity - 1; exit_switch = 1; break;
            case 77: selected_page++; if (selected_page == page_quantity) selected_page = 0; exit_switch = 1; break;
            case 27: exit_switch = 1; exit_trigger = 1; break;
            default: break;
            }
        }
    }
}


void Print_TSV_Table::Determine_Optimal_Column_Positions() {
    vector< int > column_position; int * longest_cells = Determine_Longest_Cells_In_Columns();

    // accumalate() function tallys longest_cells[]
    int longest_cells_total = accumulate(longest_cells, longest_cells + table[0].size(), 0);

    // determining spacing between columns
    column_spacing = (console_column_quantity - longest_cells_total) / (table[0].size() + 1);
    column_position.push_back(column_spacing); // <- setting column_position[0] as column_spacing

    // setting column_position[1 and onwards] in respect to the prior column.
    for(int i = 1; i < table[0].size(); i++)
        column_position.push_back(column_position[i-1] + column_spacing + longest_cells[i-1]);
    optimal_cell_position = column_position;
}


int * Print_TSV_Table::Determine_Longest_Cells_In_Columns() {
    int * longest_cells = new int[table[0].size()];
    for(int i = 0; i < table[0].size(); i++) longest_cells[i] = 0;

    // determing the longest string in each column
    for(int i = 0; i < table.size(); i++) {
        for(int j = 0; j < table[0].size(); j++) if (table[i][j].length() > longest_cells[j]) longest_cells[j] = table[i][j].length();
    } return longest_cells;
}


void Print_TSV_Table::Determine_Page_Indexing() {
    int records = (table.size() - 1); vector<int> index; index.push_back(1);

    // determining the max records per page by deduction 9 spaces from the console_row_quantity
    // then dividing by 2; this is consideration of the table grid that will be drawn
    int max_records_per_page = ((console_row_quantity - 9) / 2);

    // this for loop determines how many pages there will be in respect to the row quantity of the table
    for(int i = 0; records > 0; i++) {
        if (records >= max_records_per_page) {
            records -= max_records_per_page; index.push_back(max_records_per_page + index[i]);
        } else if (records >= 1) {
            index.push_back(records + index[i]); break;
        }
    } page_index = index; // page quantity of table is determined in relation to how many records can fit on the screen
}


void Print_TSV_Table::Draw_Table_Descriptors(string table_title, int page_quantity) {
    gotoXY(0, 1, double_line, 0);
    gotoXY(0, console_row_quantity - 2, single_line, 0);
    gotoXY(10, console_row_quantity - 2, "( Press [ESC] to return to menu )", 0);

    // placing table title at center of the console
    gotoXY(((console_column_quantity / 2) - (table_title.length() / 2)), 1, table_title, 1);

    // updating page selection in respect to whichever page is selected
    stringstream ss; ss << "( [<-] Page " << (selected_page + 1) << " of " << page_quantity << " [->] )";
    gotoXY((console_column_quantity - 35), console_row_quantity - 2, ss.str(), 0);
}


void Print_TSV_Table::Draw_Table_Grid() {
    char grid_1 = '\xB3', grid_2 = '\xC5', grid_3 = '\xC2', grid_4 = '\xC1'; // table grid chars
    int column_divider_location = (column_spacing / 2) + 1; // determining placement of column dividers

    int x = 1; for (int i = page_index[selected_page]; i < page_index[selected_page + 1]; i++) {
        gotoXY(0, (x * 1) + (x + vertical_outset), single_line, 0); // line in-between rows
        for(int j = 1; j < table[0].size(); j++) {
           if (x != 1) gotoXY((optimal_cell_position[j] - column_divider_location), (x * 1) + (x + vertical_outset), grid_2); // criss-cross
            gotoXY((optimal_cell_position[j] - column_divider_location), ((x + (x * 1)) + (vertical_outset + 1)), grid_1); // vertical line
        } x++;
    }   cout << endl << single_line; // line after last record

    for(int i = 1; i < table[0].size(); i++) { // extra table styling
        gotoXY((optimal_cell_position[i] - column_divider_location), vertical_outset + 2, grid_3);
        gotoXY((optimal_cell_position[i] - column_divider_location), ((page_index[selected_page + 1] - page_index[selected_page]) * 2) + (vertical_outset + 2), grid_4);
    }
}


void Print_TSV_Table::Print_Cells() {
    for(int i = 0; i < table[0].size(); i++) gotoXY(optimal_cell_position[i], (vertical_outset + 1), table[0][i], 0);
    int x = 1; for (int i = page_index[selected_page]; i < page_index[selected_page + 1]; i++) {
        for(int j = 0; j < table[0].size(); j++) gotoXY(optimal_cell_position[j], ((x + (x * 1)) + (vertical_outset + 1)), table[i][j], 0);
        x++; // printing all table cells with respect to the amount of records on the selected page
    }
}
