#include "CDijkstraPathRouter.h"
#include <queue>
struct CDijkstraPathRouter::SImplementation {
    //add private implementation details as needed
};

CDijkstraPathRouter::CDijkstraPathRouter() : DImplementation(std::make_unique<SImplementation>()) {
    // Implementation of the constructor
}

CDijkstraPathRouter::~CDijkstraPathRouter() {
    // Implementation of the destructor
}

std::size_t CDijkstraPathRouter::VertexCount() const noexcept {
    return DImplementation->VertexCount;
}

TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept {
    //example implementation: Generate a new ID and store the tag
    TVertexID NewID = DImplementation->VertexTags.size() + 1;
    DImplementation->VertexTags[NewID] = tag;
    
    //update the vertex count
    ++DImplementation->VertexCount;

    //return the newly created vertex ID
    return NewID;
}

std::any CDijkstraPathRouter::GetVertexTag(TVertexID id) const noexcept {
    //example implementation: Check if the vertex ID exists in the map
    auto Iter = DImplementation->DVertexTags.find(id);
    if (Iter != DImplementation->DVertexTags.end()) {
        //return the associated tag if the vertex ID is found
        return Iter->second;
    } else {
        //return a default-constructed std::any if the vertex ID is not found
        return std::any();
    }
}

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
    // Implementation of Precompute
}

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

