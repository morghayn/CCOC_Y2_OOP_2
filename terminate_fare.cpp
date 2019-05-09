#include "main.h"

Terminate_Fare::Terminate_Fare() {
    Print_TSV_Table active_fares("tsv/active_fares.tsv", "<[   T e r m i n a t e   F a r e   ]>", true);
    if (active_fares.Last_Key_Press() == 8) return;
    selected_fare = active_fares.Return_Highlighted_Record();

    Transfer_Fare_To_Log();
    Update_Status_To_Inactive("tsv/drivers.tsv", 7, 0);
    Update_Status_To_Inactive("tsv/vehicles.tsv", 8, 1);

    Check_Driver_Vehicle_Log();
}

void Terminate_Fare::Update_Status_To_Inactive(string file_name, int fare_column, int file_column) {
    vector< vector<string> > table = Generate_Two_Dimensional_Vector(file_name);
    int record_quantity = table.size(), column_quantity = table[0].size();
    for (int i = 0; i < record_quantity; i++) {
        if (selected_fare[fare_column] == table[i][file_column]) {record_to_update = i; break;}
    }   Print_Update_To_File(file_name, record_quantity, column_quantity, table);
}

void Terminate_Fare::Print_Update_To_File(string file_name, int record_quantity, int column_quantity, vector< vector<string> > table) {
    table[record_to_update][table[0].size() - 1] = "Inactive";
    ofstream change_status; change_status.open(file_name.c_str(), ios_base::trunc);
    for (int i = 0; i < record_quantity; i++) {
        for(int j = 0; j < column_quantity; j++)
            if (i == record_to_update && j == 4) change_status << Update_Kilometers(table, 4) << "\t";
            else change_status << table[i][j] << "\t";
        change_status << endl;
    }   change_status.close();
}

void Terminate_Fare::Transfer_Fare_To_Log() {
    Remove_From_Active_Fares("tsv/active_fares.tsv");

    stringstream ss; int column_quantity = selected_fare.size();
    for (int i = 0; i < column_quantity; i++) if (i < 7 || i > 8) ss << selected_fare[i] << "\t"; ss << Get_Todays_Date() << "\t";
    // if you want to keep in driver number and vehicle registration VVV
    // for (int i = 0; i < column_quantity; i++) ss << selected_fare[i] << "\t"; ss << Get_Todays_Date() << "\t";
    ofstream transfer_files; transfer_files.open("tsv/fare_log.tsv", ios_base::app);
    transfer_files << ss.str() << endl; transfer_files.close();
}

void Terminate_Fare::Remove_From_Active_Fares(string file_name) {
    vector< vector<string> > active_fares_table = Generate_Two_Dimensional_Vector(file_name);
    int record_quantity = active_fares_table.size(), column_quantity = active_fares_table[0].size();

    ofstream after_removal; after_removal.open(file_name.c_str(), ios_base::trunc);
    for (int i = 0; i < record_quantity; i++) {
        for(int j = 0; j < column_quantity; j++)
            if (selected_fare[8] != active_fares_table[i][8]) after_removal << active_fares_table[i][j] << "\t";
        if (selected_fare[8] != active_fares_table[i][8]) after_removal << endl;
    }   after_removal.close();
}

void Terminate_Fare::Check_Driver_Vehicle_Log() {
    vector< vector<string> > log_check = Generate_Two_Dimensional_Vector("tsv/driver_log.tsv");

    if (log_check.size() == 1) { // if no entries exist, add entries
        Add_Entries_For_Today("tsv/drivers.tsv", "tsv/driver_log.tsv", true);
        Add_Entries_For_Today("tsv/vehicles.tsv", "tsv/vehicle_log.tsv", false);
    } else if (log_check[1][0] != Get_Todays_Date()) { // if entries for current date don't exist, add entries
        Add_Entries_For_Today("tsv/drivers.tsv", "tsv/driver_log.tsv", true);
        Add_Entries_For_Today("tsv/vehicles.tsv", "tsv/vehicle_log.tsv", false);
    }
    Update_Driver_Vehicle_Entry(true, 7, 3); Update_Driver_Vehicle_Entry(false, 8, 4);
}

void Terminate_Fare::Add_Entries_For_Today(string details_file_name, string log_file_name, bool is_driver_log) {
    vector<string> record_update; vector< vector<string> > updated_log;
    vector< vector<string> > current_log = Generate_Two_Dimensional_Vector(log_file_name); int column_quantity = current_log[0].size();
    vector< vector<string> > details_for_log = Generate_Two_Dimensional_Vector(details_file_name); int record_quantity = details_for_log.size();
    int swipe_column_quantity; if (is_driver_log == true) swipe_column_quantity = 2; else swipe_column_quantity = 4;

    for (int i = 0; i < column_quantity; i++) record_update.push_back(current_log[0][i]);
    updated_log.push_back(record_update); record_update.clear();

    for (int i = 1; i < record_quantity; i++) {
        record_update.push_back(Get_Todays_Date());
        for (int x = 0; x < 2; x++) record_update.push_back("0");
        for (int j = 0; j < swipe_column_quantity; j++) record_update.push_back(details_for_log[i][j]);
        updated_log.push_back(record_update);
        record_update.clear();
    }

    record_quantity = current_log.size();
    for (int i = 1; i < record_quantity; i++) {
        for (int j = 0; j < column_quantity; j++) record_update.push_back(current_log[i][j]);
        updated_log.push_back(record_update);
        record_update.clear();
    }

    column_quantity = updated_log[0].size(); record_quantity = updated_log.size();
    ofstream log_update; log_update.open(log_file_name.c_str(), ios_base::trunc);
    for (int i = 0; i < record_quantity; i++) {
        for(int j = 0; j < column_quantity; j++)
            log_update << updated_log[i][j] << "\t";
        log_update << endl;
    }   log_update.close();
}

void Terminate_Fare::Update_Driver_Vehicle_Entry(bool is_driver_log, int fare_column, int file_column) {
    string fare_log; if (is_driver_log == true) fare_log = "tsv/driver_log.tsv"; else fare_log = "tsv/vehicle_log.tsv";

    // set record to update
    // first update revenue
    // then update kilometers
    vector< vector<string> > table = Generate_Two_Dimensional_Vector(fare_log);
    int record_quantity = table.size(), column_quantity = table[0].size();
    for (int i = 0; i < record_quantity; i++) {
        if (selected_fare[fare_column] == table[i][file_column]) {record_to_update = i; break;}
    }

    ofstream update_entry; update_entry.open(fare_log.c_str(), ios_base::trunc);
    for (int i = 0; i < record_quantity; i++) {
        for(int j = 0; j < column_quantity; j++) {
            if (i == record_to_update && j == 1) update_entry << Update_Revenue(table) << "\t";
            else if (i == record_to_update && j == 2) update_entry << Update_Kilometers(table, 2) << "\t";
            else update_entry << table[i][j] << "\t";
        }   update_entry << endl;
    }   update_entry.close();

}

string Terminate_Fare::Update_Revenue(vector< vector<string> > table) {
    float x; float updated_revenue;
    istringstream(selected_fare[9]) >> x;
    istringstream(table[record_to_update][1]) >> updated_revenue;
    updated_revenue += x;
    stringstream ss; ss << updated_revenue;
    return ss.str();
}

string Terminate_Fare::Update_Kilometers(vector< vector<string> > table, int kilometer_column) {
    float x; float updated_kilometers;
    istringstream(selected_fare[5]) >> x;
    istringstream(table[record_to_update][kilometer_column]) >> updated_kilometers;
    updated_kilometers += x;
    stringstream ss; ss << updated_kilometers; return ss.str();
}
