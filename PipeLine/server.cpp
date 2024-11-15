#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <memory>
#include <thread>
#include <mutex>
#include "Graph.hpp"
#include "MSTFactory.cpp"
#include "ActiveObject.hpp"

#define PORT 9035

std::mutex cout_mutex;
std::atomic<bool> isServerRunning(true);

void PRINT_SAFE(const std::string& message) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << message << std::endl;
}

// Handle graph modifications (ADD_EDGE, REMOVE_EDGE) for a specific client graph
void handleGraphModification(int clientSocket, const std::string& command, std::shared_ptr<Graph> clientGraph) {
    std::stringstream ss(command);
    std::string operation;
    ss >> operation;

    if (operation == "ADD_EDGE") {
        int index, src, dest, weight;
        ss >> index >> src >> dest >> weight;
        clientGraph->addEdge(index, src, dest, weight);
        std::string response = "Edge added successfully.\n";
        send(clientSocket, response.c_str(), response.length(), 0);
    } 
    else if (operation == "REMOVE_EDGE") {
        int src, dest;
        ss >> src >> dest;
        clientGraph->removeEdge(src, dest);
        std::string response = "Edge removed successfully.\n";
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}

// Handle graph creation (CREATE_GRAPH) for a specific client graph
void handleCreateGraph(int clientSocket, const std::string& command, std::shared_ptr<Graph> clientGraph) {
    std::stringstream ss(command);
    std::string operation;
    int numVertices;
    ss >> operation >> numVertices;

    clientGraph->resetGraph(numVertices);  // Reinitialize the client's graph
    std::string response = "Graph created with " + std::to_string(numVertices) + " vertices.\n";
    send(clientSocket, response.c_str(), response.length(), 0);
}

void handlePrintGraph(int clientSocket, std::shared_ptr<Graph> clientGraph, MSTType selectedAlgorithm) {
    std::stringstream ss;

    // Print the adjacency matrix
    ss << "Graph Adjacency Matrix:\n";
    for (int i = 0; i < clientGraph->getVertices(); ++i) {
        for (int j = 0; j < clientGraph->getVertices(); ++j) {
            ss << clientGraph->getAdjMatrix()[i][j] << " ";
        }
        ss << "\n";
    }

    // Recalculate the MST based on the selected algorithm
    MSTStrategy* mstStrategy = MSTFactory::createMSTStrategy(selectedAlgorithm);
    Tree mstTree = mstStrategy->buildTree(*clientGraph);

    // Perform calculations on the MST
    int totalWeight = mstTree.getTotalWeight();
    int longestDistance = mstTree.getLongestDistance();
    double avgDistance = mstTree.getAverageDistance();
    int shortestDistance = mstTree.getShortestDistance();

    // Display the selected algorithm
    if (selectedAlgorithm == MSTType::KRUSKAL) {
        ss << "\nThe current selected algorithm: KRUSKAL\n";
    } else if (selectedAlgorithm == MSTType::PRIM) {
        ss << "\nThe current selected algorithm: PRIM\n";
    }

    // Display MST calculations
    ss << "\nMST Calculations:\n";
    ss << "Total Weight of MST: " << totalWeight << "\n";
    ss << "Longest Distance in MST: " << longestDistance << "\n";
    ss << "Average Distance in MST: " << avgDistance << "\n";
    ss << "Shortest Distance in MST: " << shortestDistance << "\n";

    delete mstStrategy; // Clean up dynamically allocated MST strategy

    // Send the result back to the client
    std::string response = ss.str();
    send(clientSocket, response.c_str(), response.length(), 0);
}

// Handle selection of MST algorithm by the client (KRUSKAL, PRIM) for a specific client graph
void handleMSTAlgorithmSelection(int clientSocket, const std::string& command, MSTType& selectedAlgorithm) {
    std::stringstream ss(command);
    std::string operation, algorithm;
    ss >> operation >> algorithm;

    if (algorithm == "KRUSKAL") {
        selectedAlgorithm = MSTType::KRUSKAL;
        std::string response = "Kruskal algorithm selected.\n";
        send(clientSocket, response.c_str(), response.length(), 0);
    } else if (algorithm == "PRIM") {
        selectedAlgorithm = MSTType::PRIM;
        std::string response = "Prim algorithm selected.\n";
        send(clientSocket, response.c_str(), response.length(), 0);
    } else {
        std::string response = "Invalid algorithm. Please select KRUSKAL or PRIM.\n";
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}

// Handle individual client connection
void handleClient(int clientSocket) {
    PRINT_SAFE("Client connected on socket: " + std::to_string(clientSocket));

    // Initialize client graph and algorithm selection
    auto clientGraph = std::make_shared<Graph>(6);  // Default graph with 6 vertices
    MSTType selectedAlgorithm = MSTType::KRUSKAL;   // Default algorithm is Kruskal

    // Unique ActiveObject for each client
    ActiveObject graphModificationAO;
    graphModificationAO.start();

    while (isServerRunning) {
        char buffer[1024] = {0};
        int bytesRead = read(clientSocket, buffer, 1024);

        if (bytesRead <= 0) {
            PRINT_SAFE("Client disconnected or read error on socket: " + std::to_string(clientSocket));
            break;
        }

        std::string request(buffer);
        PRINT_SAFE("Received request: " + request);

        // Process request
        if (request.find("EXIT") != std::string::npos) {
            std::string response = "Connection closing...\n";
            send(clientSocket, response.c_str(), response.length(), 0);
            break;
        } 
        else if (request.find("EXIT_SERVER") != std::string::npos) {
            isServerRunning = false;
            PRINT_SAFE("Server shutdown command received.");
            std::string response = "Server is shutting down...\n";
            send(clientSocket, response.c_str(), response.length(), 0);
            break;
        }
        else if (request.find("ADD_EDGE") != std::string::npos || request.find("REMOVE_EDGE") != std::string::npos) {
            graphModificationAO.enqueue([clientSocket, request, clientGraph]() {
                handleGraphModification(clientSocket, request, clientGraph);
            });
        } 
        else if (request.find("CREATE_GRAPH") != std::string::npos) {
            graphModificationAO.enqueue([clientSocket, request, clientGraph]() {
                handleCreateGraph(clientSocket, request, clientGraph);
            });
        }
        else if (request.find("SELECT_ALGORITHM") != std::string::npos) {
            graphModificationAO.enqueue([clientSocket, request, &selectedAlgorithm]() {
                handleMSTAlgorithmSelection(clientSocket, request, selectedAlgorithm);
            });
        }
        else if (request.find("PRINT_GRAPH") != std::string::npos) {
            graphModificationAO.enqueue([clientSocket, clientGraph, selectedAlgorithm]() {
                handlePrintGraph(clientSocket, clientGraph, selectedAlgorithm);
            });
        }
        else {
            PRINT_SAFE("Unknown command: " + request);
        }
    }

    // Stop ActiveObject and close client socket
    graphModificationAO.stop();
    close(clientSocket);
    PRINT_SAFE("Client disconnected, socket closed: " + std::to_string(clientSocket));
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    PRINT_SAFE("Server is listening on port " + std::to_string(PORT));

    // Handle incoming client connections
    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        // Start a new thread for each client to allow concurrent handling
        std::thread clientThread(handleClient, new_socket);
        clientThread.detach();  // Detach so the main thread can continue accepting new connections
    }

    return 0;
}
