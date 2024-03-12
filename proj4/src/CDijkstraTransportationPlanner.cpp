#include "DijkstraTransportationPlanner.h"
#include "DijkstraPathRouter.h"
#include "TransportationPlanner.h"
#include "TransportationPlannerConfig.h"
#include "StandardDataSource.h"
#include "StandardDataSink.h"
#include "StandardErrorDataSink.h"
// #include "GeographicUtils.cpp"
#include <unordered_map>
#include <queue>
// #include "kmlout.cpp"
#include "StreetMap.h"
#include "OpenStreetMap.cpp"
#include <limits>
#include <algorithm>

struct CDijkstraTransportationPlanner::SImplementation {
    std::shared_ptr<STransportationPlannerConfig> DConfig;
    std::shared_ptr<CStreetMap> DStreetMap;
    CDijkstraPathRouter DPathRouter;
    std::size_t NodeCount() const noexcept;
    std::shared_ptr<CStreetMap::SNode> SortedNodeByIndex(std::size_t index) const noexcept;
    double FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID>& path);
    double FindFastestPath(TNodeID src, TNodeID dest, std::vector<TTripStep>& path);
    bool GetPathDescription(const std::vector<TTripStep>& path, std::vector<std::string>& desc) const noexcept;
};

CDijkstraTransportationPlanner::CDijkstraTransportationPlanner(std::shared_ptr<SConfiguration> config)
    : DImplementation(std::make_unique<SImplementation>()) {
    DImplementation->DConfig = std::dynamic_pointer_cast<STransportationPlannerConfig>(config);
    DImplementation->DStreetMap = config->StreetMap();
}


CDijkstraTransportationPlanner::~CDijkstraTransportationPlanner() {

}

std::size_t CDijkstraTransportationPlanner::NodeCount() const noexcept {
    return DImplementation->DStreetMap->NodeCount();
}

std::shared_ptr<CStreetMap::SNode> CDijkstraTransportationPlanner::SortedNodeByIndex(std::size_t index) const noexcept {
    if (DImplementation->DStreetMap && index < DImplementation->DStreetMap->NodeCount()) {
        //retrieve all nodes
        std::vector<std::shared_ptr<CStreetMap::SNode>> nodes;

        for (std::size_t i = 0; i < DImplementation->DStreetMap->NodeCount(); ++i) { //iterate over all nodes
            auto node = DImplementation->DStreetMap->NodeByIndex(i);
            if (node) {
                nodes.push_back(node);
            }
        }

        std::sort(nodes.begin(), nodes.end(), [](const auto& a, const auto& b) { //sorts using bubble sort
            return a->ID() < b->ID();
        });
        return nodes[index]; //returns node at index
    } 

    return nullptr;
}

double CDijkstraTransportationPlanner::FindShortestPath(TNodeID src, TNodeID dest, std::vector<TNodeID>& path) {
    //checks src and dest valid
    if (src < 1 || src > DImplementation->DStreetMap->NodeCount() || dest < 1 || dest > DImplementation->DStreetMap->NodeCount()) {
        return CPathRouter::NoPathExists;
    }

    std::priority_queue<std::pair<double, TNodeID>, std::vector<std::pair<double, TNodeID>>, std::greater<>> pq;
    std::unordered_map<TNodeID, double> distance;
    std::unordered_map<TNodeID, TNodeID> predecessor;

    for (std::size_t i = 0; i < DImplementation->DStreetMap->NodeCount(); ++i) { //initialize to infinity
        auto node = DImplementation->DStreetMap->NodeByIndex(i);
        if (node) {
            distance[node->ID()] = std::numeric_limits<double>::infinity();
        }
    }
    distance[src] = 0; //no distance from source to itself

    pq.push({0.0, src}); //enqueue source node

    while (!pq.empty()) {
        auto [currentDistance, currentNode] = pq.top();
        pq.pop();

        if (currentNode == dest) { //stop when destination node reached
            path.clear(); //reconstruct path
            while (currentNode != src) {
                path.insert(path.begin(), currentNode);
                currentNode = predecessor[currentNode];
            }
            path.insert(path.begin(), src);
            return currentDistance;
        }

        //iterate through ways to find edges
        for (std::size_t wayIndex = 0; wayIndex < DImplementation->DStreetMap->WayCount(); ++wayIndex) {
            auto way = DImplementation->DStreetMap->WayByIndex(wayIndex);
            if (way) {
                for (std::size_t nodeIndex = 0; nodeIndex < way->NodeCount() - 1; ++nodeIndex) {
                    TNodeID startNode = way->GetNodeID(nodeIndex);
                    TNodeID endNode = way->GetNodeID(nodeIndex + 1);

                    double edgeWeight = 1.0; //treat all edges as equal

                    //check neighbor node is valid
                    if (distance.find(endNode) == distance.end()) {
                        continue;  // Skip invalid nodes
                    }

                    if (distance[startNode] + edgeWeight < distance[endNode]) {
                        distance[endNode] = distance[startNode] + edgeWeight;
                        predecessor[endNode] = startNode;
                        pq.push({distance[endNode], endNode});
                    }
                }
            }
        }
    }
    return CPathRouter::NoPathExists; //no path
}

double CDijkstraTransportationPlanner::FindFastestPath(TNodeID src, TNodeID dest, std::vector<TTripStep>& path) {

    return 0.0;
}

bool CDijkstraTransportationPlanner::GetPathDescription(const std::vector<TTripStep>& path, std::vector<std::string>& desc) const {

    return true;
}