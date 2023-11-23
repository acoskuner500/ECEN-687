#include "coskuner9366.h"
#include <time.h>

int main(int argc, char** argv) {
    ofstream os;
    os.open("input.txt", ios::out | ios::trunc);
    int nRow, nCol, sRow, sCol, tRow, tCol;
    int blockBLRow, blockBLCol, blockTRRow, blockTRCol;
    int freeBLRow, freeBLCol, freeTRRow, freeTRCol;
    cout << "How many rows in grid? ";              cin >> nRow;
    cout << "How many columns in grid? ";           cin >> nCol;
    cout << "Row of source? ";                      cin >> sRow;
    cout << "Col of source? ";                      cin >> sRow;
    cout << "Row of target? ";                      cin >> tRow;
    cout << "Col of target? ";                      cin >> tCol;
    cout << "Row of bottom left of blockage? ";      cin >> blockBLRow;
    cout << "Col of bottom left of blockage? ";     cin >> blockBLCol;
    cout << "Row of top right of blockage? ";       cin >> blockTRRow;
    cout << "Col of top right of blockage? ";       cin >> blockTRCol;
    cout << "Row of bottom left of free island? ";  cin >> freeBLRow;
    cout << "Col of bottom left of free island? ";  cin >> freeBLCol;
    cout << "Row of top right of free island? ";    cin >> freeTRRow;
    cout << "Col of top right of free island? ";    cin >> freeTRCol;

    os << nRow << " " << nCol << endl;              // write number of rows and columns in grid
    os << sRow << " " << sCol << endl;              // write number of rows and columns in grid
    os << tRow << " " << tCol << endl;              // write number of rows and columns in grid
    os << blockBLRow << " " << blockBLCol << " ";   // write bottom left coords of blockage
    os << blockTRRow << " " << blockTRCol << endl;  // write top right coords of blockage
    os << freeBLRow << " " << freeBLCol << " ";     // write bottom left coords of free island
    os << freeTRRow << " " << freeTRCol << endl;    // write top right coords of free island
    os.close();
    return 0;
}