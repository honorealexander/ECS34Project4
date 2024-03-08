#ifndef STREETMAP_H
#define STREETMAP_H

#include "XMLEntity.h"
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <limits>
#include <vector>

class CStreetMap{
    public:
        using TNodeID = uint64_t;
        using TWayID = uint64_t;
        using TLocation = std::pair<double, double>;

        static const TNodeID InvalidNodeID = std::numeric_limits<TNodeID>::max();
        static const TWayID InvalidWayID = std::numeric_limits<TWayID>::max();


        struct SNode{
            TNodeID nodeID;
            CStreetMap::TLocation location;
            std::vector<SXMLEntity::TAttribute> DAttributes;

            virtual ~SNode(){};
            virtual TNodeID ID() const noexcept = 0;
            virtual TLocation Location() const noexcept = 0;
            virtual std::size_t AttributeCount() const noexcept = 0;
            virtual std::string GetAttributeKey(std::size_t index) const noexcept = 0;
            virtual bool HasAttribute(const std::string &key) const noexcept = 0;
            virtual std::string GetAttribute(const std::string &key) const noexcept = 0;
        };

        struct SWay{
            TWayID wayID;
            std::vector<TNodeID> nodes;
            std::vector<SXMLEntity::TAttribute> DAttributes;

            virtual ~SWay(){};
            virtual TWayID ID() const noexcept = 0;
            virtual std::size_t NodeCount() const noexcept = 0;
            virtual TNodeID GetNodeID(std::size_t index) const noexcept = 0;
            virtual std::size_t AttributeCount() const noexcept = 0;
            virtual std::string GetAttributeKey(std::size_t index) const noexcept = 0;
            virtual bool HasAttribute(const std::string &key) const noexcept = 0;
            virtual std::string GetAttribute(const std::string &key) const noexcept = 0;
        };

        virtual ~CStreetMap(){};

        virtual std::size_t NodeCount() const noexcept = 0;
        virtual std::size_t WayCount() const noexcept = 0;
        virtual std::shared_ptr<SNode> NodeByIndex(std::size_t index) const noexcept = 0;
        virtual std::shared_ptr<SNode> NodeByID(TNodeID id) const noexcept = 0;
        virtual std::shared_ptr<SWay> WayByIndex(std::size_t index) const noexcept = 0;
        virtual std::shared_ptr<SWay> WayByID(TWayID id) const noexcept = 0;
};

#endif