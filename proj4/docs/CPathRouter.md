
# CPathRouter

The CPathRouter class helps us implement a graph representing bus routes and street maps as vertices and nodes. We can then use this representation to try to find the shortest path from one node to another or one street to another. We use TVertexID to represent std::size_t in this class. All members are public.

We have two constants in this class, InvalidVertexID and NoPathExists. The former is used when we have an invalid vertex ID or if it doesn't exist. The latter also is used to say that a path between two vertices doesn't exist. It is set to the maximum of 'double' to signify that no path exists. InvalidVertex is also set to the maximum to signify its absence. All of these functions are purely virtual.

## Destructor

~CPathRouter(){}; Is the destructor and is used to do cleanup when objects of the derived classes are destroyed.

## Member Functions

std::size_t VertexCount() const noexcept

This function simply returns the number of vertices within a graph. In the case of our BusSystem, it would represent StopIDs and for OpenStreetMap, it could represent intersections. The edges between these stops represent the path, route or street.

## Generating Graph

TVertexID AddVertex(std::any tag) noexcept

This function takes in any parameter and adds it as a vertex within the graph. It returns the ID of the newly added vertex. 

bool AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir = false) noexcept

This function generates the edge between two TVertexID's, src adn dest. It take's in the weight og the edge and bool bidir = false represnts bi-directional, meaning whether it goes both directions or not. It returns true if it succesfully adds the edge and false otherwise.

## Acessing Graph

std::any GetVertexTag(TVertexID id) const noexcept

This function is like the opposite of add Vertex. Instead of giving it a vertex tag and generating an ID for it, you give this function the vertex ID and it returns the Vertex tag.

bool Precompute(std::chrono::steady_clock::time_point deadline) noexcept;

This precompute function precomputes a shortest path prior to any actual calculation done by us. This function is supposed to optimize the shortest path. The function's input parametr is deadline, which represents a time deadline for its precomputation to be copmlete so that we have a stop deadline and that our function isn't running forever.
## Finding Shortest Path

double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept

The whole goal of this class is to find the shortest path from A to B within our graph. In this case A and B are TVertexID src and TVertexID dest. std::vector<TVertexID> &path is the corrresponding vector that holds a sequence of TVertexIDs representing the shortest path between src and dest. This function returns a double which represents the total weight of the shortest path between src and dest. There should exist no other path between these two vectors with a lower weight than what is outputted by this function. In this class we do not implement any particular algorithm for shortest path but this is used to derive DjikstraPathRouter in which we implement Djikstra's algorithm within this function to find the shortest path.