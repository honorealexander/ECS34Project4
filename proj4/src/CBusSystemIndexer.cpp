#include "BusSystemIndexer.h"
#include <memory>
#include <unordered_set>
#include <algorithm>

struct CBusSystemIndexer::SImplementation {
    std::shared_ptr<CBusSystem> BusSystem;
    // aditional private members as needed for implementation

    SImplementation(std::shared_ptr<CBusSystem> bussystem) : BusSystem(bussystem) {
        //initialize additional members if needed
    }
};

CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem) : DImplementation(std::make_unique<SImplementation>(bussystem)) {}

CBusSystemIndexer::~CBusSystemIndexer() = default;

std::size_t CBusSystemIndexer::StopCount() const noexcept {
    return DImplementation->BusSystem->StopCount();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept {
    return DImplementation->BusSystem->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept {
    auto stops = std::vector<std::shared_ptr<CBusSystem::SStop>>();
    for (std::size_t i = 0; i < StopCount(); ++i) {
        stops.push_back(DImplementation->BusSystem->StopByIndex(i));
    }

    std::sort(stops.begin(), stops.end(), [](const auto &lhs, const auto &rhs) {
        return lhs->ID() < rhs->ID();
    });

    return (index < stops.size()) ? stops[index] : nullptr;
}

std::shared_ptr<CBusSystem::SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept {
    auto routes = std::vector<std::shared_ptr<CBusSystem::SRoute>>();
    for (std::size_t i = 0; i < RouteCount(); ++i) {
        routes.push_back(DImplementation->BusSystem->RouteByIndex(i));
    }

    std::sort(routes.begin(), routes.end(), [](const auto &lhs, const auto &rhs) {
        return lhs->Name() < rhs->Name();
    });

    return (index < routes.size()) ? routes[index] : nullptr;
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::StopByNodeID(TNodeID id) const noexcept {
    for (std::size_t i = 0; i < StopCount(); ++i) {
        auto stop = DImplementation->BusSystem->StopByIndex(i);
        if (stop && stop->NodeID() == id) {
            return stop;
        }
    }
    return nullptr; //NodeID not found
}

bool CBusSystemIndexer::RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<CBusSystem::SRoute>> &routes) const noexcept {
    for (std::size_t i = 0; i < RouteCount(); ++i) {
        auto route = DImplementation->BusSystem->RouteByIndex(i);
        if (route) {
            const auto stopCount = route->StopCount();
            for (std::size_t j = 0; j < stopCount; ++j) {
                auto stopID = route->GetStopID(j);
                auto stop = DImplementation->BusSystem->StopByID(stopID);
                if (stop && stop->NodeID() == src) {
                    for (std::size_t k = j + 1; k < stopCount; ++k) {
                        stopID = route->GetStopID(k);
                        stop = DImplementation->BusSystem->StopByID(stopID);
                        if (stop && stop->NodeID() == dest) {
                            routes.insert(route);
                            break; //found a route, no need to check further
                        }
                    }
                    break; //stop found, no need to check other stops in the route
                }
            }
        }
    }

    return !routes.empty();
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept {
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