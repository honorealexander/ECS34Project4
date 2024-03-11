#include "TransportationPlannerCOmmandLine.h"
#include "TransportationPlannerConfig.h"
#include "TransportationPlanner.h"
#include <iterator>
#include <sstream>

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

    // Add any other necessary member functions here
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

    std::string command;
    std::vector<std::string> tokens;

    std::istringstream iss(command);
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(tokens));
              
    std::vector<CTransportationPlanner::TTripStep> path;
    // Process the command based on the first token
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

        DImplementation->OutputSink->Write(std::vector<char>(helpMessage.begin(), helpMessage.end()));
    } else if (tokens[0] == "exit") {
        // Example: Exit the program
        return true;
    } else if (tokens[0] == "count") {
        // Example: Output the number of nodes in the map
        auto nodeCount = DImplementation->Planner->NodeCount();
        std::string message = "> " + std::to_string(nodeCount) + " nodes\n> ";
        DImplementation->OutputSink->Write(std::vector<char>(message.begin(), message.end()));
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
                DImplementation->OutputSink->Write(std::vector<char>(message.begin(), message.end()));
            } catch (const std::invalid_argument& e) {
                std::string errorMessage = "Invalid node parameter, see help.";
                DImplementation->ErrorSink->Write(std::vector<char>(errorMessage.begin(), errorMessage.end()));
                return false;
            }
        } else {
            DImplementation->ErrorSink->Write(std::vector<char>("Invalid node command, see help.", "Invalid node command, see help." + 1));
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
                DImplementation->ErrorSink->Write(std::vector<char>(std::string("Shortest path is ").c_str(), std::string("Shortest path is ").c_str() + 20));

            } catch (const std::invalid_argument& e) {
                
                DImplementation->ErrorSink->Write(std::vector<char>("Invalid shortest parameter, see help.", "Invalid shortest parameter, see help." + 38));
                
                return false;
            }
        } else {
            DImplementation->ErrorSink->Write(std::vector<char>("Invalid shortest command, see help.", "Invalid shortest command, see help." + 36));

            return false;
        }
    } else if (tokens[0] == "fastest") {
        // Example: Syntax "fastest start end"
        // Calculates the time for the fastest path from start to end
        if (tokens.size() == 3) {
            try {
                CStreetMap::TNodeID start = std::stoul(tokens[1]);
                CStreetMap::TNodeID end = std::stoul(tokens[2]);
                std::vector<CTransportationPlanner::TTripStep> path;
                double time = DImplementation->Planner->FindFastestPath(start, end, path);
                // Example: Output fastest path time
                std::string message = "> Fastest path takes " + std::to_string(time) + " min.\n> ";
                DImplementation->OutputSink->Write(std::vector<char>(message.begin(), message.end()));
            } catch (const std::invalid_argument& e) {
                DImplementation->ErrorSink->Write(std::vector<char>(
                    "Invalid fastest parameter, see help.",
                    "Invalid fastest parameter, see help." + 37
                ));
                return false;
            }
        } else {
            DImplementation->ErrorSink->Write(std::vector<char>(
                "Invalid fastest command, see help.",
                "Invalid fastest command, see help." + 35
            ));
            return false;
        }
    } else if (tokens[0] == "save") {
        // Example: Saves the last calculated path to file
        // Assuming the path has been calculated in the previous command
        // and stored in DResults, and the file name is generated based on the start, end, and time
        try {
            auto saveSink = DImplementation->Results->CreateSink(
                "results/" +
                std::to_string(std::stoul(tokens[1])) + "_" +
                std::to_string(std::stoul(tokens[2])) + "_" +
                std::to_string(DImplementation->Planner->FindFastestPath(std::stoul(tokens[1]), std::stoul(tokens[2]), path)) +
                "hr.csv");
            // Assuming path is a vector of TTripStep, modify this according to your actual implementation
            // Example: DResults->SaveResults(saveSink, path);
            std::string message = "> Path saved to <results>/" +
                std::to_string(std::stoul(tokens[1])) + "_" +
                std::to_string(std::stoul(tokens[2])) + "_" +
                std::to_string(DImplementation->Planner->FindFastestPath(std::stoul(tokens[1]), std::stoul(tokens[2]), path)) +
                "hr.csv\n> ";

            DImplementation->OutputSink->Write(std::vector<char>(message.begin(), message.end()));
        } catch (const std::invalid_argument& e) {
            DImplementation->ErrorSink->Write(std::vector<char>(
                "No valid path to save, see help.",
                "No valid path to save, see help." + 50
            ));
            return false;
        }
    } else if (tokens[0] == "print") {
        // Example: Prints the steps for the last calculated path
        // Assuming the path has been calculated in the previous command
        // and stored in DResults
        // Assuming path is a vector of TTripStep, modify this according to your actual implementation
        // Example: DResults->PrintPath(DOutputSink, path);
        DImplementation->OutputSink->Write(std::vector<char>{'>', ' '});

    } else {
        DImplementation->ErrorSink->Write(std::vector<char>(tokens[0].begin(), tokens[0].end()));

        return false;
    }

    return true;
}