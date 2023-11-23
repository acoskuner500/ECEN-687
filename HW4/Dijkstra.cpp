#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
using namespace std;
#define filename "C:\\Users\\acosk\\Documents\\GitHub\\ECEN 687\\Dijkstra.input"

class Node {
    public:
        int name = -1;
        int label = numeric_limits<int>::max();
        Node* predecessor = nullptr;

        Node();
        // Node(const Node&);
        Node(int n): name(n) {}
        bool match (int x) {
            return name == x;
        }
};

class Edge {
    public:
        Node* source;
        Node* destination;
        int weight;

        Edge();
        Edge(Node* s, Node* d, int w): source(s), destination(d), weight(w) {}
};

int main() {
    ifstream is;
    is.open(filename, ios::in);
    vector<Edge*> edges;
    vector<Node*> nodes;
    vector<Node*> queue;
    vector<Node*> path;
    vector<Node*> seen;
    for (int i = 1; i <= 8; i++) {
        nodes.push_back(new Node(i));
    }
    int w;
    Node* s, * d;
    Node* start = nodes.at(5);
    Node* end = nodes.at(7);
    start->label = 0;
    queue.push_back(start);
    seen.push_back(start);

    string line;
    getline(is,line);

    while (is) {
        for (Node* n : nodes) {
            if (n->match(line[1] - 48)) s = n;
            if (n->match(line[4] - 48)) d = n;
        }
        if (s == nullptr || d == nullptr) throw ("Missing nodes");
        w = line[7] - 48;
        edges.push_back(new Edge(s,d,w));
        getline(is,line);
    }

    while (queue.size() != 0) {
        Node* u = queue.front();
        queue.erase(queue.begin());
        for (Edge* e : edges) {
            if (u == end) break;
            if (e->source != u) continue;
            if (e->destination->label > u->label + e->weight) {
                e->destination->label = u->label + e->weight;
                e->destination->predecessor = u;
            }
            bool seenBefore = false;
            for (Node* n : seen) {
                if (n == e->destination) {
                    seenBefore = true;
                    break;
                }
            }
            if (!seenBefore) {
                bool added = false;
                for (int i = 0; i < queue.size(); i++) {
                    if (e->destination->label < queue.at(i)->label) {
                        queue.insert(queue.begin() + i, e->destination);
                        added = true;
                        break;
                    }
                }
                if (!added) queue.push_back(e->destination);
                seen.push_back(e->destination);
            }
        }
    }
    for (Node* n = end; n != start->predecessor; n = n->predecessor) {
        path.insert(path.begin(),n);
    }
    for (Node* n : path) {
        printf("Name: %d, Label: %d\n", n->name, n->label);
    }
    is.close();
}