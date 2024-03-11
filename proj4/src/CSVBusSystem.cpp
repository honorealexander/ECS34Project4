#include "CSVBusSystem.h"
#include <vector>
#include <cctype>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <algorithm>

struct CCSVBusSystem::SImplementation : public CBusSystem{
    struct SStop : public CBusSystem::SStop {
        CBusSystem::TStopID id;
        CStreetMap::TNodeID nodeID;
        SStop(CBusSystem::TStopID id, CStreetMap::TNodeID nodeID) : id(id), nodeID(nodeID) {}

        CBusSystem::TStopID ID() const noexcept override {
            return id;
        }
        CStreetMap::TNodeID NodeID() const noexcept override {
            return nodeID;
        }


    };
    struct SRoute : public CBusSystem::SRoute {
        std::string name;
        std::vector<CBusSystem::TStopID> stopIDs;
        std::unordered_set<CBusSystem::TStopID> stopIDsSet;  //added member

        SRoute(const std::string &name, const std::vector<CBusSystem::TStopID> &stopIDs) : name(name), stopIDs(stopIDs) {
            //initialize stopIDsSet with content of stopIDs
            stopIDsSet.insert(stopIDs.begin(), stopIDs.end());
        }

        std::string Name() const noexcept override {
            return name;
        }
        //returns the number of stops in system
        std::size_t StopCount() const noexcept override {
            return stopIDs.size();
        }

        CBusSystem::TStopID GetStopID(std::size_t index) const noexcept override {
            if (index < stopIDs.size()) {
                return stopIDs[index];
            } else {
                return InvalidStopID;
            }
        }
    };
    std::vector<std::shared_ptr<SStop>> Stops;
    std::vector<std::shared_ptr<SRoute>> Routes;
    std::unordered_map<CBusSystem::TStopID, std::shared_ptr<CBusSystem::SStop>> StopsByID;
    std::unordered_map<std::string, std::shared_ptr<CBusSystem::SRoute>> RoutesByName;

    std::size_t StopCount() const noexcept override{
        return Stops.size();
    }
    //returns the number of routes in  system
    std::size_t RouteCount() const noexcept override{
        return Routes.size();
    }

    std::shared_ptr<CBusSystem::SStop> StopByIndex(std::size_t index) const noexcept override{
        if (index < Stops.size()) {
            return Stops[index];
        } else {
            return nullptr;
        }
    }

    std::shared_ptr<CBusSystem::SStop> StopByID(TStopID id) const noexcept override{
        auto it = StopsByID.find(id);
        if (it != StopsByID.end()) {
            return it->second;
        } else {
        // Print a debug message if the stop is not found
        std::cerr << "Stop with ID " << id << " not found." << std::endl;
        return nullptr;
        }
    }

    std::shared_ptr<CBusSystem::SRoute> RouteByIndex(std::size_t index) const noexcept override{
        if (index < Routes.size()) {
            return Routes[index];
        } else {
            return nullptr;
        }
    }

    std::shared_ptr<CBusSystem::SRoute> RouteByName(const std::string &name) const noexcept override{
        auto it = RoutesByName.find(name);
        if (it != RoutesByName.end()){
            return it->second;
        } else {
            return nullptr;
        }

    }

};

CCSVBusSystem::CCSVBusSystem(std::shared_ptr<CDSVReader> stopsrc, std::shared_ptr<CDSVReader> routesrc) : DImplementation(std::make_unique<SImplementation>()) {
    std::vector<std::string> row;

    std::unordered_map<CBusSystem::TStopID, std::shared_ptr<SImplementation::SStop>> tempStops;
    std::unordered_map<CBusSystem::TStopID, std::shared_ptr<SImplementation::SStop>> tempStopsByID;  // Separate map
    std::unordered_map<std::string, std::shared_ptr<SImplementation::SRoute>> tempRoutes;

    while (stopsrc->ReadRow(row)) {
        if (row.size() < 2 || (row[0] == "stop_id" && row[1] == "node_id")) {
            continue;  //skip empty rows and also the title row
        }
        CBusSystem::TStopID id = std::stoull(row[0]);
        CStreetMap::TNodeID nodeID = std::stoull(row[1]);

        //create new stop
        std::shared_ptr<SImplementation::SStop> stop = std::make_shared<SImplementation::SStop>(id, nodeID);

        //add stop to temporary stops map and main Stops vector
        tempStops[id] = stop;
        tempStopsByID[id] = stop;

        DImplementation->Stops.push_back(stop);
        DImplementation->StopsByID[id] = stop;
        //print stop info for debugging
        std::cout << "Added stop: ID=" << stop->ID() << ", NodeID=" << stop->NodeID() << std::endl;
    }
    
    while (routesrc->ReadRow(row)) {
        if (row.size() < 2 || (row[0] == "route" && row[1] == "stop_id")) {
            continue;  //skip empty rows and title row
        }
        std::string routeName = row[0]; //get route name
        CBusSystem::TStopID stopID = std::stoull(row[1]);

        //check if the route name already exists in temporary routes
        auto routeIt = tempRoutes.find(routeName);
        if (routeIt == tempRoutes.end() || routeIt->second->stopIDsSet.find(stopID) == routeIt->second->stopIDsSet.end()) {
            auto& route = tempRoutes[routeName];
            if (!route) {
                route = std::make_shared<SImplementation::SRoute>(routeName, std::vector<CBusSystem::TStopID>{stopID});
                route->stopIDsSet.insert(stopID);
            } else {
                route->stopIDs.push_back(stopID);
                route->stopIDsSet.insert(stopID);
            }
        }
    }


    //transfer routes from tempRoutes to main Routes vector
    for (const auto& entry : tempRoutes) {
        DImplementation->Routes.push_back(entry.second);
        DImplementation->RoutesByName[entry.first] = entry.second;
    }

}



CCSVBusSystem::~CCSVBusSystem() = default;

std::size_t CCSVBusSystem::StopCount() const noexcept  {
    std::cout << "Number of Stops: " << DImplementation->Stops.size() << std::endl;
    for (const auto& stop : DImplementation->Stops) {
        std::cout << "Stop ID: " << stop->ID() << ", NodeID: " << stop->NodeID() << std::endl;
    }
    return DImplementation->Stops.size();
}


std::size_t CCSVBusSystem::RouteCount() const noexcept {
    return DImplementation->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByIndex(std::size_t index) const noexcept {
    return DImplementation->StopByIndex(index);
}

std::shared_ptr<CBusSystem::SStop> CCSVBusSystem::StopByID(TStopID id) const noexcept {
    auto it = DImplementation->StopsByID.find(id);
    if (it != DImplementation->StopsByID.end()) {
        return DImplementation->StopByID(id);
    } else {
        //print a message indicating the stop was not found
        std::cerr << "Stop with ID " << id << " not found." << std::endl;
        //return a null pointer
        return nullptr;
    }
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByIndex(std::size_t index) const noexcept {
    return DImplementation->RouteByIndex(index);
}

std::shared_ptr<CBusSystem::SRoute> CCSVBusSystem::RouteByName(const std::string &name) const noexcept {
    return DImplementation->RouteByName(name);
}