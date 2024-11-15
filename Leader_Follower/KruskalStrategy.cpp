#include "MSTStrategy.hpp"
#include <algorithm>

// Kruskal strategy class implementing the MSTStrategy interface
class KruskalStrategy : public MSTStrategy {
public:
    Tree buildTree(Graph& graph) override {
        int V = graph.getVertices();
        Tree tree(V);

        vector<Edge> edges = graph.getEdges();
        sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
            return a.weight < b.weight;
        });

        vector<int> parent(V), rank(V, 0);
        for (int i = 0; i < V; ++i) parent[i] = i;

        for (const Edge& edge : edges) {
            if (findSet(parent, edge.src) != findSet(parent, edge.dest)) {
                tree.addTreeEdge(edge.index, edge.src, edge.dest, edge.weight);
                unionSets(parent, rank, edge.src, edge.dest);
            }
        }

        return tree;
    }

private:
    int findSet(vector<int>& parent, int v) {
        if (parent[v] != v)
            parent[v] = findSet(parent, parent[v]);
        return parent[v];
    }

    void unionSets(vector<int>& parent, vector<int>& rank, int u, int v) {
        int rootU = findSet(parent, u);
        int rootV = findSet(parent, v);
        if (rootU != rootV) {
            if (rank[rootU] > rank[rootV])
                parent[rootV] = rootU;
            else if (rank[rootU] < rank[rootV])
                parent[rootU] = rootV;
            else {
                parent[rootV] = rootU;
                rank[rootU]++;
            }
        }
    }
};


