# CDjikstraTransportationPlanner

# CDjikstraTransportationPlanner

The CDijkstraTransportationPlanner has a private struct SImplementation with a unique ptr to it called DImplementation

class CDijkstraTransportationPlanner : public CTransportationPlanner{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
}

Publicly it has many functions to help find shortest and fastest path between nodes in a transportation network using Djikstra's algorithm 

## SImplementation

In our struct implementation we establish member variables:
std::shared_ptr<SConfiguration> DConfig;
- provides access to important information like street map and bussystem information, bustimes, speeds for walking and biking and precompute times.
std::vector<std::vector<double>> DDistanceMatrix;
- this is a 2d distance matrix that stores the pairwise distance between each node within the transportation graph
std::vector<std::vector<TNodeID>> DNextNodeMatrix;
- this 2d matrix similair to the distance matrix but instead of holding the pairwise distance it holds information about the next node to visit in the shortest/fastest path from node i to node j
std::unordered_map<TNodeID, std::vector<TNodeID>> NodeNeighbors;
- this member variable is an unordered map that stores the neighbors of each node within the transportation graph.


## Constructor an Destructor
CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config)
    : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->DConfig = config;
}
The constructor initializes the Dimplementation object. After creating the object the constructor assigns DCongif of DImplmentation to config which helps the class perform pathfinding. It covers encapsulation parameters that help with transportation planning.

~CDijkstraTransportationPlanner();
Our destructor is in charge of cleanup and getting rid of allocated memory that is no longer being used once the instance of CDjikstraTransportationPlanner is out of the scope

## Member Functions
std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept {

Node count returns how many nodes