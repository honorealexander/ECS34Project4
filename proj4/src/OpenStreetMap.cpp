#include "OpenStreetMap.h"
#include "XMLReader.h"
#include "XMLEntity.h"
#include "StreetMap.h"
#include "DSVReader.h"
#include "DSVWriter.h"
#include <iostream>
#include <memory>
#include <map>
#include <unordered_map>
#include <cstddef>
#include <algorithm>
#include <expat.h>

//define a struct named streetmap derived from COpenStreetMap::SNode
struct streetmap : public COpenStreetMap::SNode {
public:
    using TNodeID = CStreetMap::TNodeID; //define TNodeID as CStreetMap::TNodeID

    void SetAttribute(const std::string& key, const std::string& value) {
        DAttributes.emplace_back(key, value);
    }

    TNodeID ID() const noexcept override { return nodeID; } 
    CStreetMap::TLocation Location() const noexcept override { return std::make_pair(0.0, 0.0); } // Make location pair
    std::size_t AttributeCount() const noexcept override { return DAttributes.size(); } 
    std::string GetAttributeKey(std::size_t index) const noexcept override { 
        auto it = std::next(DAttributes.begin(), index);
        return (it != DAttributes.end()) ? it->first : "";
    }
    bool HasAttribute(const std::string &key) const noexcept {
        auto it = std::find_if(DAttributes.begin(), DAttributes.end(),
                            [&](const std::pair<std::string, std::string> &attribute) {
                                return attribute.first == key;
                            });

        return it != DAttributes.end();
    }
    std::string GetAttribute(const std::string &key) const noexcept {
        auto it = std::find_if(DAttributes.begin(), DAttributes.end(),
                            [&](const std::pair<std::string, std::string> &attribute) {
                                return attribute.first == key;
                            });

        return (it != DAttributes.end()) ? it->second : "";
    }
};

//define newstreetmap derived from COpenStreetMap::SWay
class newstreetmap : public COpenStreetMap::SWay {
public:
    using TNodeID = CStreetMap::TNodeID; //define TNodeID as CStreetMap::TNodeID
    using TWayID = CStreetMap::TWayID; //define TWayID as CStreetMap::TWayID

    void SetAttribute(const std::string& key, const std::string& value) {
        DAttributes.emplace_back(key, value);
    }

    TWayID ID() const noexcept override { return wayID; }
    std::size_t NodeCount() const noexcept override { return nodes.size(); } //return how many nodes there are
    TNodeID GetNodeID(std::size_t index) const noexcept override {
        return (index < nodes.size()) ? nodes[index] : CStreetMap::InvalidNodeID; //retrieve ID from index
    }
    std::size_t AttributeCount() const noexcept override { return DAttributes.size(); } //amount of attributes
    std::string GetAttributeKey(std::size_t index) const noexcept override {
        auto it = std::next(DAttributes.begin(), index);
        return (it != DAttributes.end()) ? it->first : "";
    }
    bool HasAttribute(const std::string &key) const noexcept {
        auto it = std::find_if(DAttributes.begin(), DAttributes.end(),
                            [&](const std::pair<std::string, std::string> &attribute) {
                                return attribute.first == key;
                            });

        return it != DAttributes.end();
    }
    std::string GetAttribute(const std::string &key) const noexcept {
        auto it = std::find_if(DAttributes.begin(), DAttributes.end(),
                            [&](const std::pair<std::string, std::string> &attribute) {
                                return attribute.first == key;
                            });

        return (it != DAttributes.end()) ? it->second : "";
    }
};


struct COpenStreetMap::SImplementation {
    std::map<TNodeID, std::shared_ptr<SNode>> nodes; //vector storing shared pointers to SNode
    std::map<TWayID, std::shared_ptr<SWay>> ways; // ector storing shared pointers to SWay
    XML_Parser DXMLParser;
};

//constructor
COpenStreetMap::COpenStreetMap(std::shared_ptr<CXMLReader> src) {
    DImplementation = std::make_unique<SImplementation>();
    DImplementation->DXMLParser = XML_ParserCreate(nullptr); // Initialize XML_Parser

    SXMLEntity entity;
    while (src->ReadEntity(entity, false)) {
        if (entity.DType == SXMLEntity::EType::StartElement) {
            if (entity.DNameData == "node") {
                // parse node attributes and create streetmap object
                auto newNode = std::make_shared<streetmap>();
                // set attributes for the new node
                auto nodeIDAttribute = std::find_if(entity.DAttributes.begin(), entity.DAttributes.end(),
                                    [](const auto& attribute) {
                                        return attribute.first == "id";
                                    });
            if (nodeIDAttribute != entity.DAttributes.end()) {
                newNode->SetAttribute("id", nodeIDAttribute->second);
            }
                // add the new node to the nodes map
                DImplementation->nodes[newNode->ID()] = newNode;
            } else if (entity.DNameData == "way") {
                // parse way attributes and create newstreetmap object
                auto newWay = std::make_shared<newstreetmap>();
                // set attributes for the new way
                auto wayIDAttribute = std::find_if(entity.DAttributes.begin(), entity.DAttributes.end(),
                                        [](const auto& attribute) {
                                            return attribute.first == "id";
                                        });
                if (wayIDAttribute != entity.DAttributes.end()) {
                    newWay->SetAttribute("id", wayIDAttribute->second);
                }
                // add the new way to the ways map
                DImplementation->ways[newWay->ID()] = newWay;
            }
        }
    }
}

//destructor
COpenStreetMap::~COpenStreetMap() {
    //ensure proper cleanup... automatically called
    if (DImplementation) {
        XML_ParserFree(DImplementation->DXMLParser);
    }
}


std::size_t COpenStreetMap::NodeCount() const noexcept {
    if (DImplementation) {
        return DImplementation->nodes.size(); //returns the number of nodes in the map
    } else {
        return 0;
    }
}


std::size_t COpenStreetMap::WayCount() const noexcept {
    if (DImplementation) {
        return DImplementation->ways.size(); //returns the number of ways in the map
    } else {
        return 0;
    }
}

/*returns the SNode associated with index, 
eturns nullptr if index is larger than or equal to NodeCount()*/
std::shared_ptr<COpenStreetMap::SNode> COpenStreetMap::NodeByIndex(std::size_t index) const noexcept {
    auto it = DImplementation->nodes.begin();
    std::advance(it, index);
    if (it != DImplementation->nodes.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

std::shared_ptr<COpenStreetMap::SNode> COpenStreetMap::NodeByID(TNodeID id) const noexcept {
    auto it = DImplementation->nodes.find(id);
    if (it != DImplementation->nodes.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

std::shared_ptr<COpenStreetMap::SWay> COpenStreetMap::WayByIndex(std::size_t index) const noexcept {
    auto it = DImplementation->ways.begin();
    std::advance(it, index);
    if (it != DImplementation->ways.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

std::shared_ptr<COpenStreetMap::SWay> COpenStreetMap::WayByID(TWayID id) const noexcept {
    auto it = DImplementation->ways.find(id);
    if (it != DImplementation->ways.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}

//COpenStreetMap::SNode member functions (AttributeCount, GetAttributeKey, HasAttribute, GetAttribute, etc.)
std::size_t COpenStreetMap::SNode::AttributeCount() const noexcept {
    return DAttributes.size(); //pull size aka amount of attributes
}

std::string COpenStreetMap::SNode::GetAttributeKey(std::size_t index) const noexcept {
    if (index < DAttributes.size()) {
        return DAttributes[index].first; //pulls attrivute key form index
    } else {
        return "";
    }
}

bool COpenStreetMap::SNode::HasAttribute(const std::string &key) const noexcept {
    for (const auto &attribute : DAttributes) {
        if (attribute.first == key) { //confirm if there is an attrivute
            return true;
        }
    }
    return false;
}

std::string COpenStreetMap::SNode::GetAttribute(const std::string &key) const noexcept {
    for (const auto &attribute : DAttributes) {
        if (attribute.first == key) { //pull atrrivute
            return attribute.second;
        }
    }
    return "";
}

//COpenStreetMap::SWay member functions (AttributeCount, GetAttributeKey, HasAttribute, GetAttribute, etc.)
std::size_t COpenStreetMap::SWay::AttributeCount() const noexcept {
    return DAttributes.size(); //amount of attributes (ways)
}

std::string COpenStreetMap::SWay::GetAttributeKey(std::size_t index) const noexcept {
    if (index < DAttributes.size()) {
        return DAttributes[index].first; //pull attribute key for ways
    } else {
        return "";
    }
}

bool COpenStreetMap::SWay::HasAttribute(const std::string &key) const noexcept {
    for (const auto &attribute : DAttributes) {
        if (attribute.first == key) { //confirm if way has attribute
            return true;
        }
    }
    return false;
}

std::string COpenStreetMap::SWay::GetAttribute(const std::string &key) const noexcept {
    for (const auto &attribute : DAttributes) {
        if (attribute.first == key) { //pull way attribute
            return attribute.second;
        }
    }
    return "";
}