#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>  
#include <list>
#include <limits>
using namespace std;

struct Node {
    int id, C;
    float RAT;
    string branch = "";
    bool isSink, hasBuffer, mergedInternal = false;
    Node *pred = nullptr, *left = nullptr, *right = nullptr;
    Node() {};
    Node(int id, int C, float RAT, bool isSink, bool hasBuffer):
        id(id), C(C), RAT(RAT), isSink(isSink), hasBuffer(hasBuffer) {};
};

struct Edge {
    int up, down, length;
    bool hasBuffer = false;
    Edge () {};
    Edge(int up, int down, int length): up(up), down(down), length(length) {};
};