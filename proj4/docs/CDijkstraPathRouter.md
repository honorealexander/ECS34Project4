# CDjikstraPathRouter

The CDjikstraPathRouter class is derived from the CPathRouter class and is in charge of implementing all of the same function from CPathRouter but using Djikstra's Algorithm to find the shortest path within a graph that represents a geographical map. We want to be able to parse an OSM file and find the shortest fastest path from one point to another. We can also look at the differences between walking and taking the bus.

## Private Implementation

class CDijkstraPathRouter : public CPathRouter{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
    ...
}



