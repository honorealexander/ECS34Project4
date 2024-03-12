# CDjikstraPathRouter

The CDjikstraPathRouter class is derived from the CPathRouter class and is in charge of implementing all of the same function from CPathRouter but using Djikstra's Algorithm to find the shortest path within a graph that represents a geographical map. We want to be able to parse an OSM file and find the shortest fastest path from one point to another. We can also look at the differences between walking and taking the bus.

## Private Implementation

class CDijkstraPathRouter : public CPathRouter{
    private:
        struct SImplementation;
        std::unique_ptr<SImplementation> DImplementation;
    ...
}

The CDijkstraPathRouter has a private implementation for forward decleration which includes a SImplmentation struct and a pointer to it called DImplementation

## Constructor

CDijkstraPathRouter(); is our constructor. In our cpp file we have
//constructor
CDijkstraPathRouter::CDijkstraPathRouter() : DImplementation(std::make_unique<SImplementation>()) {
    //initialize object
}

This iniaializes a CDijkstraPathRouterObject. We do not have any more logic within our constructor because our logic comes from our member function that finds the shortest path

## Destructor

//destructor
CDijkstraPathRouter::~CDijkstraPathRouter() {}

This cleans up our memory once objects of the deriveed class are destroyed.

## Member Functions

std::size_t CDijkstraPathRouter::VertexCount() const noexcept {
    return DImplementation->VertexCount; //just a skeleton
}

Next we have our vertex count function. Similairly to CPathRouter class, this function returns the number of vertices in our graph. Here we access our DImplementation withing our CDijkstraPathRouter class to get how many vertices there are. std::size_t VertexCount = 0; is a member of our CDijkstraPathRouter::SImplementation and is updated as we add to our graph. Once we have out graph set we can access the VertexCount member variable to return the amount of vertices.

CPathRouter::TVertexID CDijkstraPathRouter::AddVertex(std::any tag) noexcept {
    //add vertex
    TVertexID NewID = DImplementation->DVertexTags.size();
    DImplementation->DVertexTags.emplace(NewID, tag);
    
    //update vertex count
    ++DImplementation->VertexCount;

    //return the newly created vertex ID
    return NewID;
}

Our AddVertex function creates a newTVertexID called NewId and takes in std::any representing the tag of the vertex.

TVertexID NewID = DImplementation->DVertexTags.size(); This line gets the size of the map representing the number of vertices already in the graph. DImplementation->DVertexTags.emplace(NewID, tag); this line then adds the new tag to the graph. 
Then we update the VertexCount since we added a vertex and we return the NewID that we added.

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
Our GetVertexTag(tag id) function searches for the vertexID withing our unordered map, if the id exists then we return teh assosciated tag and return std::any if no vertex was found given the ID
    
Our AddEdge function adds an edge between src and dest vertices with a weight of weight. if bidir is set to true an additional edge between dest and src is added. if src or dest nodes do not exist, or if the weight is negative the AddEdge will return false, otherwise it returns true.
We use this to check for bidir case and add the extra edge from dest to src:
    if (bidir) {
        DImplementation->DAdjacencyList[dest][src] = weight;
    }
We use this to add the original edge from src to dest:
DImplementation->DAdjacencyList[src][dest] = weight;

Our precompute function uses Floyd-Warshall algorithm to find all of the shortest paths between all nodes rather than the single shortest through Dijkstra. We intialized an infinity matrix and then implemented Floyd-Warshall with this
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
It returns true to indicate that the algorithm was successful.

Our final function is the most important part of our file which is double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID>& path) noexcept. This function implements Djikstra's algorithm to find the shortest path from a TVertexID src to a TVertexID dest.
First we create a priority ques to hold the vertices with their tentative distances, std::priority_queue<std::pair<double, TVertexID>, std::vector<std::pair<double, TVertexID>>, std::greater<>> priorityQueue;

Then we also create two maps to store distances and previous vertices. Once we have established the priority queue, we use a while loop to implement DJikstra's algotithm which runs until our priority queue's empty.
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
