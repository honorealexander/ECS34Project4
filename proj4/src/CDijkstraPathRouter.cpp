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

    std::vector<std::vector<double>> DistanceMatrix;
    std::vector<std::vector<TVertexID>> NextVertexMatrix;
    std::vector<std::vector<TVertexID>> PredecessorMatrix;

};

// Constructor
CDijkstraPathRouter::CDijkstraPathRouter() : DImplementation(std::make_unique<SImplementation>()) {
    // Implementation of the constructor
    
}

// Destructor
CDijkstraPathRouter::~CDijkstraPathRouter() {
    // Perform cleanup or deallocate resources here
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
    // Add the edge from src to dest with the given weight

    // Check if src and dest vertices exist
    if (DImplementation->DAdjacencyList.find(src) == DImplementation->DAdjacencyList.end()) {
        AddVertex(std::any());  // Add src if not found
    }

    if (DImplementation->DAdjacencyList.find(dest) == DImplementation->DAdjacencyList.end()) {
        AddVertex(std::any());  // Add dest if not found
    }

    // Check if weight is non-negative
    if (weight < 0.0) {
        return false;
    }

    // Add edge from src to dest
    DImplementation->DAdjacencyList[src][dest] = weight;

    // If bidirectional, add edge from dest to src
    if (bidir) {
        DImplementation->DAdjacencyList[dest][src] = weight;
    }

    return true;
}


bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept {
    auto& adjacencyList = DImplementation->DAdjacencyList;
    auto& vertexCount = DImplementation->VertexCount;

    // Initialize the distance matrix with infinity
    std::vector<std::vector<double>> distanceMatrix(vertexCount, std::vector<double>(vertexCount, std::numeric_limits<double>::infinity()));

    // Initialize the next vertex matrix with invalid vertices
std::vector<std::vector<TVertexID>> predecessorMatrix(vertexCount, std::vector<TVertexID>(vertexCount, std::numeric_limits<TVertexID>::max()));

    // Set the initial distances and next vertices based on the adjacency list
    for (const auto& vertexPair : adjacencyList) {
        TVertexID srcVertex = vertexPair.first;
        for (const auto& neighbor : vertexPair.second) {
            TVertexID destVertex = neighbor.first;
            double weight = neighbor.second;
            distanceMatrix[srcVertex][destVertex] = weight;
            predecessorMatrix[srcVertex][destVertex] = destVertex;
        }
    }

    // Apply the Floyd-Warshall algorithm to compute shortest paths
    for (TVertexID k = 0; k < vertexCount; ++k) {
        for (TVertexID i = 0; i < vertexCount; ++i) {
            for (TVertexID j = 0; j < vertexCount; ++j) {
                if (distanceMatrix[i][k] + distanceMatrix[k][j] < distanceMatrix[i][j]) {
                    distanceMatrix[i][j] = distanceMatrix[i][k] + distanceMatrix[k][j];
                    predecessorMatrix[i][j] = k;  // Store the predecessor
                }
            }
        }
    }

    // Store the precomputed information in the implementation
    DImplementation->DistanceMatrix = std::move(distanceMatrix);
    DImplementation->NextVertexMatrix = std::move(predecessorMatrix);

    return true;
}

double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID>& path) noexcept {
    auto& adjacencyList = DImplementation->DAdjacencyList;

    // Priority queue to store vertices with their tentative distances
    std::priority_queue<std::pair<double, TVertexID>, std::vector<std::pair<double, TVertexID>>, std::greater<>> priorityQueue;

    // Map to store tentative distances
    std::unordered_map<TVertexID, double> tentativeDistances;

    // Map to store the previous vertex in the shortest path
    std::unordered_map<TVertexID, TVertexID> previousVertices;

    // Initialize distances and add the source vertex to the priority queue
    for (const auto& vertex : adjacencyList) {
        tentativeDistances[vertex.first] = (vertex.first == src) ? 0.0 : std::numeric_limits<double>::infinity();
        priorityQueue.emplace(tentativeDistances[vertex.first], vertex.first);
    }

    while (!priorityQueue.empty()) {
        auto currentVertex = priorityQueue.top().second;
        priorityQueue.pop();

        if (currentVertex == dest) {
            // Reconstruct the path if the destination is reached
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
