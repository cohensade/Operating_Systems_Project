#ifndef MSTSTRATEGY_HPP
#define MSTSTRATEGY_HPP

#include "Graph.hpp"
#include "Tree.hpp"

// Abstract class for MST algorithms
class MSTStrategy {
public:
    virtual Tree buildTree(Graph& graph) = 0;
    virtual ~MSTStrategy() {}
};

#endif // MSTSTRATEGY_HPP
