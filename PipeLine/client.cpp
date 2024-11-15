#include <sys/socket.h>
#include <iostream>
#include <cstring>
int setup_client_connection(int server_fd) 
{
    // Accept a new client connection
    int client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0; // Indicate no client connected
        } else {
            std::cerr << "Failed to accept client: " << strerror(errno) << std::endl;
            return -1; // Return -1 for other errors
        }
    }
    return client_fd; // Return the client file descriptor if successful
}