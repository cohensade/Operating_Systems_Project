#ifndef EDGE_HPP
#define EDGE_HPP

struct Edge {
    int index;
    int src;
    int dest;
    int weight;

    Edge(int i, int s, int d, int w) : index(i), src(s), dest(d), weight(w) {}
};

#endif // EDGE_HPP
