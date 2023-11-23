#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <limits>
using namespace std;
// #define DEBUG
// #define PRINT

class Cell {
    public:
    int label, cost;        // Label, curr cost
    int row, col;           // Coordinates
    int distance;           // dist to target
    bool isBlocked = false; // Is in blockage?
    bool isFree = false;    // Is in free island?
    bool visited = false;   // Visited?
    Cell *n, *s, *e, *w;    // Neighbors
    // Constructors
    Cell () {};
    Cell (int l, int r, int c): label(l), row(r), col(c) {};
};

enum direction {
    north, south, east, west, no_dir
};

int main(int argc, char** argv) {
    int nRow, nCol, sRow, sCol, tRow, tCol;
    int blockBLRow, blockBLCol, blockTRRow, blockTRCol;
    int freeBLRow, freeBLCol, freeTRRow, freeTRCol;
    // { // Init vars by reading from file
    ifstream is;
    #ifndef DEBUG
        if (argc != 2) {
            cerr << "Single argument needed for name of input file\n";
            return 1;
        }
        is.open(argv[1], ios::in);
    #else
        is.open("input.txt", ios::in);
    #endif
    is >> nRow >> nCol;             // read in number of rows and columns in grid
    is >> sRow >> sCol;             // read in coords of source point
    is >> tRow >> tCol;             // read in coords of target point
    is >> blockBLRow >> blockBLCol; // read in bottom left coords of blockage
    is >> blockTRRow >> blockTRCol; // read in top right coords of blockage
    is >> freeBLRow >> freeBLCol;   // read in bottom left coords of free island
    is >> freeTRRow >> freeTRCol;   // read in top right coords of free island
    is.close();
    // }

    // Init grid array
    #ifdef PRINT
    cout << "Initializing grid array\n";
    #endif
    Cell grid[nRow][nCol];
    for (int i=0; i<nRow; i++) {
        for (int j=0; j<nCol; j++) {
            grid[i][j] = Cell(i*nCol+j,i+1,j+1);
            if (j >= blockBLCol-1 && j <= blockTRCol-1 && i >= blockBLRow-1 && i <= blockTRRow-1) {
                grid[i][j].isBlocked = true;
                continue;
            }
            if (j >= freeBLCol-1 && j <= freeTRCol-1 && i >= freeBLRow-1 && i <= freeTRRow-1) {
                grid[i][j].isFree = true;
            }
            grid[i][j].n = (i < nRow-1) ? &grid[i+1][j] : nullptr;
            grid[i][j].s = (i > 0)      ? &grid[i-1][j] : nullptr;
            grid[i][j].e = (j < nCol-1) ? &grid[i][j+1] : nullptr;
            grid[i][j].w = (j > 0)      ? &grid[i][j-1] : nullptr;

            grid[i][j].distance = abs(i-tRow+1) + abs(j-tCol+1);
            grid[i][j].cost = 0;
        }
    }

    // Init source cell
    #ifdef PRINT
    cout << "Initializing source cell\n";
    #endif
    grid[sRow-1][sCol-1].visited = true;
    Cell* currCell = &grid[sRow-1][sCol-1], *nextCell;
    
    int candidateCost = 0, minCost, minDist, minLabel;
    map<int,pair<int,direction>> visited, minVisited;
    stack<Cell *> path;

    minVisited[currCell->label] = make_pair(0,no_dir);

    #ifdef PRINT
    cout << "Visiting neighbors\n";
    #endif
    while (currCell != &grid[tRow-1][tCol-1]) {
        #ifdef PRINT
        printf("%d, %d\n", currCell->label, currCell->cost);
        #endif
        for (int i=0; i<4; i++) {
            switch (i) {
                case north: nextCell = currCell->n; break;
                case south: nextCell = currCell->s; break;
                case east: nextCell = currCell->e; break;
                case west: nextCell = currCell->w; break;
            }
            if (nextCell != nullptr && !nextCell->isBlocked &&
                !(nextCell->visited && (minVisited.find(nextCell->label) != minVisited.end()))) {
                candidateCost = currCell->cost + ((currCell->isFree && nextCell->isFree) ? 0 : 1);
                if (nextCell->visited && candidateCost < visited[nextCell->label].first
                    || !nextCell->visited) {
                    visited[nextCell->label].first = candidateCost;
                    nextCell->cost = candidateCost;
                    switch (i) {
                        case north:
                            visited[nextCell->label].second = south;
                            break;
                        case south:
                            visited[nextCell->label].second = north;
                            break;
                        case east:
                            visited[nextCell->label].second = west;
                            break;
                        case west:
                            visited[nextCell->label].second = east;
                            break;
                    }
                }
                nextCell->visited = true;
            }
        }
        minCost = numeric_limits<int>::max();
        minDist = numeric_limits<int>::max();
        for (map<int,pair<int,direction>>::iterator vit = visited.begin(); vit != visited.end(); vit++) {
            if (vit->second.first <= minCost && grid[minLabel/nCol][minLabel%nCol].distance < minDist) {
                minCost = vit->second.first;
                minLabel = vit->first;
            }
        }
        minVisited[minLabel] = visited[minLabel];
        visited.erase(minLabel);
        currCell = &grid[minLabel/nCol][minLabel%nCol];
    }
    printf("Total path cost: %d\n", minVisited[currCell->label].first);
    while (currCell != &grid[sRow-1][sCol-1]) {
        path.push(currCell);
        switch (minVisited[currCell->label].second) {
            case north: currCell = currCell->n; break;
            case south: currCell = currCell->s; break;
            case east: currCell = currCell->e; break;
            case west: currCell = currCell->w; break;
        }
    }
    path.push(currCell);
    while (!path.empty()) {
        printf("(%d,%d) ", path.top()->row, path.top()->col);
        path.pop();
    }
    cout << endl;
    
    return 0;
}