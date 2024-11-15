#include "MSTStrategy.hpp"
#include <queue>
#include <climits>

// Prim strategy class implementing the MSTStrategy interface
class PrimStrategy : public MSTStrategy {
public:
    Tree buildTree(Graph& graph) override {
        int V = graph.getVertices();
        Tree tree(V);

        vector<int> key(V, INT_MAX);
        vector<bool> inMST(V, false);
        vector<int> parent(V, -1);

        key[0] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, 0});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            inMST[u] = true;

            for (int v = 0; v < V; ++v) {
                // Check if there's an edge and if the vertex v is not in the MST yet
                if (graph.getAdjMatrix()[u][v] && !inMST[v] && graph.getAdjMatrix()[u][v] < key[v]) {
                    key[v] = graph.getAdjMatrix()[u][v];
                    pq.push({key[v], v});
                    parent[v] = u;
                }
            }
        }

        // After Prim's algorithm is done, add edges to the tree using the correct index
        const vector<Edge>& edges = graph.getEdges();

        for (int v = 1; v < V; ++v) {
            if (parent[v] != -1) {
                int src = parent[v];
                int dest = v;

                // Find the edge that corresponds to (src, dest) in the original graph
                for (const auto& edge : edges) {
                    if ((edge.src == src && edge.dest == dest) || (edge.src == dest && edge.dest == src)) {
                        // Add the edge with the correct index from the original graph
                        tree.addTreeEdge(edge.index, edge.src, edge.dest, edge.weight);
                        break; // Break once we've found the correct edge
                    }
                }
            }
        }

        return tree;
    }
};
