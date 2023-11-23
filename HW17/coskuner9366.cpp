#include "coskuner9366.h"
// #define DEBUG
// #define PRINT

const int r=1, c=1, Rb=1, Cb=1, tb=1, Rd=1;

bool operator< (const Node node1, const Node node2) {
    if (node1.C == node2.C) {
        return node1.RAT > node2.RAT;
    }
    return node1.C < node2.C;
}
bool operator> (Node node1, Node node2) {
    return node1.RAT < node2.RAT;
}
bool operator== (Node node1, Node node2) {
    return (
        node1.id == node2.id &&
        node1.C == node2.C &&
        node1.RAT == node2.RAT &&
        node1.isSink == node2.isSink &&
        node1.hasBuffer == node2.hasBuffer &&
        node1.branch == node2.branch &&
        node1.pred == node2.pred &&
        node1.left == node2.left &&
        node1.right == node2.right
    );
}
Node addWire(Node downstream, int length) {
    int C;
    float RAT;
    C = downstream.C + c*length;
    RAT = downstream.RAT - r*length*(c*length/2.0 + downstream.C);
    return Node(downstream.pred->id, C, RAT, false, false);
}
Node insertBuffer(int id, Node downstream) {
    float RAT = downstream.RAT - Rb*downstream.C - tb;
    Node buf = Node(id, Cb, RAT, false, true);
    buf.hasBuffer = true;
    return buf;
}
Node merge(Node left, Node right) {
    return Node(left.id, left.C + right.C, min(left.RAT, right.RAT), false, false);
}

float addDriver(Node driver) {
    return driver.RAT - Rd*driver.C;
}

int main(int argc, char** argv) {
    int n;
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
    is >> n;
    Node nodes[2*n];
    Edge edges[2*n-1];
    // Init vars by reading from file
    for (int i=0; i<=2*n-1; i++) {
        if (i>0 && i<=n) {
            int C;
            float RAT;
            is >> C >> RAT;
            nodes[i] = Node(i, C, RAT, true, false);
        } else {
            nodes[i] = Node(i, 0, 0, false, false);
        }
    }
    for (int i=0; i<2*n-1; i++) {
        int up, down, length;
        is >> up >> down >> length;
        edges[i] = Edge(up, down, length);
        if (nodes[up].left == nullptr) {
            nodes[up].left = &nodes[down];
            nodes[down].branch = "L";
        } else if (nodes[up].right == nullptr) {
            nodes[up].right = &nodes[down];
            nodes[down].branch = "R";
        } else {
            // Both branches are not null, error
            cerr << "More than 2 children for node " << up << endl;
        }
        nodes[down].pred = &nodes[up];
    }
    is.close();

    vector<map<string,Node>> solutions;
    map<string,Node> optimal;
    float maxSlack = numeric_limits<float>::min();
    for (int i=2*n-2; i>=0; i--) {
        Node wire, buf, *up, *down;
        up = &nodes[edges[i].up];
        down = &nodes[edges[i].down];
        if (i==0 && down->mergedInternal) {
            map<map<string,Node>, float> slack;
            for (map<string,Node> map : solutions) {
                slack[map] = addDriver(addWire(map[to_string(down->id)+"M"], edges[i].length));
                if (slack[map] > maxSlack) {
                    maxSlack = slack[map];
                    optimal = map;
                }
            }
            break;
        }
        if (down->isSink || down->mergedInternal) {
            #ifdef PRINT
            cout << "Generating wire and buffer candidates for " << up->id << down->branch << endl;
            #endif
            // Duplicate solution candidates list
            int size = solutions.size();
            for (int i=0; i<size; i++) {
                solutions.push_back(solutions[i]);
            }
            
            // Generate new addition to each solution
            wire = addWire(*down, edges[i].length);
            buf = insertBuffer(up->id, wire);
            // Add to set of candidates
            if (solutions.empty()) {
                solutions.push_back(map<string,Node>());
                solutions.push_back(map<string,Node>());
                solutions[0][to_string(up->id)+down->branch] = wire;
                solutions[1][to_string(up->id)+down->branch] = buf;
            } else {
                for (int j=0; j<solutions.size(); j++) {
                    if (down->mergedInternal) {
                        wire = addWire(solutions[j][to_string(down->id)+"M"], edges[i].length);
                        buf = insertBuffer(up->id, wire);
                    }
                    solutions[j][to_string(up->id)+down->branch] = (j<solutions.size()/2) ? wire : buf;
                }
            }
        } else {
            // Merge right and left candidates and prune
            if (down->left != nullptr && down->right != nullptr) {
                #ifdef PRINT
                cout << "Merging " << down->id << endl; 
                #endif
                set<Node, greater<Node>> mergeList;
                set<Node, less<Node>> leftSet, rightSet, mergedSet;
                // Generate left and right candidate sets
                for (map<string,Node> map : solutions) {
                    Node left = map[to_string(down->id)+"L"];
                    Node right = map[to_string(down->id)+"R"];
                    mergeList.insert(left);
                    mergeList.insert(right);
                    leftSet.insert(left);
                    rightSet.insert(right);
                }
                // Generate new merged set and add to solutions
                for (Node node : mergeList) {
                    bool removeL = false, removeR = false;
                    set<Node>::iterator left, right;
                    const Node *leftNode, *rightNode;
                    if (leftSet.empty() || rightSet.empty()) {
                        break;
                    } else if ((left = leftSet.find(node)) != leftSet.end()) {
                        leftNode = &*left;
                        rightNode = &*rightSet.begin();
                        removeL = true;
                    } else if ((right = rightSet.find(node)) != rightSet.end()) {
                        leftNode = &*leftSet.begin();
                        rightNode = &*right;
                        removeR = true;
                    }
                    int j=0;
                    for (j=0; j<solutions.size(); j++) {
                        if ((*leftNode == solutions[j][to_string(down->id)+"L"]) &&
                            (*rightNode == solutions[j][to_string(down->id)+"R"]))
                        {
                            Node mergeNode = merge(*leftNode, *rightNode);
                            mergeNode.pred = up;
                            #ifdef PRINT
                            cout << "Adding " << mergeNode.C << "," << mergeNode.RAT
                                 << " to (" << j << "," << down->id << "M)\n";
                            #endif
                            solutions[j][to_string(down->id)+"M"] = mergeNode;
                            mergedSet.insert(mergeNode);
                        }
                    }
                    if (removeL) leftSet.erase(left);
                    if (removeR) rightSet.erase(right);
                }
                // Remove inferior candidates from merged set
                #ifdef PRINT
                cout << "Pruning merge set\n";
                #endif
                set<Node>::iterator it1 = mergedSet.begin(), it2 = next(it1,1);
                while (it1 != mergedSet.end() && it2 != mergedSet.end()) {
                    if (it1->RAT >= it2->RAT) {
                        mergedSet.erase(it2);
                    } else {
                        it1 = it2;
                    }
                    it2 = next(it1,1);
                }
                // Remove inferior candidates from solutions
                #ifdef PRINT
                cout << "Pruning inferior solutions\n\n";
                #endif
                while (true) {
                    int k=0;
                    for (k=0; k<solutions.size(); k++) {
                        if (solutions[k].find(to_string(down->id)+"M") == solutions[k].end() ||
                            mergedSet.find(solutions[k][to_string(down->id)+"M"]) == mergedSet.end())
                        {
                            solutions.erase(solutions.begin()+k);
                            break;
                        }
                    }
                    if (k == solutions.size()) break;
                }
            } else {
                cerr << "One or both children of non-sink node " << down->id << " are null\n";
            }
            down->mergedInternal = true;
            i++;
        }
    }

    int bufCount = 0;
    // Count and mark edges if it has buffer
    for (int i=0; i<sizeof(edges)/sizeof(Edge); i++) {
        string key = to_string(edges[i].up)+(nodes[edges[i].down].branch);
        if (optimal.find(key) != optimal.end() && optimal[key].hasBuffer) {
            edges[i].hasBuffer = true;
            bufCount++;
        }
    }
    cout << "Total number of buffers inserted: " << bufCount << endl;
    for (Edge e : edges) {
        if (e.hasBuffer)
            cout << "buffer inserted at node " << e.up
                << " (on the edge to node " << e.down << ")" << endl;
    }
    cout << "Required arrival time: " << maxSlack << endl;
    return 0;
}