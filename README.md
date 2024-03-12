# ECS34Project4

## Olive Maunupau SSID: 918816205

## Honorè Alexander SSID: 921129112

## Current Status

We are currently trying to fix our OSM file because we believe it is causing errors in our tranpsortationplannercommandline file

## Links
Used this to help understand Djikstra's:
https://brilliant.org/wiki/dijkstras-short-path-finder/

Used this to implement Floyd-Warshall algorithm to find all of the shortest paths between all nodes rather than the single shortest through Dijkstra:
https://www.geeksforgeeks.org/comparison-dijkstras-floyd-warshall-algorithms/#:~:text=Dijkstra's%20algorithm%20is%20a%20single%2Dsource%20shortest%20path%20algorithm%20used,vertices%20in%20a%20weighted%20graph

for help with std:any
https://en.cppreference.com/w/cpp/utility/any

for help with std::move
https://en.cppreference.com/w/cpp/utility/move

for help with Dijkstras
https://en.cppreference.com/w/cpp/types/numeric_limits/infinity

for std::reverse
https://en.cppreference.com/w/cpp/algorithm/reverse

for help with sort in CSVBusSystemIndexer
https://stackoverflow.com/questions/37557430/converting-const-auto-to-iterator

## Generative AI

### Prompt 1
Is there a reason my constructor is running infinitely?
while (src->ReadEntity(entity, false)) {
        if (entity.DType == SXMLEntity::EType::StartElement) {
            if (entity.DNameData == "node") {
                // parse node attributes and create streetmap object
                auto newNode = std::make_shared<streetmap>();
                // set attributes for the new node
                auto nodeIDAttribute = std::find_if(entity.DAttributes.begin(), entity.DAttributes.end(),
                                    [](const auto& attribute) {
                                        return attribute.first == "id";
                                    });
                if (nodeIDAttribute != entity.DAttributes.end()) {
                    newNode->SetAttribute("id", nodeIDAttribute->second);
                }
                // add the new node to the nodes map
                DImplementation->nodes[newNode->ID()] = newNode;
            } else if (entity.DNameData == "way") {
                // parse way attributes and create newstreetmap object
                auto newWay = std::make_shared<newstreetmap>();
                // set attributes for the new way
                auto wayIDAttribute = std::find_if(entity.DAttributes.begin(), entity.DAttributes.end(),
                                        [](const auto& attribute) {
                                            return attribute.first == "id";
                                        });
                if (wayIDAttribute != entity.DAttributes.end()) {
                    newWay->SetAttribute("id", wayIDAttribute->second);
                }
                // add the new way to the ways map
                DImplementation->ways[newWay->ID()] = newWay;
            }
        }
    }

### Response 1
An infinite loop in this context usually indicates that the XML parsing process never completes or encounters an error, causing the parser to keep waiting for more input. One potential reason for this behavior is that the condition in the while loop used for reading XML entities (src->ReadEntity(entity, false)) might never evaluate to false, causing the loop to continue indefinitely.

To debug this issue, you should check if the ReadEntity function is properly detecting the end of the XML input stream and returning false when appropriate. Here are a few steps you can take to diagnose and fix the problem:

Check the ReadEntity function: Review the implementation of the ReadEntity function in the CXMLReader class (or wherever it's defined). Ensure that it correctly handles the end of the input stream and returns false when there's no more data to read.

Verify the XML input: Ensure that the XML file you're trying to parse is well-formed and complete. Sometimes, missing or malformed XML tags can confuse the parser and prevent it from recognizing the end of the document.

Check for error conditions: Look for error conditions or exceptions that might occur during XML parsing. If an error occurs but isn't properly handled, it could cause the parsing process to hang indefinitely.

Add debug output: Insert debug output statements inside the while loop to print information about each XML entity as it's being read. This can help you identify where the parsing process is getting stuck or if there's a specific entity causing the issue.

Test with a smaller input: If possible, try parsing a smaller or simplified version of the XML input to see if the problem persists. This can help isolate the issue and identify any specific elements or attributes causing the infinite loop.

By following these steps and carefully reviewing the XML parsing code, you should be able to identify and resolve the issue causing the infinite loop.

### Changes 1
I then looked at my ReadEntity to see if there was a reason my infinite loop was coming from there. I couldn't figure it out so I asked chat again if it noticed any errors with my function

### Prompt 2
    bool ReadEntity(SXMLEntity &entity, bool skipcdata){
        while(DEntityQueue.empty()){
            if(DDataSource->Read(DataBuffer, 256)){
                XML_Parse(DXMLParser, DataBuffer.data(), DataBuffer.size(), DataBuffer.size() < 256);
            } else {
                XML_Parse(DXMLParser, DataBuffer.data(), 0, true);
            }
        }
        if(DEntityQueue.empty()){
            return false;
        }
        entity = DEntityQueue.front();
        DEntityQueue.pop();
        return true;
    };

### Response 2
Thank you for sharing the ReadEntity function. Based on the provided code, the function reads XML entities from a queue (DEntityQueue) filled by the XML_Parse function. However, it seems that there isn't any condition in the while loop that explicitly checks for the end of the XML input stream or signals when the parsing process is complete.

To address the issue of the infinite loop, you need to ensure that the ReadEntity function correctly handles the end of the XML input stream and returns false when there are no more entities to read. Here's a revised version of the function with additional checks:

cpp
Copy code
bool ReadEntity(SXMLEntity &entity, bool skipcdata) {
    while (DEntityQueue.empty()) {
        if (DDataSource->Read(DataBuffer, 256)) {
            if (!XML_Parse(DXMLParser, DataBuffer.data(), DataBuffer.size(), false)) {
                // Parsing error occurred
                return false;
            }
        } else {
            if (!XML_Parse(DXMLParser, DataBuffer.data(), 0, true)) {
                // Parsing error occurred
                return false;
            }
        }
    }
    
    if (DEntityQueue.empty()) {
        // No more entities to read
        return false;
    }
    
    entity = DEntityQueue.front();
    DEntityQueue.pop();
    return true;
}
In this revised version:

After each call to XML_Parse, it's essential to check the return value to detect any parsing errors. If XML_Parse returns false, it indicates a parsing error occurred, and the function should return false.
The while loop continues to read entities until the queue is empty or a parsing error occurs.
If the queue is empty after the loop, it means there are no more entities to read, and the function returns false.
By ensuring that the ReadEntity function handles parsing errors and correctly detects the end of the XML input stream, you should be able to prevent the infinite loop and ensure the XML parsing process terminates properly.

### Changes 2
I made the changes ChatGPT recomended to me by changing this line:
XML_Parse(DXMLParser, DataBuffer.data(), DataBuffer.size(), DataBuffer.size() < 256);

to this line:
if (!XML_Parse(DXMLParser, DataBuffer.data(), DataBuffer.size(), false)) {
                    return false;
                }

And this line:
XML_Parse(DXMLParser, DataBuffer.data(), 0, true);

To this line:
                if (!XML_Parse(DXMLParser, DataBuffer.data(), 0, true)) {
                    return false;
                }


### Prompt 3
Can you show me how I may implement an adjacency list with an unordered map in c++?

### Response 3
Certainly! Here's how you can implement an adjacency list using std::unordered_map where each edge has an associated weight:

cpp
Copy code
#include <iostream>
#include <unordered_map>

// Define the vertex ID type
using TVertexID = int;

// Define the adjacency list type
using AdjacencyList = std::unordered_map<TVertexID, std::unordered_map<TVertexID, double>>;

// Function to add an edge to the adjacency list
void addEdge(AdjacencyList& adjacencyList, TVertexID src, TVertexID dest, double weight) {
    // Add the destination vertex with its weight to the adjacency list of the source vertex
    adjacencyList[src][dest] = weight;
    
    // If it's an undirected graph, add the reverse edge
    // adjacencyList[dest][src] = weight; // Uncomment this line for undirected graph
}

int main() {
    // Create an empty adjacency list
    AdjacencyList adjacencyList;

    // Add edges to the graph
    addEdge(adjacencyList, 0, 1, 3.5);  // Edge from vertex 0 to vertex 1 with weight 3.5
    addEdge(adjacencyList, 0, 2, 2.0);  // Edge from vertex 0 to vertex 2 with weight 2.0
    addEdge(adjacencyList, 1, 2, 1.5);  // Edge from vertex 1 to vertex 2 with weight 1.5

    // Accessing the adjacency list
    for (const auto& pair : adjacencyList) {
        TVertexID vertex = pair.first;
        const auto& neighbors = pair.second;
        std::cout << "Vertex " << vertex << " is adjacent to:";
        for (const auto& neighbor : neighbors) {
            TVertexID neighborVertex = neighbor.first;
            double weight = neighbor.second;
            std::cout << " (vertex " << neighborVertex << ", weight " << weight << ")";
        }
        std::cout << std::endl;
    }

    return 0;
}
This example demonstrates how to create an adjacency list for an undirected graph where each edge has an associated weight. Uncommenting the line inside the addEdge function allows you to create a directed graph by adding only the forward edges.

### Changes 3
Used this info to create my DAdjacencyList in CDjikstraPathRouter
std::unordered_map<TVertexID, std::unordered_map<TVertexID, double>> DAdjacencyList;

### Prompt 4
bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept {
    auto& adjacencyList = DImplementation->DAdjacencyList;
    auto& vertexCount = DImplementation->VertexCount;
}

How can i intialize my distance in predecessor matrix here? 

### Response 4
To initialize the distance and predecessor matrices (distanceMatrix and predecessorMatrix), you can use a similar approach as initializing the adjacency list. You can loop through each vertex and set the initial values accordingly.

Here's how you can initialize both matrices:

cpp
Copy code
bool CDijkstraPathRouter::Precompute(std::chrono::steady_clock::time_point deadline) noexcept {
    auto& adjacencyList = DImplementation->DAdjacencyList;
    auto& vertexCount = DImplementation->VertexCount;

    // Initialize distance matrix and predecessor matrix
        std::vector<std::vector<double>> distanceMatrix(vertexCount, std::vector<double>(vertexCount, std::numeric_limits<double>::infinity()));    
    std::vector<std::vector<TVertexID>> predecessorMatrix(vertexCount, std::vector<TVertexID>(vertexCount, std::numeric_limits<TVertexID>::max()));

    // Now you can use these matrices for further computation

    return true;  // Or return false if an error occurred
}

### Changes 4
I initialized my matrices using this way and cited them in the comments.

### Prompt 5
double CDijkstraPathRouter::FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID>& path) noexcept {
    auto& adjacencyList = DImplementation->DAdjacencyList;
    //priority queue used for Dijkstras
    std::priority_queue<std::pair<double, TVertexID>, std::vector<std::pair<double, TVertexID>>, std::greater<>> priorityQueue;

    std::unordered_map<TVertexID, double> tentativeDistances; //store tentative distances

    std::unordered_map<TVertexID, TVertexID> previousVertices; //store previous vertices
How can I innitialize the tentative distance of each vertex in the graph?

### Response 5
To initialize the tentative distance of each vertex in the graph, you can use a map or an array to store these distances. Here's how you can do it:

Using a std::unordered_map:

cpp
Copy code
#include <unordered_map>
#include <limits>

// Assuming TVertexID is the type used to represent vertex IDs
using TVertexID = /* specify your vertex ID type here */;

// Assuming your graph is represented by an adjacency list stored in DAdjacencyList
std::unordered_map<TVertexID, double> tentativeDistances;

// Initialize tentative distances for each vertex in the graph
for (const auto& vertex : DAdjacencyList) {
    tentativeDistances[vertex.first] = (vertex.first == src) ? 0.0 : std::numeric_limits<double>::infinity();
}

### Changes 5
I used this for loop within my adjacency list to initialize my graph, then afterwards I am able to add the vertex and ID pair to my priority queue to use for DJikstra's.

### Prompt 6
User

std::size_t CBusSystemIndexer::StopCount() const noexcept {
    return DImplementation->BusSystem->StopCount();
}

I'm getting errors when I do this, why doesn't it work?

### Response 6
If you're encountering errors when calling DImplementation->BusSystem->StopCount(), it's likely because DImplementation->BusSystem is either not initialized or it's null when you're trying to access it.

Here are a few things you can check:

Initialization: Ensure that DImplementation->BusSystem is properly initialized before calling StopCount(). If DImplementation->BusSystem is a raw pointer, make sure it points to a valid object. If it's a smart pointer like std::shared_ptr or std::unique_ptr, make sure it's initialized with a valid object.

Null Pointer Check: If DImplementation->BusSystem can be null, you should check for null before dereferencing it to avoid runtime errors. You can use an if statement to check if it's null before accessing StopCount().

Here's an example of how you can add a null pointer check:

cpp
Copy code
std::size_t CBusSystemIndexer::StopCount() const noexcept {
    return (DImplementation->BusSystem) ? DImplementation->BusSystem->StopCount() : 0;
}
By adding this null pointer check, you can avoid dereferencing a null pointer, which should help resolve the errors you're encountering.

### Changes 6
I changed both my StopCount() and RouteCount() to adding a nullpointer check using ternary operator.

I went from 
std::size_t CBusSystemIndexer::StopCount() const noexcept {
    return DImplementation->BusSystem->StopCount();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept {
    return DImplementation->BusSystem->RouteCount();
}

To:
std::size_t CBusSystemIndexer::StopCount() const noexcept {
    return (DImplementation->BusSystem) ? DImplementation->BusSystem->StopCount() : 0;
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept {
    return (DImplementation->BusSystem) ? DImplementation->BusSystem->RouteCount() : 0;
}

