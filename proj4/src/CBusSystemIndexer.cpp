#include "BusSystemIndexer.h"
#include <memory>
#include <unordered_set>
#include <algorithm>
#include <iostream>

struct CBusSystemIndexer::SImplementation {
    std::shared_ptr<CBusSystem> BusSystem;

    SImplementation(std::shared_ptr<CBusSystem> bussystem) : BusSystem(bussystem) {
       
    }
};

CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem)
    : DImplementation(std::make_unique<SImplementation>(bussystem)) {}

CBusSystemIndexer::~CBusSystemIndexer() = default;


std::size_t CBusSystemIndexer::StopCount() const noexcept { //checks for null pointer, chatgpt suggester ternary operator
    return (DImplementation->BusSystem) ? DImplementation->BusSystem->StopCount() : 0;
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept { //checks for null pointer
    return (DImplementation->BusSystem) ? DImplementation->BusSystem->RouteCount() : 0;
}

//returns stopp at specific index from a vector of stops
//retrieves stops and sorts them by id before return
//retun nullptr if index out of range or vector is empty
std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept {
    auto stops = std::vector<std::shared_ptr<CBusSystem::SStop>>();

    //directly query the BusSystem member
    for (std::size_t i = 0; i < DImplementation->BusSystem->StopCount(); ++i) {
        auto stop = DImplementation->BusSystem->StopByIndex(i);
        if (stop) {
            stops.push_back(stop);
        }
    }

    if (!stops.empty() && index < stops.size()) {
        std::sort(stops.begin(), stops.end(), [](const auto &lhs, const auto &rhs) {
            return lhs->ID() < rhs->ID();
        });

        return stops[index];
    }

    return nullptr;
}

std::shared_ptr<CBusSystem::SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept {
    auto routes = std::vector<std::shared_ptr<CBusSystem::SRoute>>(); //stores routes
    for (std::size_t i = 0; i < RouteCount(); ++i) {
        auto route = DImplementation->BusSystem->RouteByIndex(i);
        if (route) { //if route valid add to vector
            routes.push_back(route);
        }
    }

    if (!routes.empty() && index < routes.size()) { //make sure routes is sortable
        std::sort(routes.begin(), routes.end(), [](const auto &lhs, const auto &rhs) { //stack overflow
            if (lhs->Name() != rhs->Name()) { //compare based on route name than on stop count,
                return lhs->Name() < rhs->Name();
            }
            return lhs->StopCount() < rhs->StopCount();
        });

        return routes[index]; //return route at index after sorting is complete
    }

    return nullptr;
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::StopByNodeID(TNodeID id) const noexcept {
    for (std::size_t i = 0; i < StopCount(); ++i) { //iterate through until we find our stop by id
        auto stop = DImplementation->BusSystem->StopByIndex(i);
        if (stop && stop->NodeID() == id) {
            return stop; 
        }
    }
    return nullptr; //return null if not found
}

bool CBusSystemIndexer::RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<CBusSystem::SRoute>> &routes) const noexcept {
    if (src == dest) {
        return false; //handle incorrect input, they cannot equal each other
    }

    for (std::size_t i = 0; i < RouteCount(); ++i) {
        auto route = DImplementation->BusSystem->RouteByIndex(i);
        if (!route) {
            std::cerr << "Route with index " << i << " not found." << std::endl;
            continue;  //skip to next if route not found
        }

        const auto stopCount = route->StopCount();
        bool srcFound = false;
        bool destFound = false;

        
        std::unordered_set<TNodeID> routeStops; //create set for stops in route

        for (std::size_t j = 0; j < stopCount; ++j) {
            auto stopID = route->GetStopID(j);
            auto stop = DImplementation->BusSystem->StopByID(stopID);
            if (!stop) {
                std::cerr << "Stop with ID " << stopID << " not found in route " << route->Name() << "." << std::endl;
                continue;  //skip if route not found
            }

            routeStops.insert(stop->NodeID());

            if (stop->NodeID() == src) { //checks to make sure src and dest aren't in current route
                srcFound = true;
            } else if (stop->NodeID() == dest) {
                destFound = true;
            }
        }

        //add route if path found
        if (srcFound && destFound) {
            routes.insert(route);
        }
    }
    //debug
    std::cout << "Source: " << src << ", Destination: " << dest << ", Routes size: " << routes.size() << std::endl;

    //success if at least one route is found
    return !routes.empty();
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept {
    if (src == dest) {
        return false; //same as above
    }

    for (std::size_t i = 0; i < RouteCount(); ++i) { //iterate through all routes
        auto route = DImplementation->BusSystem->RouteByIndex(i); //get route at current index
        if (route) {
            const auto stopCount = route->StopCount(); //get stop count for route
            bool srcFound = false;
            bool destFound = false;

            for (std::size_t j = 0; j < stopCount; ++j) { //iterates through stops within current route
                auto stopID = route->GetStopID(j); //looking for src and dest
                auto stop = DImplementation->BusSystem->StopByID(stopID);
                if (stop) {
                    if (stop->NodeID() == src) {
                        srcFound = true;
                    } else if (stop->NodeID() == dest) {
                        destFound = true;
                    }

                    //break early if both source and destination stops are found
                    if (srcFound && destFound) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
