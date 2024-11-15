#include "Graph.hpp"
#include <bits/algorithmfwd.h>

// Constructor
Graph::Graph(int V) : V(V) {
    adjMatrix.resize(V, vector<int>(V, 0));
}

// Add edge to the graph with an index
void Graph::addEdge(int index, int src, int dest, int weight) {
    std::lock_guard<std::mutex> lock(graphMutex);

    adjMatrix[src][dest] = weight;
    adjMatrix[dest][src] = weight; // For undirected graph

    edges.push_back(Edge(index, src, dest, weight)); // Store the edge with index
}

// Remove edge from the graph
void Graph::removeEdge(int src, int dest) {
    std::lock_guard<std::mutex> lock(graphMutex);

    adjMatrix[src][dest] = 0;
    adjMatrix[dest][src] = 0;
    
    
    //  // Remove the edge from the edge list

    for (auto it = edges.begin(); it != edges.end(); /* ללא עלייה באיטרטור כאן */) {
        if ((it->src == src && it->dest == dest) || (it->src == dest && it->dest == src)) {
            it = edges.erase(it); // מחיקת הקשת והתעדכנות לאיטרטור הבא
        } else {
            ++it; // המשך לאיטרטור הבא
        }
    }
}

// Print the adjacency matrix
void Graph::printGraph() {
    std::lock_guard<std::mutex> lock(graphMutex);

    cout << "Adjacency Matrix:" << endl;
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

void Graph::resetGraph(int newV) {
    std::lock_guard<std::mutex> lock(graphMutex); // נועלים את הגרף בזמן שינוי
    V = newV;
    adjMatrix.assign(V, std::vector<int>(V, 0)); // אתחול מחדש של מטריצת הסמיכויות
    edges.clear(); // איפוס רשימת הקשתות
}


// Print the edges with indexes
void Graph::printEdges() {
    cout << "Edges in the graph (index, src, dest, weight):" << endl;
    for (const auto& edge : edges) {
        cout << "(" << edge.index << ", " << edge.src << ", " << edge.dest << ", " << edge.weight << ")" << endl;
    }
}

// Get the adjacency matrix
vector<vector<int>>& Graph::getAdjMatrix() {
     std::lock_guard<std::mutex> lock(graphMutex);

    return adjMatrix;
}

// Get number of vertices
int Graph::getVertices() const {
     std::lock_guard<std::mutex> lock(graphMutex);

    return V;
}

// Get the list of edges
const vector<Edge>& Graph::getEdges() const {
     std::lock_guard<std::mutex> lock(graphMutex);

    return edges;
}

void Graph::setEdges(const std::vector<Edge>& newEdges) {
     std::lock_guard<std::mutex> lock(graphMutex);
     
    edges = newEdges;  // עדכון רשימת הקשתות
    adjMatrix = std::vector<std::vector<int>>(V, std::vector<int>(V, 0));  // איפוס מטריצת השכנויות

    // הוספת הקשתות למטריצה החדשה
    for (const auto& edge : edges) {
        adjMatrix[edge.src][edge.dest] = edge.weight;
        adjMatrix[edge.dest][edge.src] = edge.weight;  // כיוון שהגרף לא מכוון
    }
}
