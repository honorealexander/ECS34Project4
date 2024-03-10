#include <gtest/gtest.h>
#include "DSVWriter.h"
#include "DSVReader.h"
#include "StringDataSource.h"
#include "StringDataSink.h"

TEST(DSVReader, ReadSingleColumnRow) {
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>("Hello\n");
    CDSVReader reader(source, ',');

    std::vector<std::string> expected = {"Hello"};
    std::vector<std::string> row;

    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row, expected);
}

TEST(DSVReader, Read2Rows) {
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>("Hey*there\nHello*world");
    CDSVReader reader(source, '*');

    std::vector<std::string> expected1 = {"Hey", "there"};
    std::vector<std::string> expected2 = {"Hello", "world"};

    std::vector<std::string> row1;
    std::vector<std::string> row2;

    EXPECT_TRUE(reader.ReadRow(row1));
    EXPECT_TRUE(reader.ReadRow(row2));
}


TEST(DSVWriter, WriteRow1) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, '*');

    std::vector<std::string> input1 = {"Hey", "there"};
    std::vector<std::string> input2 = {"Hello", "world"};

    EXPECT_TRUE(writer.WriteRow(input1));
    EXPECT_TRUE(writer.WriteRow(input2));
    EXPECT_EQ(sink->String(), "Hey*there\nHello*world");
}

TEST(DSVReader, ReadMultipleRowsDifferentDelimiters) {
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>("Hey,there\nHello|world\nHow|are|you");
    CDSVReader reader1(source, ',');
    CDSVReader reader2(source, '|');

    std::vector<std::string> expected1 = {"Hey", "there"};
    std::vector<std::string> expected2 = {"Hello", "world"};
    std::vector<std::string> expected3 = {"How", "are", "you"};

    std::vector<std::string> row1, row2, row3;

    EXPECT_TRUE(reader1.ReadRow(row1));
    EXPECT_TRUE(reader2.ReadRow(row2));
    EXPECT_TRUE(reader2.ReadRow(row3));
}

TEST(DSVWriter, WriteRowWithEmptyColumns) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');

    std::vector<std::string> input1 = {"Hey", "", "there"};
    std::vector<std::string> input2 = {"Hello", "", "world"};
    std::vector<std::string> input3 = {"How", "are", "", "you"};

    EXPECT_TRUE(writer.WriteRow(input1));
    EXPECT_TRUE(writer.WriteRow(input2));
    EXPECT_TRUE(writer.WriteRow(input3));

    EXPECT_EQ(sink->String(), "Hey,,there\nHello,,world\nHow,are,,you");
}

TEST(DSVReader, ReadEmptyRow) {
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>("\n");
    CDSVReader reader(source, ',');

    std::vector<std::string> expected = {""};
    std::vector<std::string> row;

    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row, expected);
}

TEST(DSVReader, ReadMultipleColumnsRow) {
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>("One,Two,Three\n");
    CDSVReader reader(source, ',');

    std::vector<std::string> expected = {"One", "Two", "Three"};
    std::vector<std::string> row;

    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row, expected);
}

TEST(DSVReader, ReadMultipleRows) {
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>("1,2,3\n4,5,6\n7,8,9\n");
    CDSVReader reader(source, ',');

    std::vector<std::vector<std::string>> expected = {{"1", "2", "3"}, {"4", "5", "6"}, {"7", "8", "9"}};
    std::vector<std::string> row;

    for (const auto& exp : expected) {
        EXPECT_TRUE(reader.ReadRow(row));
        EXPECT_EQ(row, exp);
    }
}

/*TEST(DSVReader, ReadRowWithLeadingAndTrailingDelimiters) {
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>(",Hey,there,");
    CDSVReader reader(source, ',');
    std::vector<std::string> expected = {"", "Hey", "there", ""};
    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row, expected);
}*/

TEST(DSVWriter, WriteSingleRow) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');

    std::vector<std::string> row = {"Hello", "World"};

    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(sink->String(), "Hello,World");
}

TEST(DSVWriter, WriteMultipleRows) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');

    std::vector<std::string> row1 = {"1", "2", "3"};
    std::vector<std::string> row2 = {"4", "5", "6"};
    std::vector<std::string> row3 = {"7", "8", "9"};

    EXPECT_TRUE(writer.WriteRow(row1));
    EXPECT_TRUE(writer.WriteRow(row2));
    EXPECT_TRUE(writer.WriteRow(row3));

    EXPECT_EQ(sink->String(), "1,2,3\n4,5,6\n7,8,9");
}

TEST(DSVWriter, WriteRowWithSpecialCharacters) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');

    std::vector<std::string> row = {"Special", "character\"with\"", "new\nline"};

    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(sink->String(), "Special,\"character\"\"with\"\"\",\"new\nline\"");
}

TEST(DSVWriter, WriteRowWithQuotingAll) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', true);

    std::vector<std::string> row = {"This, has, commas", "This\"has\"double\"quotes", "This\nhas\nnewlines"};

    EXPECT_TRUE(writer.WriteRow(row));
    EXPECT_EQ(sink->String(), "\"This, has, commas\",\"This\"\"has\"\"double\"\"quotes\",\"This\nhas\nnewlines\"");
}

TEST(DSVWriter, WriteSimpleRow) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');

    std::vector<std::string> input = {"Name", "Age", "City"};
    
    EXPECT_TRUE(writer.WriteRow(input));
    EXPECT_EQ(sink->String(), "Name,Age,City");
}

TEST(DSVWriter, WriteRowWithDoubleQuoteInValues) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');

    std::vector<std::string> input = {"John \"Doe\"", "35", "New \"York\""};
    
    EXPECT_TRUE(writer.WriteRow(input));
    EXPECT_EQ(sink->String(), "\"John \"\"Doe\"\"\",35,\"New \"\"York\"\"\"");
}

TEST(DSVWriter, WriteEmptyRow) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',');

    std::vector<std::string> input;
    
    EXPECT_TRUE(writer.WriteRow(input));
    EXPECT_EQ(sink->String(), "");
}
TEST(DSVWriter, WriteRowWithQuotedValues) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', true);
    std::vector<std::string> row = { "John,Doe", "35", "New York" };
    writer.WriteRow(row);
    EXPECT_EQ(sink->String(), "\"John,Doe\",\"35\",\"New York\"");
}

TEST(DSVWriter, WriteRowWithUnquotedValues) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', false);
    std::vector<std::string> row = { "John Doe", "35", "New York" };
    writer.WriteRow(row);
    EXPECT_EQ(sink->String(), "John Doe,35,New York");
}

TEST(DSVWriter, WriteRowWithQuoteAllEnabled) {
    auto sink = std::make_shared<CStringDataSink>();
    CDSVWriter writer(sink, ',', true);
    std::vector<std::string> row = { "John,Doe", "35", "New York" };
    writer.WriteRow(row);
    EXPECT_EQ(sink->String(), "\"John,Doe\",\"35\",\"New York\"");
}