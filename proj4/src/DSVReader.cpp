#include "DSVReader.h"
#include <sstream>
#include <gtest/gtest.h>

struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> DataSource;
    char Delimiter;

    SImplementation(std::shared_ptr<CDataSource> src, char delimiter)
        : DataSource(src), Delimiter(delimiter) {} //

    ~SImplementation() { //do cleanup if required
    }
};

CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)
    : DImplementation(std::make_unique<SImplementation>(src, delimiter)) {}

CDSVReader::~CDSVReader() = default;

bool CDSVReader::End() const {
    return DImplementation->DataSource->End();
}

bool CDSVReader::ReadRow(std::vector<std::string>& row) {
    row.clear(); //makes sure row is clear before trying to put more data into it for the next row

    char ch;
    std::string column;

    while (DImplementation->DataSource->Get(ch)) {
        if (ch == DImplementation->Delimiter) { //add column to row if delimeter is found
            row.push_back(column);
            column.clear();
        } else if (ch == '\n' || ch == '\r') { //add the last column  then break if new line is found
            row.push_back(column);
            break;
        } else { //push character to the column
            column += ch;
        }
    }
    return !row.empty(); //returns true to confirm that at least one row was read
}