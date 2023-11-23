#include "coskuner9366.h"
int main(/* int argc, char** argv */) {
    int n, C, RAT, up, down, length;
    vector<Node> nodes;
    vector<Edge> edges;
    cout << "How many sinks? "; cin >> n;
    for (int i=1; i<=n; i++) {
        cout << "For sink " << i << "... \n";
        cout << "\tWhat is the sink capacitance? ";           cin >> C;
        cout << "\tWhat is the Required Time of Arrival (RAT)? ";   cin >> RAT;
        nodes.push_back(Node(i, C, RAT, false, false));
    }
    for (int i=0; i<2*n-1; i++) {
        cout << "For edge " << i << "... \n";
        cout << "\tWhat is the upstream node index? ";              cin >> up;
        cout << "\tWhat is the downstream node index? ";            cin >> down;
        cout << "\tWhat is the edge length? ";                      cin >> length;
        edges.push_back(Edge(up, down, length));
    }

    ofstream os;
    os.open("input.txt", ios::out | ios::trunc);
    os << n << endl;
    for (Node n : nodes) {
        os << n.C << " " << n.RAT << endl;
    }
    for (Edge e : edges) {
        os << e.up << " " << e.down << " " << e.length << endl;
    }
    os.close();
    return 0;
}