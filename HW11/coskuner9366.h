#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <map>
#include <limits>
#include <string>
using namespace std;

class Node {
    public:
        struct Edge {
            bool cut;
            int weight;
        };
        map<int,Edge> connected; // dest label mapped to "Edge"
        int label = -1;         // Label of this node
        int partition = -1;     // Partition of this node
        bool swapped = false;
        Node () {}
        Node(int l, int p): label(l), partition(p) {}
};