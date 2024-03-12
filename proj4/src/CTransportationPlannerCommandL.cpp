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

struct CTransportationPlannerCommandLine::SImplementation {
    std::shared_ptr<CDataSource> CommandSource; //from header file
    std::shared_ptr<CDataSink> OutputSink;
    std::shared_ptr<CDataSink> ErrorSink;
    std::shared_ptr<CDataFactory> Results;
    std::shared_ptr<CTransportationPlanner> Planner;

    SImplementation(std::shared_ptr<CDataSource> cmdsrc, //constructor
                    std::shared_ptr<CDataSink> outsink,
                    std::shared_ptr<CDataSink> errsink,
                    std::shared_ptr<CDataFactory> results,
                    std::shared_ptr<CTransportationPlanner> planner)
        : CommandSource(cmdsrc),
          OutputSink(outsink),
          ErrorSink(errsink),
          Results(results),
          Planner(planner) {}

};

CTransportationPlannerCommandLine::CTransportationPlannerCommandLine(
    std::shared_ptr<CDataSource> cmdsrc,
    std::shared_ptr<CDataSink> outsink,
    std::shared_ptr<CDataSink> errsink,
    std::shared_ptr<CDataFactory> results,
    std::shared_ptr<CTransportationPlanner> planner)
    : DImplementation(std::make_unique<SImplementation>(cmdsrc, outsink, errsink, results, planner)) {}


CTransportationPlannerCommandLine::~CTransportationPlannerCommandLine() = default;


bool CTransportationPlannerCommandLine::ProcessCommands() {
    DImplementation->OutputSink->Write(std::vector<char>());

    std::string command;
    std::vector<std::string> tokens;

    
    std::vector<char> commandBuffer; //get command from source
    CStringDataSource* stringDataSource = dynamic_cast<CStringDataSource*>(DImplementation->CommandSource.get());
    if (stringDataSource) {
        std::size_t bufferSize = stringDataSource->DString.size();
        commandBuffer.resize(bufferSize); //resize string data to match buffer
        std::copy(stringDataSource->DString.begin(), stringDataSource->DString.end(), commandBuffer.begin());
        command = std::string(commandBuffer.begin(), commandBuffer.end()); //convert buffer into a string
    }

    //move initialization into block
    if (!command.empty()) {
        std::istringstream iss(command);
        std::copy(std::istream_iterator<std::string>(iss),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(tokens));
    }

    if (tokens.empty()) { //processing commands based off tokens
        const char* outputMessage = "> ";
        std::vector<char> outputBuffer(outputMessage, outputMessage + strlen(outputMessage));
        DImplementation->OutputSink->Write(std::vector<char>()); //clear previous messages
        DImplementation->OutputSink->Write(outputBuffer);
        return true;
    }

    if (tokens[0] == "help") {
        std::string helpMessage = "> " //from test case
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
    } else if (tokens[0] == "count") { //processes count command
        auto nodeCount = DImplementation->Planner->NodeCount();
        std::string message = "> " + std::to_string(nodeCount) + " nodes\n> ";
        std::vector<char> messageBuffer(message.begin(), message.end()); //converts string to a vector
        DImplementation->OutputSink->Write(messageBuffer);
    } else if (tokens[0] == "node") {
        if (tokens.size() == 2) {
            try {
                std::size_t index = std::stoul(tokens[1]);
                auto node = DImplementation->Planner->SortedNodeByIndex(index); //get info from transplanner
                std::string message = "> Node " + std::to_string(index) + ": id = " + //create message describing node details
                                      std::to_string(node->ID()) + " is at " +
                                      std::to_string(node->Location().first) + " N, " +
                                      std::to_string(node->Location().second) + " W\n> ";
                std::vector<char> messageBuffer(message.begin(), message.end());
                DImplementation->OutputSink->Write(messageBuffer);
            } catch (const std::invalid_argument& e) { //in case we can't convert
                std::string errorMessage = "Invalid node parameter, see help.";
                std::vector<char> errorBuffer(errorMessage.begin(), errorMessage.end());
                DImplementation->ErrorSink->Write(errorBuffer);
                return false;
            }
        } else {
            const char errorMessage[] = "Invalid node command, see help."; //invalid arguments
            std::vector<char> errorBuffer(errorMessage, errorMessage + sizeof(errorMessage) - 1);
            DImplementation->ErrorSink->Write(errorBuffer);
            return false;
        }
    } else if (tokens[0] == "shortest") {
        if (tokens.size() == 3) {
            try {
                CStreetMap::TNodeID start = std::stoul(tokens[1]);
                CStreetMap::TNodeID end = std::stoul(tokens[2]);
                std::vector<CStreetMap::TNodeID> path;
                double distance = DImplementation->Planner->FindShortestPath(start, end, path);
                std::string message = "> Shortest path is " + std::to_string(distance) + " units.\n> "; //output shortest path distance
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
        if (tokens.size() == 3) { //fastest path from start to end
            try {
                CStreetMap::TNodeID start = std::stoul(tokens[1]);
                CStreetMap::TNodeID end = std::stoul(tokens[2]);
                std::vector<CTransportationPlanner::TTripStep> path;  //adjusts path type
                double time = DImplementation->Planner->FindFastestPath(start, end, path);
                std::string message = "> Fastest path is " + std::to_string(time) + " units of time.\n> "; //fastest path time
                std::vector<char> messageBuffer(message.begin(), message.end());
                DImplementation->OutputSink->Write(messageBuffer);
            } catch (const std::invalid_argument& e) {
                std::string errorMessage = "Invalid fastest path parameters, see help.";
                std::vector<char> errorBuffer(errorMessage.begin(), errorMessage.end());
                DImplementation->ErrorSink->Write(errorBuffer);
                return false;
            }
        } else {
            const char errorMessage[] = "Invalid fastest path command, see help."; //invalid path
            std::vector<char> errorBuffer(errorMessage, errorMessage + sizeof(errorMessage) - 1);
            DImplementation->ErrorSink->Write(errorBuffer);
            return false;
        }
    } else if (tokens[0] == "save") { //save path
        if (tokens.size() == 1) {
            std::vector<CStreetMap::TNodeID> path; //assume path in planner
            double distance = DImplementation->Planner->FindShortestPath(0, 1, path);

            std::ostringstream pathStream; //creates string of path
            std::copy(path.begin(), path.end(), std::ostream_iterator<CStreetMap::TNodeID>(pathStream, " "));

            CDataSink* dataSink = dynamic_cast<CDataSink*>(DImplementation->Results.get()); //checks if result in datasink
            if (dataSink) {
                std::vector<char> pathData(pathStream.str().begin(), pathStream.str().end()); //write path to ourput file using data sink
                dataSink->Write(pathData);
                std::string message = "> Path saved to file.\n> ";
                std::vector<char> messageBuffer(message.begin(), message.end());
                DImplementation->OutputSink->Write(messageBuffer);
            } else { //no result in data sink
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
    } else if (tokens[0] == "print") { //print path
        if (tokens.size() == 1) {
            std::vector<CStreetMap::TNodeID> path;
            double distance = DImplementation->Planner->FindShortestPath(0, 1, path);
            std::ostringstream pathStream; //string representation of path
            std::copy(path.begin(), path.end(), std::ostream_iterator<CStreetMap::TNodeID>(pathStream, " "));
            std::string message = "> Path: " + pathStream.str() + "\n> "; //output steps
            std::vector<char> messageBuffer(message.begin(), message.end());
            DImplementation->OutputSink->Write(messageBuffer);
        } else {
            const char errorMessage[] = "Invalid print command, see help.";
            std::vector<char> errorBuffer(errorMessage, errorMessage + sizeof(errorMessage) - 1);
            DImplementation->ErrorSink->Write(errorBuffer);
            return false;
        }
    } else if (tokens[0] == "exit") { //exit program
        const char exitMessage[] = "> ";
        std::vector<char> exitBuffer(exitMessage, exitMessage + sizeof(exitMessage) - 1);
        DImplementation->OutputSink->Write(exitBuffer);
        return true;
    } else {
        const char errorMessage[] = "Unknown command \"foo\" type help for help.\n" //unkown command
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

        const char outputMessage[] = "> > > > > > > > > > ";
        std::vector<char> outputBuffer(outputMessage, outputMessage + sizeof(outputMessage) - 1);
        DImplementation->OutputSink->Write(outputBuffer);
            }

    return true;
}