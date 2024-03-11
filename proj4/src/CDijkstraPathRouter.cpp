#include "DijkstraPathRouter.h"
#include "PathRouter.h"
#include <queue>
#include <memory>
#include <any>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

struct CDijkstraPathRouter::SImplementation {
    std::unordered_map<TVertexID, std::unordered_map<TVertexID, double>> DAdjacencyList;
    std::unordered_map<TVertexID, std::any> DVertexTags;
    std::size_t VertexCount = 0;
};

CDijkstraPathRouter::CDijkstraPathRouter() : DImplementation(std::make_unique<SImplementation>()) {
    // Implementation of the constructor
    
}

CDijkstraPathRouter::~CDijkstraPathRouter() {
    // Implementation of the destructor
}

std::size_t CDijkstraPathRouter::VertexCount() const noexcept {
    return DImplementation->VertexCount; //just a skeleton
}

CPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept {
    //add vertex
    TVertexID NewID = DImplementation->DVertexTags.size();
    DImplementation->DVertexTags.emplace(NewID, tag);
    
    //update vertex count
    ++DImplementation->VertexCount;

    //return the newly created vertex ID
    return NewID;
}

std::any CDijkstraPathRouter::GetVertexTag(TVertexID id) const noexcept {
    //example implementation: Check if the vertex ID exists in the map
    auto Iteration = DImplementation->DVertexTags.find(id);
    if (Iteration != DImplementation->DVertexTags.end()) {
        //return the associated tag if the vertex ID is found
        return Iteration->second;
    } else {
        //return a default-constructed std::any if the vertex ID is not found
        return std::any();
    }
}

/* adds an edge between src and dest vertices with a weight of weight. if
 bidir is set to true an additional edge between dest and src is added. if
 src or dest nodes do not exist, or if the weight is negative the AddEdge
 will return false, otherwise it returns true. */
bool CDijkstraPathRouter::AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir) noexcept {
    //example implementation: Add the edge from src to dest with the given weight

    //check if src and dest vertices exist
    if (DImplementation->DAdjacencyList.find(src) == DImplementation->DAdjacencyList.end() ||
        DImplementation->DAdjacencyList.find(dest) == DImplementation->DAdjacencyList.end()) {
        return false; //one or both vertices not found
    }

    //add edge from src to dest
    DImplementation->DAdjacencyList[src][dest] = weight;

    //if bidirectional, add edge from dest to src
    if (bidir) {
        DImplementation->DAdjacencyList[dest][src] = weight;
    }

    return true;
}

bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept {
    // need to do: implementation of Precompute
    return true;
};

double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID>& path) noexcept {
    
    
    auto& adjacencyList = DImplementation->DAdjacencyList;
    //priority queue to store vertices with their tentative distances
    std::priority_queue<std::pair<double, TVertexID>, std::vector<std::pair<double, TVertexID>>, std::greater<>> priorityQueue;

    //map to store tentative distances
    std::unordered_map<TVertexID, double> tentativeDistances;

    //map to store the previous vertex in the shortest path
    std::unordered_map<TVertexID, TVertexID> previousVertices;

    //initialize distances and add the source vertex to the priority queue
    for (const auto& vertex : adjacencyList) {
        tentativeDistances[vertex.first] = (vertex.first == src) ? 0.0 : std::numeric_limits<double>::infinity();
        priorityQueue.emplace(tentativeDistances[vertex.first], vertex.first);
    }

    while (!priorityQueue.empty()) {
        auto currentVertex = priorityQueue.top().second;
        priorityQueue.pop();

        if (currentVertex == dest) {
            //reconstruct the path if the destination is reached
            while (currentVertex != src) {
                path.push_back(currentVertex);
                currentVertex = previousVertices[currentVertex];
            }
            path.push_back(src);
            std::reverse(path.begin(), path.end());

            return tentativeDistances[dest];
        }

        for (const auto& neighbor : adjacencyList[currentVertex]) {
            double newDistance = tentativeDistances[currentVertex] + neighbor.second;
            if (newDistance < tentativeDistances[neighbor.first]) {
                tentativeDistances[neighbor.first] = newDistance;
                previousVertices[neighbor.first] = currentVertex;
                priorityQueue.emplace(newDistance, neighbor.first);
            }
        }
    }

    // No path found
    path.clear();
    return std::numeric_limits<double>::infinity();
}

