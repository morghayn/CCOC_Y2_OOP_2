#include <iostream>
#include <conio.h>
#include "utilities.h"
#include "utilities.cpp"
using namespace std;

class MainMenu: public Utilities {
    public:
        MainMenu();
        void SetMenu(int s); void Print(int s); void Execute(int s);
    private:
        bool exitTrig;
};

int main() {MainMenu Load; return 0;}

MainMenu::MainMenu() {
    int arr_1[8] = {0,1,2,3,0,1,2,3}, arr_2[8] = {0,0,1,2,4,4,5,6}, arr_3[8] = {1,2,3,3,5,6,7,7}, arr_4[8] = {4,5,6,7,4,5,6,7};
    int s = 0, c = 0; exitTrig = 0; SetMenu(s); ShowConsoleCursor(0);

    while(exitTrig == 0) {
        c = 0;
        switch(c=getch()) {
        case 72: s = arr_1[s]; break;
        case 75: s = arr_2[s]; break;
        case 77: s = arr_3[s]; break;
        case 80: s = arr_4[s]; break;
        case 13: Execute(s); break;
        default: break;
        } Print(s);
    }
}

void MainMenu::SetMenu(int s) {
    system("CLS");
    Draw(2,20,5,79,19); gotoXY(45,8,"[CallOut Dispatch Services]",0);
    gotoXY(20,11,string(79,'\xDB'),0); for(int i = 0; i < 12; i++) gotoXY(58,11+i,string(3,'\xDB'),0);
    Print(s);
}

void MainMenu::Print(int s) {
    int l = 14, h[8]; for (int i = 0; i < 8; i++) h[i] = (s != (i)) ? 0 : 1;
    gotoXY(38,l,"View",h[0]);
    gotoXY(36,l+3,"Database",h[1]);
    gotoXY(36,l+6,"Settings",h[2]);
    gotoXY(83,l,"Help",h[3]);
    gotoXY(82,l+3,"Drivers",h[4]);
    gotoXY(50,l+3,"Vehicles",h[5]);
    gotoXY(67,l+3,"Report",h[6]);
    gotoXY(83,l+3,"Exit",h[7]);
    gotoXY(0,0);
}

void MainMenu::Execute(int s) {
    switch(s) {
    case 0: break;
    case 1: break;
    case 2: break;
    case 3: break;
    case 4: exitTrig = 1; break;
    default: break;
    }
}
