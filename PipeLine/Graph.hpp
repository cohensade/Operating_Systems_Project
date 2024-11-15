#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream>
#include <mutex>

using namespace std;

// Struct representing an edge between two vertices with a weight and an index
struct Edge {
    int index;
    int src, dest, weight;
    Edge(int idx, int s, int d, int w) : index(idx), src(s), dest(d), weight(w) {}
};

// Class representing the graph
class Graph {
    int V; // Number of vertices
    vector<vector<int>> adjMatrix; // Adjacency matrix
    vector<Edge> edges; // List of edges with indexes

private:
    // פונקציה לבדיקה אם קשת היא זו שצריך למחוק
    bool isEdgeToRemove(const Edge& e, int src, int dest) const {
        return (e.src == src && e.dest == dest) || (e.src == dest && e.dest == src);
    }
    mutable std::mutex graphMutex; //Allow mutex in const functions



public:
    // Constructor
    Graph(int V);

    // Add edge to the graph with an index
    void addEdge(int index, int src, int dest, int weight);

    // Remove edge from the graph
    void removeEdge(int src, int dest);

    // Print the adjacency matrix
    void printGraph();

    // Print the edges with indexes
    void printEdges();


    void resetGraph(int newV);



    // Get the adjacency matrix
    vector<vector<int>>& getAdjMatrix();

    // Get number of vertices
    int getVertices() const;

    // Get the list of edges
    const vector<Edge>& getEdges() const;

    void setEdges(const std::vector<Edge>& newEdges);
};

#endif // GRAPH_HPP
