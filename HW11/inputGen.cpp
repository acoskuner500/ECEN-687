#include "coskuner9366.h"
#include <time.h>
// #define HARDCODED

const int numNodes = 8, numPart = 2;

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Single argument needed for name of input file to be generated\n";
        return 1;
    }
    // write to file output
    ofstream os;
    os.open(argv[1], ios::out | ios::trunc);

    unsigned int i, sl, dl, sp, dp, seed,
        numEdges, minEdges, maxEdges,
        numCut = 0, numUncut = 0, numSelf = 0;
    bool edgeFound = false;
    map<int,Node::Edge>::iterator it;

#ifdef HARDCODED
    // hardcoded parameters
    seed = 123456789; // 123456789 gives exactly 300 edges, but solves in 4 iterations
    minEdges = 80;
    maxEdges = 300;
#else
    int numNodes, numPart;
    // user defined parameters through terminal std IO
    cout << "Choose a seed for RNG: ";
    cin >> seed;
    cout << "How many nodes? ";
    cin >> numNodes;
    cout << "How many partitions? ";
    cin >> numPart;
    cout << "How many edges min? ";
    cin >> minEdges;
    cout << "How many edges max? ";
    cin >> maxEdges;
    while (numNodes % numPart != 0) {
        cout << "Need number that divides " << numNodes << " evenly." << endl;
        cin >> numPart;
    }
#endif
    srand(seed);
    // srand(time(NULL));
    // random number of edges between min and max inclusive
    numEdges = (rand() % (maxEdges - minEdges + 1)) + minEdges;
    Node nodes[numNodes];                   // collection of all nodes
    os << numNodes << " " << numEdges << endl;

    // init all nodes and add to init partition
    for (i = 0; i < numNodes; i++) {
        int p = i / (numNodes / numPart);
        nodes[i] = Node(i+1, p);
    }

    for (i = 0; i < numEdges; i++) {
        // randomize edges and determine if cut
        sl = (rand() % numNodes) + 1; // rand 1 to n
        dl = (rand() % numNodes) + 1; // rand 1 to n

        // write edge source and destination nodes to file
        os << sl << " " << dl;
        if (i != numEdges - 1) {
            os << endl;
        }

        if (sl > dl) swap(sl, dl);
        sp = (sl - 1) / (numNodes / numPart);
        dp = (dl - 1) / (numNodes / numPart);
        for (it = nodes[sl-1].connected.begin(); it != nodes[sl-1].connected.end(); it++) {
            if (it->first == dl) {
                it->second.weight++;
                edgeFound = true;
                break;
            }
        }
        if (!edgeFound) {
            nodes[sl-1].connected[dl].weight = 1;
            if (sl != dl) nodes[dl-1].connected[sl].weight = 1;
        }
        nodes[sl-1].connected[dl].cut = (sp != dp);
        if (sl != dl)   nodes[dl-1].connected[sl].cut = (sp != dp);
        if (sp != dp && !edgeFound) numCut++;
        if (sp == dp && !edgeFound && sl != dl) numUncut++;
        if (sl == dl && !edgeFound) numSelf++;
        edgeFound = false;
    }

    // print init partition to terminal
    for (Node n : nodes) {
        cout << n.label << " in part " << n.partition << endl;
    }
    // (optional) print number of types of edges
    cout << "number of edges generated: " << numEdges << endl;
    cout << "number of cut edges: " << numCut << endl;
    cout << "number of uncut edges: " << numUncut << endl;
    cout << "number of self-looping edges: " << numSelf << endl;
    // close output file
    os.close();
    return 0;
}