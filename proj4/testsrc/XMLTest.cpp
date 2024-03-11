#include <gtest/gtest.h>
#include "XMLReader.h"
#include "StringDataSource.h"
#include "StringDataSink.h"
#include "XMLEntity.h"
#include "XMLWriter.h"


TEST(XMLReader, ReadEntity) {
    std::string xmlData = "<root><element1 attribute=\"value\">Hello</element1><element2>World</element2></root>";
    std::shared_ptr<CStringDataSource> dataSource = std::make_shared<CStringDataSource>(xmlData);
    CXMLReader xmlReader(dataSource);
    SXMLEntity entity;
    bool success = xmlReader.ReadEntity(entity, false);
    ASSERT_TRUE(success);
    ASSERT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    ASSERT_EQ(entity.DNameData, "root");
}
TEST(XMLReader, ReadSingleElement) {
    std::string xmlData = "<book><title>Harry Potter</title></book>";
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>(xmlData);
    CXMLReader reader(source);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "book");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "title");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "Harry Potter");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "title");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "book");
}

TEST(XMLReader, ReadNestedElements) {
    std::string xmlData = "<book><title>Harry Potter</title><author>J.K. Rowling</author></book>";
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>(xmlData);
    CXMLReader reader(source);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "book");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "title");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "Harry Potter");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "title");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "author");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "J.K. Rowling");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "author");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "book");
}

TEST(XMLReader, ReadEmptyElement) {
    std::string xmlData = "<empty></empty>";
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>(xmlData);
    CXMLReader reader(source);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "empty");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "empty");
}

TEST(XMLReader, ReadCDATASection) {
    std::string xmlData = "<description><![CDATA[This is a <b>bold</b> statement]]></description>";
    std::shared_ptr<CStringDataSource> source = std::make_shared<CStringDataSource>(xmlData);
    CXMLReader reader(source);

    SXMLEntity entity;
    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::StartElement);
    EXPECT_EQ(entity.DNameData, "description");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::CharData);
    EXPECT_EQ(entity.DNameData, "This is a <b>bold</b> statement");

    ASSERT_TRUE(reader.ReadEntity(entity, false));
    EXPECT_EQ(entity.DType, SXMLEntity::EType::EndElement);
    EXPECT_EQ(entity.DNameData, "description");
}

TEST(XMLWriter, WriteEntity) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CXMLWriter xmlWriter(sink);
    SXMLEntity entity;
    entity.DType = SXMLEntity::EType::StartElement;
    entity.DNameData = "element";
    bool success = xmlWriter.WriteEntity(entity);
    ASSERT_TRUE(success);
    ASSERT_EQ(sink->String(), "<element>");
} 


TEST(XMLWriter, WriteTwoEntities) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CXMLWriter xmlWriter(sink);
    
    SXMLEntity entity1;
    entity1.DType = SXMLEntity::EType::StartElement;
    entity1.DNameData = "element1";
    ASSERT_TRUE(xmlWriter.WriteEntity(entity1));

    SXMLEntity entity2;
    entity2.DType = SXMLEntity::EType::StartElement;
    entity2.DNameData = "element2";
    ASSERT_TRUE(xmlWriter.WriteEntity(entity2));

    std::string expectedOutput = "<element1><element2>";
    ASSERT_EQ(sink->String(), expectedOutput);
}

TEST(XMLWriter, WriteEmptyEntity) {
    std::shared_ptr<CStringDataSink> sink = std::make_shared<CStringDataSink>();
    CXMLWriter xmlWriter(sink);
    SXMLEntity emptyEntity;
    emptyEntity.DType = SXMLEntity::EType::StartElement;
    emptyEntity.DNameData = "";
    bool success = xmlWriter.WriteEntity(emptyEntity);
    ASSERT_TRUE(success);
    ASSERT_EQ(sink->String(), "<>");
}

