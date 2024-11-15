#ifndef TREE_HPP
#define TREE_HPP

#include "Graph.hpp"
#include <vector>

using namespace std;

// Class representing a tree built from a graph
class Tree {
    int V; // Number of vertices
    int edgeCount; // Number of edges in the tree
    vector<Edge> treeEdges; // Collection of edges forming the tree

public:
    // Constructor
    Tree(int V);

    // Add edge to the tree
    void addTreeEdge(int index, int src, int dest, int weight);

    // Print the tree edges
    void printTree();

    // Get number of edges in the tree
    int getEdgeCount() const;

    int getTotalWeight() const;  // Total weight of MST

    int getLongestDistance() const;   // Longest distance between two vertices

    double getAverageDistance() const; // Average distance between all pairs

     int getShortestDistance() const; // Shortest distance between two vertices in the MST (i != j) using DFS/BFS
};



#endif // TREE_HPP
