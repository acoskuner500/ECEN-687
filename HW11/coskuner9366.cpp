#include "coskuner9366.h"
#define PRINT
const int numPart = 2;
// #define DEBUG

int main(int argc, char** argv) {
    #ifndef DEBUG
    if (argc != 2) {
        cerr << "Single argument needed for name of input file\n";
        return 1;
    }
    ifstream is;
    is.open(argv[1], ios::in);
    #else
    ifstream is;
    is.open("input.txt", ios::in);
    #endif
    int numNodes, numEdges;
    is >> numNodes >> numEdges; // read in number of nodes and edges

    string s, d;
    int i, j, sl, dl, sp, dp, si, di;
    bool srcFound = false, destFound = false;
    Node src, dest, n = Node();
    Node nodeArr[numNodes];             // Node array
    vector<Node> nodes(numNodes);                 // Vector version of node array
    vector<Node>::iterator it1, it2;    // nodes vector iterator

    for (i = 0; i < numNodes; i++) nodeArr[i] = n;
    while (is.good()) {
        // read node labels
        is >> s >> d;
        sl = stoi(s);
        dl = stoi(d);
        // swap if descending for easy search
        if (sl > dl) swap(sl, dl);
        // find partition
        sp = (sl - 1) / (numNodes / numPart);
        dp = (dl - 1) / (numNodes / numPart);

        // check if already found in the array
        // increment edge weight if found
        for (i=0; i< numNodes; i++) {
            if (nodeArr[i].label == sl) {
                srcFound = true;
                // self loop?
                if (sl == dl) destFound = true;
                (nodeArr[i].connected[dl].weight)++;
            } else if (nodeArr[i].label == dl) {
                destFound = true;
                (nodeArr[i].connected[sl].weight)++;
            }
        }

        // new node
        if (!srcFound && !destFound) {
            if (sl == dl) { // self loop?
                src = Node(sl,sp);
                nodeArr[sl-1] = src;
                (nodeArr[sl-1].connected[sl].weight)++;
            } else {
                src = Node(sl,sp);
                dest = Node(dl,dp);
                nodeArr[sl-1] = src;
                nodeArr[dl-1] = dest;
                (nodeArr[sl-1].connected[dl].weight)++;
                (nodeArr[dl-1].connected[sl].weight)++;
            }
        } else if (!srcFound) {
            src = Node(sl,sp);
            nodeArr[sl-1] = src;
            (nodeArr[sl-1].connected[dl].weight)++;
        } else if (!destFound) {
            dest = Node(dl,dp);
            nodeArr[dl-1] = dest;
            (nodeArr[dl-1].connected[sl].weight)++;
        }
        // determine if edge is cut
        nodeArr[sl-1].connected[dl].cut = (sp != dp);
        if (sl != dl) nodeArr[dl-1].connected[sl].cut = (sp != dp);
        srcFound = false;
        destFound = false;
    }
    is.close();

    // switch array to vector for searching
    // for (i=0; i<numNodes; i++) nodes.insert(nodes.begin() + i,nodeArr[i]);
    for (i=0; i<numNodes; i++) nodes[i] = nodeArr[i];

    int max;            // Max improvement
    int imp;            // Improvement value
    int totalCuts = 0;  // Total number of cut edges
    int stopCnt = -1;   // Cut count after tentative swap
    bool firstPass = true;

    map<pair<Node*,Node*>,int> improvement;             // Table of swap pairs and their improvement scores
    map<pair<Node*,Node*>,int>::iterator iit;           // Improvement table iterator
    vector<pair<pair<Node*,Node*>,int>> tentative;      // Tentative swaps list with cut count
    vector<pair<pair<Node*,Node*>,int>>::iterator tit;  // Tentative swaps list iterator
    map<int,Node::Edge>::iterator n1it, n2it;           // Connected list iterator
    pair<Node*,Node*> swapPair;                         // Pair of nodes to potentially swap
    pair<Node*,Node*> swapMax;                          // Pair for tentative swap

    cout << "Starting improvement list:\n";
    do {
        // Find pair with the highest improvement
        max = numeric_limits<int>::min();
        for (it1 = nodes.begin(); it1 != nodes.end(); it1++) {
            for (it2 = nodes.begin(); it2 != nodes.end(); it2++) {
                imp = 0;
                if (it1->partition < it2->partition && !it1->swapped && !it2->swapped) {
                    for (n1it = it1->connected.begin(); n1it != it1->connected.end(); n1it++) {
                        // if first node edge's pairs are in first node's partition, imp++
                        // if first node edge's pairs are in second node's partition, imp--
                        if (nodes[n1it->first-1].partition == it2->partition)imp++;
                        if (nodes[n1it->first-1].partition == it1->partition && nodes[n1it->first-1].label != it1->label) imp--;
                    }
                    for (n2it = it2->connected.begin(); n2it != it2->connected.end(); n2it++) {
                        // if second node edge's pairs are in first node's partition, imp++
                        // if second node edge's pairs are in second node's partition, imp--
                        if (nodes[n2it->first-1].partition == it1->partition) imp++;
                        if (nodes[n2it->first-1].partition == it2->partition && nodes[n2it->first-1].label != it2->label) imp--;
                    }
                    if (it1->connected.find(it2->label) != it1->connected.end()) {
                        imp -= 2;                   // decrease by 2 if nodes are connected
                        if(firstPass) totalCuts++;  // set total cut count in first pass
                    }
                    swapPair = make_pair(&(*it1), &(*it2)); // create swap pair
                    improvement[swapPair] = imp;            // insert it into improvement table
                    
                    #ifdef PRINT
                    cout << "\t[" << swapPair.first->label << ",";
                    cout << swapPair.second->label << "]: " << imp;
                    #endif

                    if (imp > max) {
                        max = imp;
                        swapMax = swapPair;
                        #ifdef PRINT
                        cout << " swapMax";
                        #endif
                    }
                    #ifdef PRINT
                    cout << endl;
                    #endif
                }
            }
        }
        cout << "Improvemenet list size: " << improvement.size() << endl;
        
        // Tentative swap; add to tentative swap list with cut count
        int l1 = swapMax.first->label, l2 = swapMax.second->label;
        int oldp1 = swapMax.first->partition, oldp2 = swapMax.second->partition;
        swap(swapMax.first->partition, swapMax.second->partition);
        swapMax.first->swapped = true;
        swapMax.second->swapped = true;

        if (firstPass) stopCnt = totalCuts;
        firstPass = false;
        #ifdef PRINT
        cout << "\tcuts before: " << stopCnt << endl;
        #endif
        stopCnt -= max;
        tentative.push_back(make_pair(swapMax,stopCnt));

        #ifdef PRINT
        cout << "\tswap: " << swapMax.first->label << "," << swapMax.second->label;
        cout << "\n\timprovement: " << improvement[swapMax];
        cout << "\n\tcuts after:" << tentative.back().second << endl;
        #endif
        
        // Find nodes connected to swap pair nodes and cut or uncut their edges
        for (n1it = swapMax.first->connected.begin(); n1it != swapMax.first->connected.end(); n1it++) {
            if (nodes[n1it->first-1].partition == oldp1) {
                n1it->second.cut = true;
                nodes[n1it->first-1].connected[swapMax.first->label].cut = true;
            }
            if (nodes[n1it->first-1].partition == oldp2) {
                n1it->second.cut = false;
                nodes[n1it->first-1].connected[swapMax.first->label].cut = false;
            }
        }
        for (n2it = swapMax.second->connected.begin(); n2it != swapMax.second->connected.end(); n2it++) {
            if (nodes[n2it->first-1].partition == oldp2) {
                n2it->second.cut = true;
                nodes[n2it->first-1].connected[swapMax.second->label].cut = true;
            }
            if (nodes[n2it->first-1].partition == oldp1) {
                n2it->second.cut = false;
                nodes[n2it->first-1].connected[swapMax.second->label].cut = true;
            }
        }

        // Remove swapped pair nodes from improvement table
        while (true) {
            for (iit = improvement.begin(); iit != improvement.end(); iit++) {
                if (iit->first.first->label == l1 ||
                    iit->first.second->label == l1 ||
                    iit->first.first->label == l2 || 
                    iit->first.second->label == l2) {
                    #ifdef PRINT
                    cout << "Removing [" << iit->first.first->label << "," << iit->first.second->label << "]: " << iit->second << endl;
                    #endif
                    improvement.erase(iit);
                    break;
                }
            }
            if (iit == improvement.end()) break;
        }
        #ifdef PRINT
        cout << "\nRemaining improvement table:\n";
        #endif
    } while (stopCnt != totalCuts && improvement.size() != 0);
    cout << "No more swaps possible\n\n";

    int mincut = totalCuts;                     // Min cuts in tentative swap list
    pair<pair<Node*,Node*>,int> stop;           // Pair for min cut
    int iterations = 0;
    cout << "Cuts after 0 iterations: " << totalCuts << endl;
    for (tit = tentative.begin(); tit != tentative.end(); tit++) {
        cout << "Cuts after " << ++iterations << " iterations: " << tit->second << endl;
        if (tit->second < mincut) {
            mincut = tit->second;
            stop = make_pair(tit->first, tit->second);
        }
    }
    
    iterations = 0;
    // If mincut < totalCuts, complete swaps up to stop
    if (mincut < totalCuts) {
        for (i=0; i<numNodes; i++) nodes[i] = nodeArr[i];
        tit = tentative.begin();
        while (true) {
            totalCuts = tit->second;
            Node *n1 = tit->first.first;
            Node *n2 = tit->first.second;
            for (n1it = n1->connected.begin(); n1it != n1->connected.end(); n1it++) {
                if (nodes[n1it->first-1].partition == n1->partition && nodes[n1it->first-1].label != n1->label) {
                    n1it->second.cut = true;
                    nodes[n1it->first-1].connected[n1->label].cut = true;
                }
                if (nodes[n1it->first-1].partition == n2->partition && nodes[n1it->first-1].label != n2->label) {
                    n1it->second.cut = false;
                    nodes[n1it->first-1].connected[n1->label].cut = false;
                }
            }
            for (n2it = n2->connected.begin(); n2it != n2->connected.end(); n2it++) {
                if (nodes[n2it->first-1].partition == n2->partition && nodes[n2it->first-1].label != n2->label) {
                    n2it->second.cut = true;
                    nodes[n2it->first-1].connected[n2->label].cut = true;
                }
                if (nodes[n2it->first-1].partition == n1->partition && nodes[n2it->first-1].label != n1->label) {
                    n2it->second.cut = false;
                    nodes[n2it->first-1].connected[n2->label].cut = false;
                }
            }
            swap(n1->partition, n2->partition);
            for (Node n : nodes) {
                if (n.label == n1->label) n = *n1;
                if (n.label == n2->label) n = *n2;
            }
            iterations++;
            if (tit->first == stop.first) {
                cout << "Stopped at mincut swap.\nLast swap ";
                cout << stop.first.first->label << "," << stop.first.second->label;
                cout << " with " << stop.second << " cuts remaining\n";
                break;
            }
            tit++;
            if (tit == tentative.end()) {
                cout << "No more swaps possible.\n";
                break;
            }
        }
    }
    int nodePerPart = numNodes/numPart;
    vector<Node> nodeTable[numPart];
    vector<pair<int,int>> cutset;
    for (Node n : nodes) {
        nodeTable[n.partition].push_back(n);
        for (n1it = n.connected.begin(); n1it != n.connected.end(); n1it++) {
            if (n1it->second.cut && n.label < n1it->first) cutset.push_back(make_pair(n.label,n1it->first));
        }
    }
    for (i=0; i<numPart; i++) {
        cout << "Partition " << i << endl;
        for (Node n : nodeTable[i]) cout << "\t" << n.label << endl;
    }
    cout << "Min cutset value after " << iterations << " iterations: " << totalCuts << endl;
    for (pair<int,int> p : cutset) {
        cout << p.first << ", " << p.second << endl;
    }
    return 0;
}