#include "Tree.hpp"
#include <algorithm>
#include <climits>
#include <queue>
#include <numeric>


// Constructor
Tree::Tree(int V) : V(V), edgeCount(0) {}

// Add edge to the tree
void Tree::addTreeEdge(int index, int src, int dest, int weight) {
    treeEdges.push_back(Edge(index, src, dest, weight));
    edgeCount++;
}

// Print the tree edges
void Tree::printTree() {
    cout << "Tree edges (index, src, dest, weight):" << endl;
    for (const auto& edge : treeEdges) {
        cout << "(" << edge.index << ", " << edge.src << ", " << edge.dest << ", " << edge.weight << ")" << endl;
    }

    cout << "Total number of vertices in the tree: " << V << endl;
    cout << "Total number of edges in the tree: " << edgeCount << endl;
    
}

// Get number of edges in the tree
int Tree::getEdgeCount() const {
    return edgeCount;
}

// Get total weight of the MST
int Tree::getTotalWeight() const {
    int totalWeight = 0;
    for (const auto& edge : treeEdges) {
        totalWeight += edge.weight;
    }
    return totalWeight;
}

// Helper function to perform BFS/DFS to find longest path (used for longest distance)
int bfs(const vector<vector<pair<int, int>>>& adj, int start, vector<int>& dist) {
    int n = adj.size();
    vector<bool> visited(n, false);
    queue<int> q;

    q.push(start);
    dist[start] = 0;
    visited[start] = true;

    int farthestNode = start;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (const auto& neighbor : adj[node]) {
            int next = neighbor.first;
            int weight = neighbor.second;

            if (!visited[next]) {
                visited[next] = true;
                dist[next] = dist[node] + weight;
                q.push(next);

                if (dist[next] > dist[farthestNode]) {
                    farthestNode = next;
                }
            }
        }
    }

    return farthestNode;
}

// Get the longest distance between two vertices in the MST
int Tree::getLongestDistance() const {
    // Build adjacency list from the tree edges
    vector<vector<pair<int, int>>> adj(V);
    for (const auto& edge : treeEdges) {
        adj[edge.src].push_back({edge.dest, edge.weight});
        adj[edge.dest].push_back({edge.src, edge.weight});
    }

    // Run BFS from an arbitrary node to find the farthest node
    vector<int> dist(V, 0);
    int farthestNode = bfs(adj, 0, dist);

    // Run BFS again from the farthest node to find the maximum distance
    dist.assign(V, 0);
    farthestNode = bfs(adj, farthestNode, dist);

    // The maximum distance found
    return *max_element(dist.begin(), dist.end());
}

// BFS helper function to calculate shortest distance between any two vertices
int bfsShortestDistance(const std::vector<std::vector<std::pair<int, int>>>& adj, int start, int target) {
    int n = adj.size();
    std::vector<int> dist(n, INT_MAX);
    std::queue<int> q;

    q.push(start);
    dist[start] = 0;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (const auto& neighbor : adj[node]) {
            int next = neighbor.first;
            int weight = neighbor.second;

            if (dist[next] == INT_MAX) {
                dist[next] = dist[node] + weight;
                q.push(next);
            }
        }
    }

    return dist[target];
}


// Get the average distance between every two vertices in the MST
double Tree::getAverageDistance() const {
    // Build adjacency list from the tree edges
    std::vector<std::vector<std::pair<int, int>>> adj(V);
    for (const auto& edge : treeEdges) {
        adj[edge.src].push_back({edge.dest, edge.weight});
        adj[edge.dest].push_back({edge.src, edge.weight});
    }

    // Initialize variables to store total distance and count of pairs
    int totalDistance = 0;
    int pairCount = 0;

    // Iterate over all pairs of vertices to calculate the shortest distance between them
    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            int distance = bfsShortestDistance(adj, i, j);
            if (distance != INT_MAX) {
                totalDistance += distance;
                pairCount++;
            }
        }
    }

    // Calculate the average distance between every two vertices
    return pairCount > 0 ? static_cast<double>(totalDistance) / pairCount : 0.0;
}



// Get the shortest distance between any two vertices in the MST
int Tree::getShortestDistance() const {
    // Build adjacency list from the tree edges
    std::vector<std::vector<std::pair<int, int>>> adj(V);
    for (const auto& edge : treeEdges) {
        adj[edge.src].push_back({edge.dest, edge.weight});
        adj[edge.dest].push_back({edge.src, edge.weight});
    }

    // Iterate over all pairs of vertices to find the minimum distance
    int shortestDistance = INT_MAX;
    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            int distance = bfsShortestDistance(adj, i, j);
            if (distance < shortestDistance) {
                shortestDistance = distance;
            }
        }
    }

    return shortestDistance;
}