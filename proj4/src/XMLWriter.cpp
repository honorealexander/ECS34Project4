#include "XMLWriter.h"
#include "XMLEntity.h"
#include <expat.h>
#include "DataSink.h"

struct CXMLWriter::SImplementation {
    XML_Parser parser;
    std::shared_ptr<CDataSink> sink;

    SImplementation(std::shared_ptr<CDataSink> sink) : sink(sink) {
        parser = XML_ParserCreate(NULL);
        XML_SetUserData(parser, this);
    }

    ~SImplementation() {
        XML_ParserFree(parser);
    }

    bool Flush() {
        if (XML_Parse(parser, nullptr, 0, true)) { //line generated from Chat, noted in ReadME Changes 9
            return true;
        } else {
            return false;
        }
    }

    bool WriteEntity(const SXMLEntity& entity) {
        std::string startTag = "<" + entity.DNameData + ">"; //constructing tags for XML entities
        std::string endTag = "</" + entity.DNameData + ">";
        std::string cdata = entity.DNameData;

        switch (entity.DType) { //skeleton from Chat, Changes 10 cited in ReadME
            case SXMLEntity::EType::StartElement:
                if (!sink->Write(std::vector<char>(startTag.begin(), startTag.end()))) { //writes start tag to sink
                    return false;
                }
                break;
            case SXMLEntity::EType::EndElement:
                if (!sink->Write(std::vector<char>(endTag.begin(), endTag.end()))) { //writes element tag to sink
                    return false;
                }
                break;
            case SXMLEntity::EType::CharData:
                if (!sink->Write(std::vector<char>(cdata.begin(), cdata.end()))) {
                    return false;
                }
                break;
            default:
                break;
        }

        return true; //default answer is true, false only returned if there is a writing failure
    }

};

CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink) {
    DImplementation = std::make_unique<SImplementation>(sink);
}

CXMLWriter::~CXMLWriter() = default;

bool CXMLWriter::Flush() {
    return DImplementation->Flush();
}

bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    return DImplementation->WriteEntity(entity);
}
