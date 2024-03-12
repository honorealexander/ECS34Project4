# CTransportationPlanner

Our CTransportationPlanner class is in charge of planning routes of transportation from a streetmap and differentiating paths between walking, biking and bussing. It provides deferent functions for finding shortest and fastest paths between two nodes and providing a path description for the user. It uses TTripStep as std::pair<ETransportationMode, TNodeID> and ETransportationMode to enumerate a type differentiatinng Walk, Bike and Bus.

## Struct SConfiguration

Nested within the CTransportationPlanner is the SCongfiguration struct which is responsible for holding parameters for the transportation planner.

It has these member functions:
~SConfiguration(){};
- destructor in charge of cleanup and deallocating unused resources
std::shared_ptr<CStreetMap> StreetMap() const noexcept = 0;
- returns a sharedpointer to the streetmap
std::shared_ptr<CBusSystem> BusSystem() const noexcept = 0;
- returns a shared pointer to the bussystem
double WalkSpeed() const noexcept = 0;
- function that returns a double representing the walking speed
double BikeSpeed() const noexcept = 0;
- function that returns a double representing the bikespeed
double DefaultSpeedLimit() const noexcept = 0;
- function returning a double that represents the default speed limit which is the automatic speed limit for a street given there are no further restrictions or reasons to go above or below it. For example construction or on freeways when you can go with the flow of traffic.
double BusStopTime() const noexcept = 0;
- This function returns a double representing the anount of time spent at each busstop
int PrecomputeTime() const noexcept = 0;
- This function returns a double that represents the time taken through the precomputed routes.

## Destructor
~CTransportationPlanner(){};
This destructor is in charge of performing cleanup tasks assosciated with instances of CTransportationPlanners

## Member Functions
std::size_t NodeCount() const noexcept = 0;
- this returns the number of nodes in the map, can be adopted by many different classes
std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept = 0;
- takes a parameter called index and returns the node at given index in the sorted order
double FindShortestPath(TNodeID src, TNodeID dest, std::vector< TNodeID > &path) = 0;
- this fubction takes a src node and dest node as well as the path between the two nodes, and returns the shortest path from source to destination.
double FindFastestPath(TNodeID src, TNodeID dest, std::vector< TTripStep > &path) = 0;
- similair to FindShortestPath, FindFastestPath takes in the same source and dest parameters but also takes in each transportation trip step returns the fastest path rather than the shortest.
bool GetPathDescription(const std::vector< TTripStep > &path, std::vector< std::string > &desc) const = 0;
- This function takes in a TTripStep vector called path and returns information about the path such as the transportation trip step
