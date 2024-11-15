# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11 -pthread

# Targets
SERVER_TARGET = server
CLIENT_OBJECT = client.o  # נבנה רק את client.o

# Source files for server
SERVER_SRCS = server.cpp Graph.cpp MSTFactory.cpp KruskalStrategy.cpp PrimStrategy.cpp Tree.cpp ThreadPool.cpp

# Source files for client
CLIENT_SRCS = client.cpp

# Object files for server and client
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

# Default target: build the server and client object file
all: $(SERVER_TARGET) $(CLIENT_OBJS)
	@echo "Server and client object file built successfully."

# Build the server executable
$(SERVER_TARGET): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $(SERVER_TARGET) $(SERVER_OBJS)
	@echo "Server built successfully."

# Build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean the build
clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_TARGET)
	@echo "Clean complete."

# Run the server
run_server: $(SERVER_TARGET)
	./$(SERVER_TARGET)
