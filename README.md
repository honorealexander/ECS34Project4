# ECS34Project4

## Olive Maunupau SSID: 918816205

## Honorè Alexander SSID: 

## Current Status

We are currently trying to fix our OSM file because we believe it is causing errors in our tranpsortationplannercommandline file

## Links
Used this to help understand Djikstra's:
https://brilliant.org/wiki/dijkstras-short-path-finder/

Used this to implement Floyd-Warshall algorithm to find all of the shortest paths between all nodes rather than the single shortest through Dijkstra:
https://www.geeksforgeeks.org/comparison-dijkstras-floyd-warshall-algorithms/#:~:text=Dijkstra's%20algorithm%20is%20a%20single%2Dsource%20shortest%20path%20algorithm%20used,vertices%20in%20a%20weighted%20graph


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
