import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

public class Dijkstra {

    final static String filename = "C:\\Users\\acosk\\Documents\\GitHub\\ECEN 687\\Dijkstra.input";
    public static class Node {
        int name, label;
        Node predecessor;
        public boolean match (int x) {
            return name == x;
        }
        public Node() {
            name = -1;
            label = (int) Double.POSITIVE_INFINITY;
            predecessor = null;
        }
        public Node(int name) {
            this();
            this.name = name;
        }
    }

    public static class Edge {
        Node source;
        Node destination;
        int weight;
        public Edge(Node source, Node destination, int weight) {
            this.source = source;
            this.destination = destination;
            this.weight = weight;
        }
    }

    public static void main(String[] args) {
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            ArrayList<Edge> edges = new ArrayList<Edge>();
            ArrayList<Node> nodes = new ArrayList<Node>();
            ArrayList<Node> queue = new ArrayList<Node>();
            ArrayList<Node> path = new ArrayList<Node>();
            ArrayList<Node> seen = new ArrayList<Node>();
            
            for (int i = 1; i <= 8; i++) {
                nodes.add(new Node(i));
                System.out.println(nodes.get(i-1) + String.format("i %d", i));
            }

            int w;
            Node s = null, d = null;
            Node start = nodes.get(5), end = nodes.get(7);
            start.label = 0;
            queue.add(start);
            seen.add(start);

            String line = br.readLine();
            while (line != null) {
                for (Node n : nodes) {
                    if (n.match(Integer.parseInt(line.substring(1,2)))) s = n;
                    if (n.match(Integer.parseInt(line.substring(4,5)))) d = n;
                }
                if (s == null || d == null) throw new Exception("No source or destination found");
                w = Integer.parseInt(line.substring(7,8));
                edges.add(new Edge(s,d,w));
                line = br.readLine();
            }

            while (queue.size() != 0) {
                Node u = queue.remove(0);
                for (Edge e : edges) {
                    if (u == end) break;
                    if (e.source != u) continue;
                    if (e.destination.label > u.label + e.weight) {
                        e.destination.label = u.label + e.weight;
                        e.destination.predecessor = u;
                    }
                    if (!seen.contains(e.destination)) {
                        boolean added = false;
                        for (int i = 0; i < queue.size(); i++) {
                            if (e.destination.label < queue.get(i).label) {
                                queue.add(queue.indexOf(queue.get(i)), e.destination);
                                added = true;
                                break;
                            }
                        }
                        if (!added) queue.add(e.destination);
                        seen.add(e.destination);
                    }
                }
            }
            for (Node n = end; n != start.predecessor; n = n.predecessor) {
                path.add(0, n);
            }
            for (Node n : path) {
                System.out.printf("Name: %d, Label: %d%n", n.name, n.label);
            }

            br.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
