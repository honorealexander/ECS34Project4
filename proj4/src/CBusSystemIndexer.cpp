#include "BusSystemIndexer.h"
#include <memory>
#include <unordered_set>
#include <algorithm>
#include <iostream>

struct CBusSystemIndexer::SImplementation {
    std::shared_ptr<CBusSystem> BusSystem;
    // additional private members as needed for implementation

    SImplementation(std::shared_ptr<CBusSystem> bussystem) : BusSystem(bussystem) {
        // initialize additional members if needed
    }
};

CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem) : DImplementation(std::make_unique<SImplementation>(bussystem)) {}

CBusSystemIndexer::~CBusSystemIndexer() = default;

std::size_t CBusSystemIndexer::StopCount() const noexcept {
    return (DImplementation->BusSystem) ? DImplementation->BusSystem->StopCount() : 0;
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept {
    return (DImplementation->BusSystem) ? DImplementation->BusSystem->RouteCount() : 0;
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept {
    auto stops = std::vector<std::shared_ptr<CBusSystem::SStop>>();
    
    // Directly query the BusSystem member
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
    auto routes = std::vector<std::shared_ptr<CBusSystem::SRoute>>();
    for (std::size_t i = 0; i < RouteCount(); ++i) {
        auto route = DImplementation->BusSystem->RouteByIndex(i);
        if (route) {
            routes.push_back(route);
        }
    }

    if (!routes.empty() && index < routes.size()) {
        std::sort(routes.begin(), routes.end(), [](const auto &lhs, const auto &rhs) {
            return lhs->Name() < rhs->Name();
        });

        return routes[index];
    }

    return nullptr;
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::StopByNodeID(TNodeID id) const noexcept {
    for (std::size_t i = 0; i < StopCount(); ++i) {
        auto stop = DImplementation->BusSystem->StopByIndex(i);
        if (stop && stop->NodeID() == id) {
            return stop;
        }
    }
    return nullptr; // NodeID not found
}

bool CBusSystemIndexer::RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<CBusSystem::SRoute>> &routes) const noexcept {
    if (src == dest) {
        return false; // Source and destination are the same, no valid routes
    }

    for (std::size_t i = 0; i < RouteCount(); ++i) {
        auto route = DImplementation->BusSystem->RouteByIndex(i);
        if (route) {
            const auto stopCount = route->StopCount();
            bool srcFound = false;
            bool destFound = false;

            for (std::size_t j = 0; j < stopCount; ++j) {
                auto stopID = route->GetStopID(j);
                auto stop = DImplementation->BusSystem->StopByID(stopID);
                if (stop) {
                    if (stop->NodeID() == src) {
                        srcFound = true;
                    } else if (stop->NodeID() == dest) {
                        destFound = true;
                    }
                }
            }

            // Insert the route if both source and destination stops are found
            if (srcFound && destFound) {
                routes.insert(route);
            }
        }
    }

    // If valid routes are found, the function should return true
    return !routes.empty();
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept {
    if (src == dest) {
        return false; // Source and destination are the same, no valid route
    }

    for (std::size_t i = 0; i < RouteCount(); ++i) {
        auto route = DImplementation->BusSystem->RouteByIndex(i);
        if (route) {
            const auto stopCount = route->StopCount();
            bool srcFound = false;
            bool destFound = false;

            for (std::size_t j = 0; j < stopCount; ++j) {
                auto stopID = route->GetStopID(j);
                auto stop = DImplementation->BusSystem->StopByID(stopID);
                if (stop) {
                    if (stop->NodeID() == src) {
                        srcFound = true;
                    } else if (stop->NodeID() == dest) {
                        destFound = true;
                    }

                    // Break early if both source and destination stops are found
                    if (srcFound && destFound) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}