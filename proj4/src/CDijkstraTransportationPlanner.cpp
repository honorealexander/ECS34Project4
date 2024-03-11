#include "DijkstraTransportationPlanner.h"
#include "DijkstraPathRouter.h"
#include "TransportationPlanner.h"
#include "TransportationPlannerConfig.h"
#include "GeographicUtils.h"
#include <unordered_map>
#include <queue>
#include "kmlout.cpp"
#include "OpenStreetMap.cpp"

extern std::unordered_map<CStreetMap::TNodeID, CStreetMap::TLocation> DNodeIDToLocation;

struct CDijkstraTransportationPlanner::SImplementation {
    std::shared_ptr<SConfiguration> DConfig;
    std::vector<std::vector<double>> DDistanceMatrix;
    std::vector<std::vector<TNodeID>> DNextNodeMatrix;
    std::unordered_map<TNodeID, std::vector<TNodeID>> NodeNeighbors;
};


//constructor
CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config)
    : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->DConfig = config;
}


//destructor
CDijkstraTransportationPlanner::~CDijkstraTransportationPlanner() {
    // Implementation of the destructor
}

std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept {
    // Implement your logic to return the number of nodes in the street map
    return COpenStreetMap::nodes.size();  // Assuming DNodes is a vector containing your street map nodes
}

std::shared_ptr<CStreetMap::SNode> CDijkstraTransportationPlanner::SortedNodeByIndex(std::size_t index) const noexcept {
    // Check if the index is within valid range
    if (index < NodeCount()) {
        // Sort the nodes by Node ID
        auto sortedNodes = DNodes;
        std::sort(sortedNodes.begin(), sortedNodes.end(), [](const auto &lhs, const auto &rhs) {
            return lhs->ID() < rhs->ID();
        });

        // Return the node at the specified index
        return sortedNodes[index];
    }

    // Index is out of range or equal to NodeCount(), return nullptr
    return nullptr;
}

double CDijkstraTransportationPlanner::FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID>& path) {
    // Initialize path and return value
    path.clear();
    double distance = DImplementation->DDistanceMatrix[src][dest];

    // If there is no direct path, return NoPathExists
    if (distance == std::numeric_limits<double>::infinity())
        return CPathRouter::NoPathExists;

    // Reconstruct the path
    path.push_back(src);
    while (src != dest) {
        src = DImplementation->DNextNodeMatrix[src][dest];
        path.push_back(src);
    }

    return distance;
}

// Returns the time in hours for the fastest path between the src and dest
// nodes of the if one exists. NoPathExists is returned if no path exists.
// The transportation mode and nodes of the fastest path are filled in the
// path parameter.
// double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector<TTripStep>& path) {
//     // Check if source and destination nodes are valid
//     if (src >= NodeCount() || dest >= NodeCount()) {
//         // Handle invalid nodes
//         return CPathRouter::NoPathExists;
//     }

//     // Initialize data structures for Dijkstra's algorithm
//     std::priority_queue<std::pair<double, TNodeID>, std::vector<std::pair<double, TNodeID>>, std::greater<>> priorityQueue;
//     std::unordered_map<TNodeID, double> tentativeTimes;
//     std::unordered_map<TNodeID, TNodeID> previousNodes;

//     // Initialize times and add the source node to the priority queue
//     for (std::size_t nodeIndex = 0; nodeIndex < NodeCount(); ++nodeIndex) {
//         tentativeTimes[nodeIndex] = (nodeIndex == src) ? 0.0 : std::numeric_limits<double>::infinity();
//         priorityQueue.emplace(tentativeTimes[nodeIndex], nodeIndex);
//     }

//     while (!priorityQueue.empty()) {
//         auto current = priorityQueue.top().second;
//         priorityQueue.pop();

//         if (current == dest) {
//             // Reconstruct the path if the destination is reached
//             while (current != src) {
//                 path.push_back({DImplementation->NodeInfo(current).TransportationMode, current});
//                 current = previousNodes[current];
//             }
//             path.push_back({DImplementation->NodeInfo(src).TransportationMode, src});
//             std::reverse(path.begin(), path.end());

//             return tentativeTimes[dest];
//         }

//         for (const auto& neighbor : DImplementation->NodeNeighbors[current]) {
//             double newTime = tentativeTimes[current] + CalculateTravelTime(current, neighbor);
//             if (newTime < tentativeTimes[neighbor]) {
//                 tentativeTimes[neighbor] = newTime;
//                 previousNodes[neighbor] = current;
//                 priorityQueue.emplace(newTime, neighbor);
//             }
//         }
//     }

//     // No path found
//     path.clear();
//     return CPathRouter::NoPathExists;
// }

// Returns true if the path description is created. Takes the trip steps path
// and converts it into a human readable set of steps.
bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector<TTripStep>& path, std::vector<std::string>& desc) const {
    desc.clear();

    if (path.empty()) {
        return false; // No path available
    }

    for (const auto& step : path) {
        std::string modeStr;
        switch (step.first) {
            case ETransportationMode::Walk:
                modeStr = "Walk";
                break;
            case ETransportationMode::Bike:
                modeStr = "Bike";
                break;
            case ETransportationMode::Bus:
                modeStr = "Bus";
                break;
        }

        std::string stepDesc = "Mode: " + modeStr + ", Node ID: " + std::to_string(step.second);
        desc.push_back(stepDesc);
    }

    return true;
}