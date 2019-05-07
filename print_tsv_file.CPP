#include "main.h"

Print_TSV_Table::Print_TSV_Table(string file_name, string title, bool selection_table) {
    exit_trigger = false;
    selector = selection_table;
    table_title = title;
    table_file_name = file_name;

    Set_Console();
    selected_record = 1;
    current_page = 0;
    table = Generate_Two_Dimensional_Vector(file_name);
    column_quantity = table[0].size();
    record_quantity = table.size();

    Determine_Column_Spacing();
    Create_Table_Pages();

    Change_Page_Request(0, 1);
    if (selector == true) Change_Record_Selection(0);

    while (exit_trigger == false) {
        switch (key_press=getch()) {
            case 72: if (selector == true) Change_Record_Selection(72); break;
            case 80: if (selector == true) Change_Record_Selection(80); break;
            case 75: Change_Page_Request(key_press, 0); break;
            case 77: Change_Page_Request(key_press, 0); break;
            case 8:  exit_trigger = true; break;
            case 13:
            exit_trigger = true;
            if (selector == true) {
                if (table[selected_record][table[0].size() - 1] == "Active") {
                    Yes_No_Box("Selected record is active. Either return or change your choice!", "Return", "Change Choice");
                    if (yes_no_box_selection == true) key_press = 8;
                    else if (yes_no_box_selection == false) exit_trigger = false;
                }
            }   Change_Page_Request(0, 1); Change_Record_Selection(0); break;
        }
    }
}

void Print_TSV_Table::Change_Page_Request(int key_value, bool is_table_load) {
    int initial_selected_page = current_page;
    if (key_value == 77) current_page++; if (key_value == 75) current_page--;
    if (current_page == -1) current_page = page_quantity - 1;
    if (current_page == page_quantity) current_page = 0;

    if (initial_selected_page != current_page || is_table_load == true) {
        system("CLS");
        Print_Table_Descriptors();
        Print_Table_Grid(1);
        Print_Cells(1);
    }
}

void Print_TSV_Table::Determine_Column_Spacing() {
    int * longest_cells = Determine_Longest_Cells_In_Columns();
    int total_of_longest_cells = accumulate(longest_cells, (longest_cells + column_quantity), 0);
    column_position.push_back((console_width - total_of_longest_cells) / (column_quantity + 1));
    for (int i = 1; i < column_quantity; i++)
        column_position.push_back(column_position[i-1] + column_position[0] + longest_cells[i-1]);
}

int * Print_TSV_Table::Determine_Longest_Cells_In_Columns() {
    int * longest_cells = new int[column_quantity];
    for (int i = 0; i < column_quantity; i++) longest_cells[i] = 0;
    for (int i = 0; i < record_quantity; i++)
        for (int j = 0; j < column_quantity; j++) {
            int cell_length = table[i][j].length();
            if (cell_length > longest_cells[j]) longest_cells[j] = table[i][j].length();
        }
    return longest_cells;
}

void Print_TSV_Table::Create_Table_Pages() {
    page_range.push_back(1); int records = (record_quantity - 1);
    int max_records_per_page = ((console_height - 9) / 2);

    for (int i = 0; records > 0; i++) { // determining page quantity and record range for each page
        if (records >= max_records_per_page) {
            records -= max_records_per_page;
            page_range.push_back(max_records_per_page + page_range[i]);
        } else if (records >= 1) {
            page_range.push_back(records + page_range[i]);
            break;
        }
    } page_quantity = page_range.size() - 1;
}

void Print_TSV_Table::Print_Table_Descriptors() {
    gotoXY(0, 1, double_line, 0);
    gotoXY(((console_width / 2) - (table_title.length() / 2)), 1, table_title, 1);

    stringstream ss; string footer_message;
    if (selector == false) footer_message = "( Press [BACK-SPACE] to return to menu )";
    else footer_message = "( Press [BACK-SPACE] to return to menu or [ENTER] to select record )";

    gotoXY(0, console_height - 2, single_line, 0);
    gotoXY(10, console_height - 2, footer_message, 0);
    ss << "( [<-] Page " << (current_page + 1) << " of " << page_quantity << " [->] )";
    gotoXY(console_width - 35, console_height - 2, ss.str(), 0);
}

void Print_TSV_Table::Print_Table_Grid(int x) {
    for (int i = page_range[current_page]; i < page_range[current_page + 1]; i++) {
        int y_calc = ((x * 1) + 3 + x);
        gotoXY(0, y_calc, single_line, 0);
        gotoXY(0, (y_calc + 2), single_line, 0);
        for (int j = 1; j < column_quantity; j++) {
            int char_location = (column_position[j] - ((column_position[0] / 2) + 1));
            gotoXY(char_location, (y_calc + 1), '\xB3');
            if (x != 1) gotoXY(char_location, y_calc, '\xC5');
            if (x == 1) gotoXY(char_location, 5, '\xC2');
            gotoXY(char_location, (((page_range[current_page + 1] - page_range[current_page]) * 2) + 5), '\xC1');
        } x++;
    }
}

void Print_TSV_Table::Print_Cells(int x) {
    for (int i = 0; i < column_quantity; i++) // column names
        gotoXY(column_position[i], (4), table[0][i]);

    for (int i = page_range[current_page]; i < page_range[current_page + 1]; i++) { // column cells
        for(int j = 0; j < column_quantity; j++)
            gotoXY(column_position[j], ((x * 1) + x + 4), table[i][j]);
        x++;
    }
}

/// ~-~-~-~-~ <[  S e l e c t o r   T a b l e   F u n c t i o n a l i t y  ]> ~-~-~-~-~

void Print_TSV_Table::Change_Record_Selection(int key_value) {
    Remove_Last_Highlighted_Record(Get_Record_Location(1));
    if (key_value == 80) selected_record++;
    if (key_value == 72)  selected_record--;
    if (selected_record > (record_quantity - 1)) selected_record = record_quantity - 1;
    if (selected_record == 0) selected_record = 1;

    if (selected_record < page_range[current_page]) Change_Page_Request(75, 0);
    if (selected_record >= page_range[current_page + 1]) Change_Page_Request(77, 0);
    Highlight_Selected_Record(Get_Record_Location(1));
    Return_Highlighted_Record();
}

int Print_TSV_Table::Get_Record_Location(int record_location) {
    for (int i = page_range[current_page]; i < page_range[current_page + 1]; i++) {
        if (selected_record == i) break; record_location++;
    }   return record_location;
}

void Print_TSV_Table::Remove_Last_Highlighted_Record(int record_location) {
    int y_calc = ((record_location * 1) + 3 + record_location);

    gotoXY(0, (y_calc + 1), string(console_width, ' '), 0);
    for (int i = 0; i < 3; i += 2) gotoXY(0, y_calc + i, single_line, 0);
    for (int i = 0; i < column_quantity; i++) {
        int char_location = (column_position[i] - ((column_position[0] / 2) + 1));

        gotoXY(column_position[i], (y_calc + 1), table[selected_record][i], 0);
        if(i != 0) {
            gotoXY(char_location, 5, '\xC2');
            gotoXY(char_location, (y_calc + 1), '\xB3');
            gotoXY(char_location, (y_calc + 2), '\xC5');
            if (record_location != 1) gotoXY(char_location, y_calc, '\xC5');
            gotoXY(char_location, (((page_range[current_page + 1] - page_range[current_page]) * 2) + 5), '\xC1');
        }
    }
}

void Print_TSV_Table::Highlight_Selected_Record(int record_location) {
    int y_calc = ((record_location * 1) + 3 + record_location);
    gotoXY(0, (y_calc + 1), string(console_width, ' '), 1);
    for (int i = 0; i < column_quantity; i++) {
        if(i != 0) gotoXY(0, y_calc, string(console_width, '\xDF'), 1);
        if(i != 0) gotoXY(0, (y_calc + 2), string(console_width, '\xDC'), 1);
        gotoXY(column_position[i], (y_calc + 1), table[selected_record][i], 1);
    }
}

vector<string> Print_TSV_Table::Return_Highlighted_Record() {
    vector<string> highlighted_record;
    for(int j = 0; j < column_quantity; j++)
        highlighted_record.push_back(table[selected_record][j]);
    return highlighted_record;
}

void Print_TSV_Table::Submit_Status_Change() {
    table[selected_record][table[0].size() - 1] = "Active";
    ofstream change_status;
    change_status.open(table_file_name.c_str(), ios_base::trunc);
    for (int i = 0; i < record_quantity; i++) { // column cells
        for(int j = 0; j < column_quantity; j++)
            change_status << table[i][j] << "\t";
        change_status << endl;
    }   change_status.close();
}
