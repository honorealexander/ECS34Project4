#include "BusSystemIndexer.h"
#include <memory>
#include <unordered_set>
#include <algorithm>

struct CBusSystemIndexer::SImplementation {
    std::shared_ptr<CBusSystem> BusSystem;
    // Additional private members as needed for implementation

    SImplementation(std::shared_ptr<CBusSystem> bussystem) : BusSystem(bussystem) {
        // Initialize additional members if needed
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
    // Implement logic to find stop by NodeID
    // For now, returning the stop with the specified NodeID
    for (std::size_t i = 0; i < DImplementation->BusSystem->StopCount(); ++i) {
        auto stop = DImplementation->BusSystem->StopByIndex(i);
        if (stop && stop->NodeID() == id) {
            return stop;
        }
    }
    return nullptr;
}

bool CBusSystemIndexer::RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<CBusSystem::SRoute>> &routes) const noexcept {
    // Implement logic to find routes between NodeIDs
    // For now, returning false (no routes found)
    return false;
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept {
    // Implement logic to check if there is a route between NodeIDs
    // For now, returning false (no route between NodeIDs)
    return false;
}