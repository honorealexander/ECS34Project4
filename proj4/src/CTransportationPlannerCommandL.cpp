#include "TransportationPlannerCommandLine.h"
#include "TransportationPlannerConfig.h"
#include "TransportationPlanner.h"
#include "StringDataSource.h"
#include "StringDataSink.h"
#include "DSVReader.h"
#include "XMLReader.h"
#include "OpenStreetMap.h"
#include "CSVBusSystem.h"
#include "FileDataSink.h"
#include <iterator>
#include <sstream>
#include <cstring>

// CTransportationPlannerCommandLine member functions
// Constructor for the Transportation Planner Command Line
struct CTransportationPlannerCommandLine::SImplementation {
    std::shared_ptr<CDataSource> CommandSource;
    std::shared_ptr<CDataSink> OutputSink;
    std::shared_ptr<CDataSink> ErrorSink;
    std::shared_ptr<CDataFactory> Results;
    std::shared_ptr<CTransportationPlanner> Planner;

    // Constructor
    SImplementation(std::shared_ptr<CDataSource> cmdsrc,
                    std::shared_ptr<CDataSink> outsink,
                    std::shared_ptr<CDataSink> errsink,
                    std::shared_ptr<CDataFactory> results,
                    std::shared_ptr<CTransportationPlanner> planner)
        : CommandSource(cmdsrc),
          OutputSink(outsink),
          ErrorSink(errsink),
          Results(results),
          Planner(planner) {}

    // add any other necessary member functions here
};

CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(
    std::shared_ptr<CDataSource> cmdsrc,
    std::shared_ptr<CDataSink> outsink,
    std::shared_ptr<CDataSink> errsink,
    std::shared_ptr<CDataFactory> results,
    std::shared_ptr<CTransportationPlanner> planner)
    : DImplementation(std::make_unique<SImplementation>(cmdsrc, outsink, errsink, results, planner)) {}

// Destructor for the Transportation Planner Command Line
CTransportationPlannerCommandLine::~CTransportationPlannerCommandLine() = default;

// Processes the commands input to the
bool CTransportationPlannerCommandLine::ProcessCommands() {
    DImplementation->OutputSink->Write(std::vector<char>());

    std::string command;
    std::vector<std::string> tokens;

    // Get the command from CommandSource
    std::vector<char> commandBuffer;
    CStringDataSource* stringDataSource = dynamic_cast<CStringDataSource*>(DImplementation->CommandSource.get());
    if (stringDataSource) {
        // Get the size of the string data
        std::size_t bufferSize = stringDataSource->DString.size();
        // Resize the buffer to match the size of the string data
        commandBuffer.resize(bufferSize);
        // Copy the string data into the buffer
        std::copy(stringDataSource->DString.begin(), stringDataSource->DString.end(), commandBuffer.begin());
        // Convert the buffer to a string
        command = std::string(commandBuffer.begin(), commandBuffer.end());
    }

    // Move the initialization of std::istringstream inside the block
    if (!command.empty()) {
        std::istringstream iss(command);
        std::copy(std::istream_iterator<std::string>(iss),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(tokens));
    }

    // Process the command based on the first token
    if (tokens.empty()) {
        const char* outputMessage = "> ";
        std::vector<char> outputBuffer(outputMessage, outputMessage + strlen(outputMessage));
        // Clear previous content
        DImplementation->OutputSink->Write(std::vector<char>());
        DImplementation->OutputSink->Write(outputBuffer);
        return true;
    }

    if (tokens[0] == "help") {
        // Example: Display help menu
        std::string helpMessage = "> "
                                    "------------------------------------------------------------------------\n"
                                    "help     Display this help menu\n"
                                    "exit     Exit the program\n"
                                    "count    Output the number of nodes in the map\n"
                                    "node     Syntax \"node [0, count)\" \n"
                                    "         Will output node ID and Lat/Lon for node\n"
                                    "fastest  Syntax \"fastest start end\" \n"
                                    "         Calculates the time for fastest path from start to end\n"
                                    "shortest Syntax \"shortest start end\" \n"
                                    "         Calculates the distance for the shortest path from start to end\n"
                                    "save     Saves the last calculated path to file\n"
                                    "print    Prints the steps for the last calculated path\n"
                                    "> ";
        std::vector<char> helpBuffer(helpMessage.begin(), helpMessage.end());
        DImplementation->OutputSink->Write(helpBuffer);
    } else if (tokens[0] == "count") {
        // Process "count" command
        auto nodeCount = DImplementation->Planner->NodeCount();
        std::string message = "> " + std::to_string(nodeCount) + " nodes\n> ";
        // Convert the string to a vector<char>
        std::vector<char> messageBuffer(message.begin(), message.end());
        DImplementation->OutputSink->Write(messageBuffer);
    } else if (tokens[0] == "node") {
        // Example: Syntax "node [0, count)"
        // Will output node ID and Lat/Lon for node
        if (tokens.size() == 2) {
            try {
                std::size_t index = std::stoul(tokens[1]);
                auto node = DImplementation->Planner->SortedNodeByIndex(index);
                std::string message = "> Node " + std::to_string(index) + ": id = " +
                                      std::to_string(node->ID()) + " is at " +
                                      std::to_string(node->Location().first) + " N, " +
                                      std::to_string(node->Location().second) + " W\n> ";
                std::vector<char> messageBuffer(message.begin(), message.end());
                DImplementation->OutputSink->Write(messageBuffer);
            } catch (const std::invalid_argument& e) {
                std::string errorMessage = "Invalid node parameter, see help.";
                std::vector<char> errorBuffer(errorMessage.begin(), errorMessage.end());
                DImplementation->ErrorSink->Write(errorBuffer);
                return false;
            }
        } else {
            const char errorMessage[] = "Invalid node command, see help.";
            std::vector<char> errorBuffer(errorMessage, errorMessage + sizeof(errorMessage) - 1);
            DImplementation->ErrorSink->Write(errorBuffer);
            return false;
        }
    } else if (tokens[0] == "shortest") {
        // Example: Syntax "shortest start end"
        // Calculates the distance for the shortest path from start to end
        if (tokens.size() == 3) {
            try {
                CStreetMap::TNodeID start = std::stoul(tokens[1]);
                CStreetMap::TNodeID end = std::stoul(tokens[2]);
                std::vector<CStreetMap::TNodeID> path;
                double distance = DImplementation->Planner->FindShortestPath(start, end, path);
                // Example: Output shortest path distance
                std::string message = "> Shortest path is " + std::to_string(distance) + " units.\n> ";
                std::vector<char> messageBuffer(message.begin(), message.end());
                DImplementation->OutputSink->Write(messageBuffer);
            } catch (const std::invalid_argument& e) {
                std::string errorMessage = "Invalid shortest path parameters, see help.";
                std::vector<char> errorBuffer(errorMessage.begin(), errorMessage.end());
                DImplementation->ErrorSink->Write(errorBuffer);
                return false;
            }
        } else {
            const char errorMessage[] = "Invalid shortest path command, see help.";
            std::vector<char> errorBuffer(errorMessage, errorMessage + sizeof(errorMessage) - 1);
            DImplementation->ErrorSink->Write(errorBuffer);
            return false;
        }
    } else if (tokens[0] == "fastest") {
        // Example: Syntax "fastest start end"
        // Calculates the time for the fastest path from start to end
        if (tokens.size() == 3) {
            try {
                CStreetMap::TNodeID start = std::stoul(tokens[1]);
                CStreetMap::TNodeID end = std::stoul(tokens[2]);
                std::vector<CTransportationPlanner::TTripStep> path;  // Adjust the type of path
                double time = DImplementation->Planner->FindFastestPath(start, end, path);
                // Example: Output fastest path time
                std::string message = "> Fastest path is " + std::to_string(time) + " units of time.\n> ";
                std::vector<char> messageBuffer(message.begin(), message.end());
                DImplementation->OutputSink->Write(messageBuffer);
            } catch (const std::invalid_argument& e) {
                std::string errorMessage = "Invalid fastest path parameters, see help.";
                std::vector<char> errorBuffer(errorMessage.begin(), errorMessage.end());
                DImplementation->ErrorSink->Write(errorBuffer);
                return false;
            }
        } else {
            const char errorMessage[] = "Invalid fastest path command, see help.";
            std::vector<char> errorBuffer(errorMessage, errorMessage + sizeof(errorMessage) - 1);
            DImplementation->ErrorSink->Write(errorBuffer);
            return false;
        }
    } else if (tokens[0] == "save") {
        // Example: Save the last calculated path to file
        if (tokens.size() == 1) {
            // Assume the last calculated path is stored in Planner
            // Modify the following line accordingly based on your implementation
            std::vector<CStreetMap::TNodeID> path;
            double distance = DImplementation->Planner->FindShortestPath(0, 1, path);

            // Create a string representation of the path
            std::ostringstream pathStream;
            std::copy(path.begin(), path.end(), std::ostream_iterator<CStreetMap::TNodeID>(pathStream, " "));

            // Check if Results is a CDataSink
            CDataSink* dataSink = dynamic_cast<CDataSink*>(DImplementation->Results.get());
            if (dataSink) {
                // Write the path to the output file using the CDataSink
                std::vector<char> pathData(pathStream.str().begin(), pathStream.str().end());
                dataSink->Write(pathData);
                std::string message = "> Path saved to file.\n> ";
                std::vector<char> messageBuffer(message.begin(), message.end());
                DImplementation->OutputSink->Write(messageBuffer);
            } else {
                // Handle the case where Results is not a CDataSink
                const char errorMessage[] = "Results must be a CDataSink for saving paths to file.";
                std::vector<char> errorBuffer(errorMessage, errorMessage + sizeof(errorMessage) - 1);
                DImplementation->ErrorSink->Write(errorBuffer);
                return false;
            }
        } else {
            const char errorMessage[] = "Invalid save command, see help.";
            std::vector<char> errorBuffer(errorMessage, errorMessage + sizeof(errorMessage) - 1);
            DImplementation->ErrorSink->Write(errorBuffer);
            return false;
        }
    } else if (tokens[0] == "print") {
        // Example: Print the steps for the last calculated path
        if (tokens.size() == 1) {
            // Assume the last calculated path is stored in Planner
            // Modify the following line accordingly based on your implementation
            std::vector<CStreetMap::TNodeID> path;
            double distance = DImplementation->Planner->FindShortestPath(0, 1, path);
            // Create a string representation of the path
            std::ostringstream pathStream;
            std::copy(path.begin(), path.end(), std::ostream_iterator<CStreetMap::TNodeID>(pathStream, " "));
            // Output the path steps
            std::string message = "> Path: " + pathStream.str() + "\n> ";
            std::vector<char> messageBuffer(message.begin(), message.end());
            DImplementation->OutputSink->Write(messageBuffer);
        } else {
            const char errorMessage[] = "Invalid print command, see help.";
            std::vector<char> errorBuffer(errorMessage, errorMessage + sizeof(errorMessage) - 1);
            DImplementation->ErrorSink->Write(errorBuffer);
            return false;
        }
    } else if (tokens[0] == "exit") {
        // Example: Exit the program
        const char exitMessage[] = "> ";
        std::vector<char> exitBuffer(exitMessage, exitMessage + sizeof(exitMessage) - 1);
        DImplementation->OutputSink->Write(exitBuffer);
        return true;
    } else {
        // Example: Unknown command
        const char errorMessage[] = "Unknown command \"foo\" type help for help.\n"
                                    "Invalid node command, see help.\n"
                                    "Invalid node parameter, see help.\n"
                                    "Invalid shortest command, see help.\n"
                                    "Invalid shortest parameter, see help.\n"
                                    "Invalid fastest command, see help.\n"
                                    "Invalid fastest parameter, see help.\n"
                                    "No valid path to save, see help.\n"
                                    "No valid path to print, see help.\n";
        std::vector<char> errorBuffer(errorMessage, errorMessage + sizeof(errorMessage) - 1);
        DImplementation->ErrorSink->Write(errorBuffer);
    }

    return true;
}