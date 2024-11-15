#include "Graph.hpp"
#include "Tree.hpp"
#include "MSTFactory.cpp"

int main() {
    // Create a graph with 6 vertices
    Graph graph(6);

    // Add some edges with weights and indexes
    graph.addEdge(0, 0, 1, 10);
    graph.addEdge(1, 2, 3, 6);
    graph.addEdge(2, 2, 4, 8);
    graph.addEdge(3, 4, 5, -1);
    graph.addEdge(4, 1, 3, 15);
    graph.addEdge(5, 1, 2, 3);

    // Print the graph's adjacency matrix
    graph.printGraph();

    // Create MST strategy using the factory
    MSTStrategy* mstStrategy = MSTFactory::createMSTStrategy(MSTType::KRUSKAL);
    
    // Build and print MST using Kruskal's algorithm
    Tree kruskalTree = mstStrategy->buildTree(graph);
    cout << "\nKruskal's MST:" << endl;
    kruskalTree.printTree();
    
    delete mstStrategy; // Clean up

    // Create MST strategy using the factory
    mstStrategy = MSTFactory::createMSTStrategy(MSTType::PRIM);

    // Build and print MST using Prim's algorithm
    Tree primTree = mstStrategy->buildTree(graph);
    cout << "\nPrim's MST:" << endl;
    primTree.printTree();

    cout << "\n\n Hisuvim!!!!!!!!!!!!!!!!!!!!" << endl;

    cout << "\n Prim!!!!!!!!!!!!!!!!!!!!" << endl;
    // Perform calculations on the MST
    cout << "Total Weight of MST: " << primTree.getTotalWeight() << endl;
   
    cout << "Longest Distance in MST: " << primTree.getLongestDistance() << endl;
    //cout << "Average Distance in MST: " << primTree.getAverageDistance() << endl;
    //cout << "Shortest Distance in MST: " << primTree.getShortestDistanceBetween() << endl;
    primTree.printTree();
    cout << "\n Kruskal!!!!!!!!!!!!!!!!!!!!" << endl;

    cout << "Total Weight of MST: " << kruskalTree.getTotalWeight() << endl;
    cout << "Longest Distance in MST: " << kruskalTree.getLongestDistance() << endl;
    //cout << "Average Distance in MST: " << kruskalTree.getAverageDistance() << endl;
    //cout << "Shortest Distance in MST: " << kruskalTree.getShortestDistanceBetween() << endl;

    
    
    
    
    
    
    delete mstStrategy; // Clean up

    return 0;
}
    // Build and print MST using Kruskal's algorithm
