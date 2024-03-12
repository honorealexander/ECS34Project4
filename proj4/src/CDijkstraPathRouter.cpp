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

//constructor
CDijkstraPathRouter::CDijkstraPathRouter() : DImplementation(std::make_unique<SImplementation>()) {
    //initialize object
}

//destructor
CDijkstraPathRouter::~CDijkstraPathRouter() {}


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
    if (DImplementation->DAdjacencyList.find(src) == DImplementation->DAdjacencyList.end()) {
        AddVertex(std::any()); //checks if source vertex exists and if it doesn't then adds it
    }
    if (DImplementation->DAdjacencyList.find(dest) == DImplementation->DAdjacencyList.end()) {
        AddVertex(std::any()); //same for destination vertex
    }
    if (weight < 0.0) { //makes sure weight of edge is non-negative
        return false;
    }

    DImplementation->DAdjacencyList[src][dest] = weight; //add weight to adjacency list
    if (bidir) {
        DImplementation->DAdjacencyList[dest][src] = weight;
    }

    return true;
}


bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept {
    auto& adjacencyList = DImplementation->DAdjacencyList;
    auto& vertexCount = DImplementation->VertexCount;

    //initialize the distance matrix with infinity
    std::vector<std::vector<double>> distanceMatrix(vertexCount, std::vector<double>(vertexCount, std::numeric_limits<double>::infinity())); //got these lines from chat gpt

    //initialize the next vertex matrix with invalid vertices
    std::vector<std::vector<TVertexID>> predecessorMatrix(vertexCount, std::vector<TVertexID>(vertexCount, std::numeric_limits<TVertexID>::max())); //chat gpt

    //sets distances and vertices based on list
    for (const auto& vertexPair : adjacencyList) {
        TVertexID srcVertex = vertexPair.first;
        for (const auto& neighbor : vertexPair.second) {
            TVertexID destVertex = neighbor.first;
            double weight = neighbor.second;
            distanceMatrix[srcVertex][destVertex] = weight;
            predecessorMatrix[srcVertex][destVertex] = destVertex;
        }
    }

    //floyd-warshall algorithm
    for (TVertexID k = 0; k < vertexCount; ++k) {
        for (TVertexID i = 0; i < vertexCount; ++i) {
            for (TVertexID j = 0; j < vertexCount; ++j) {
                if (distanceMatrix[i][k] + distanceMatrix[k][j] < distanceMatrix[i][j]) {
                    distanceMatrix[i][j] = distanceMatrix[i][k] + distanceMatrix[k][j];
                    predecessorMatrix[i][j] = k;  //stores the predecessor
                }
            }
        }
    }

    //copies these matrices into our DImplementation matrices
    DImplementation->DistanceMatrix = std::move(distanceMatrix);
    DImplementation->NextVertexMatrix = std::move(predecessorMatrix);

    return true;
}

double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID>& path) noexcept {
    auto& adjacencyList = DImplementation->DAdjacencyList;
    //priority queue used for Dijkstras
    std::priority_queue<std::pair<double, TVertexID>, std::vector<std::pair<double, TVertexID>>, std::greater<>> priorityQueue;

    std::unordered_map<TVertexID, double> tentativeDistances; //store tentative distances

    std::unordered_map<TVertexID, TVertexID> previousVertices; //store previous vertices

    // Initialize distances and add the source vertex to the priority queue
    for (const auto& vertex : adjacencyList) {
        tentativeDistances[vertex.first] = (vertex.first == src) ? 0.0 : std::numeric_limits<double>::infinity(); //chat
        priorityQueue.emplace(tentativeDistances[vertex.first], vertex.first);
    }

    while (!priorityQueue.empty()) { //while we still have nodes left
        auto currentVertex = priorityQueue.top().second;
        priorityQueue.pop();

        if (currentVertex == dest) { //reconstruct if we find our destintation vertex
            while (currentVertex != src) {
                path.push_back(currentVertex);
                currentVertex = previousVertices[currentVertex]; //move to previous vertex
            }
            path.push_back(src);
            std::reverse(path.begin(), path.end()); //we have to reverse to get the right order cause we added it in the previous order way

            return tentativeDistances[dest];
        }

        for (const auto& neighbor : adjacencyList[currentVertex]) { //for loop to iterate through current vertex and see if the new distance we caluclate is shorter than any other
            double newDistance = tentativeDistances[currentVertex] + neighbor.second;
            if (newDistance < tentativeDistances[neighbor.first]) { //if it's not shorter we dont need to update because we already have a distance shorter
                tentativeDistances[neighbor.first] = newDistance;
                previousVertices[neighbor.first] = currentVertex;
                priorityQueue.emplace(newDistance, neighbor.first);
            }
        }
    }

    //no path found
    path.clear();
    return std::numeric_limits<double>::infinity();
}
