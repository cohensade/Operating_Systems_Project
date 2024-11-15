#include "MSTStrategy.hpp"
#include "KruskalStrategy.cpp"
#include "PrimStrategy.cpp"

// Enumeration for the different MST algorithms
enum class MSTType {
    KRUSKAL,
    PRIM
};

// Factory class for creating MST strategies
class MSTFactory {
public:
    static MSTStrategy* createMSTStrategy(MSTType type) {
        switch (type) {
            case MSTType::KRUSKAL:
                return new KruskalStrategy();
            case MSTType::PRIM:
                return new PrimStrategy();
            default:
                return nullptr;
        }
    }
};

