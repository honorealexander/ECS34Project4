#include "BusSystemIndexer.h"
#include <memory>

struct CBusSystemIndexer::SImplementation : public CBusSystem{
    std::shared_ptr<CBusSystem> BusSystem;

    SImplementation(std::shared_ptr<CBusSystem> bussystem) : BusSystem(bussystem) {}

    std::size_t StopCount() const noexcept {
        return 0;
    }

    std::size_t RouteCount() const noexcept {
        return 0;
    }

    std::shared_ptr<CBusSystem::SStop> SortedStopByIndex(std::size_t index) const noexcept {
        // Implement SortedStopByIndex here
        return nullptr;
    }

    std::shared_ptr<CBusSystem::SRoute> SortedRouteByIndex(std::size_t index) const noexcept {
        // Implement SortedRouteByIndex here
        return nullptr;
    }

    std::shared_ptr<CBusSystem::SStop> StopByNodeID(TNodeID id) const noexcept {
        // Implement StopByNodeID here
        return nullptr;
    }

    bool RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<CBusSystem::SRoute>>& routes) const noexcept {
        // Implement RoutesByNodeIDs here
        return true;
    }

    bool RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept {
        // Implement RouteBetweenNodeIDs here
        return true;
    }
};

CBusSystemIndexer::CBusSystemIndexer(std::shared_ptr<CBusSystem> bussystem)
    : DImplementation(std::make_unique<SImplementation>(bussystem)) {}

CBusSystemIndexer::~CBusSystemIndexer() = default;

std::size_t CBusSystemIndexer::StopCount() const noexcept {
    return DImplementation->BusSystem->StopCount();
}

std::size_t CBusSystemIndexer::RouteCount() const noexcept {
    return DImplementation->BusSystem->RouteCount();
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::SortedStopByIndex(std::size_t index) const noexcept {
    // Implement SortedStopByIndex here
    return nullptr;
}

std::shared_ptr<CBusSystem::SRoute> CBusSystemIndexer::SortedRouteByIndex(std::size_t index) const noexcept {
    // Implement SortedRouteByIndex here
    return nullptr;
}

std::shared_ptr<CBusSystem::SStop> CBusSystemIndexer::StopByNodeID(TNodeID id) const noexcept {
    // Implement StopByNodeID here
    return nullptr;
}

bool CBusSystemIndexer::RoutesByNodeIDs(TNodeID src, TNodeID dest, std::unordered_set<std::shared_ptr<CBusSystem::SRoute>>& routes) const noexcept {
    // Implement RoutesByNodeIDs here
    return true;
}

bool CBusSystemIndexer::RouteBetweenNodeIDs(TNodeID src, TNodeID dest) const noexcept {
    // Implement RouteBetweenNodeIDs here
    return true;
}
