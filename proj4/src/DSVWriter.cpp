#include "DSVWriter.h"
#include <sstream>

struct CDSVWriter::SImplementation {
    std::shared_ptr<CDataSink> sink;
    char delimeter;
    bool quoteall;
    bool firstRow = true;

    SImplementation(std::shared_ptr<CDataSink> sink, char delimeter, bool quoteall)
        : sink(sink), delimeter(delimeter), quoteall(quoteall) {}

    ~SImplementation() { 
    }
};

CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimeter, bool quoteall)
    : DImplementation(std::make_unique<SImplementation>(sink, delimeter, quoteall)) {}

CDSVWriter::~CDSVWriter() = default;

bool CDSVWriter::WriteRow(const std::vector<std::string>& row) {
    if (!DImplementation->sink) {
        return false;
    } 

    if (!DImplementation->firstRow) {
        DImplementation->sink->Put('\n'); 
    } else {
        DImplementation->firstRow = false; 
    }

    bool firstColumn = true;

    for (const std::string& cell : row) {
        if (!firstColumn) {
            if (DImplementation->delimeter == '"') { //checking to see if delimeter is " the delimeter interpret it as a comma according to guidelines
                DImplementation->sink->Put(','); 
            } else {
                DImplementation->sink->Put(DImplementation->delimeter);
            }
        }

        if (cell.find(DImplementation->delimeter) != std::string::npos || // check that we are still looking for things to be put in double quotes
            cell.find('"') != std::string::npos || //these are all according to the guidelines on Project2 doc
            cell.find('\n') != std::string::npos ||
            DImplementation->quoteall) {
            DImplementation->sink->Put('"');

            // Replace double quote characters with two double quotes
            // got this section from ChatGPT (Changes 4 in ReadME)
            for (char c : cell) {
                if (c == '"') {
                    DImplementation->sink->Put('"');
                    DImplementation->sink->Put('"'); 
                } else {
                    DImplementation->sink->Put(c);
                }
            }

            DImplementation->sink->Put('"');
        } else {
            for (char c : cell) {
                DImplementation->sink->Put(c);
            }
        }

        firstColumn = false;
    }

    return true;

}


