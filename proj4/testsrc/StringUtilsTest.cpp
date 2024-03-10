#include <gtest/gtest.h>
#include "StringUtils.h"

TEST(StringUtilsTest, SliceTest){
    std::string Base = "Hello";
    EXPECT_EQ(StringUtils::Slice(Base, 0, 5), "Hello");
    EXPECT_EQ(StringUtils::Slice(Base, 1, 5), "ello");
}

TEST(StringUtilsTest, Capitalize){
    std::string Base = "hello";
    EXPECT_EQ(StringUtils::Capitalize(Base), "Hello");
    EXPECT_EQ(StringUtils::Capitalize(std::string()), "");
    EXPECT_EQ(StringUtils::Capitalize("HELLO"), "Hello");
    EXPECT_EQ(StringUtils::Capitalize("    \t\r\n;*^73"), "    \t\r\n;*^73");
}

TEST(StringUtilsTest, Upper){
    std::string Base = "hello";
    EXPECT_EQ(StringUtils::Upper(Base), "HELLO");
    EXPECT_EQ(StringUtils::Upper("Hi, there!"), "HI, THERE!");
}

TEST(StringUtilsTest, Lower){
    std::string Base = "Hello";
    EXPECT_EQ(StringUtils::Lower(Base), "hello");
    EXPECT_EQ(StringUtils::Lower("Hi, There!"), "hi, there!");
    
}

TEST(StringUtilsTest, LStrip){
    std::string Base = "    Hi! ";
    EXPECT_EQ(StringUtils::LStrip(Base), "Hi! ");
    EXPECT_EQ(StringUtils::LStrip("/   Banana"), "/   Banana");
    EXPECT_EQ(StringUtils::LStrip("   .HiHi"), ".HiHi");
}

TEST(StringUtilsTest, RStrip){
    std::string Base = " Hey There!   ";
    EXPECT_EQ(StringUtils::RStrip(Base), " Hey There!");
    EXPECT_EQ(StringUtils::RStrip("   Hi!"), "   Hi!");
    EXPECT_EQ(StringUtils::RStrip("   3429u3wfes          "), "   3429u3wfes");
}

TEST(StringUtilsTest, Strip){
    std::string Base = " Hey There!   ";
    EXPECT_EQ(StringUtils::Strip(Base), "Hey There!");
    EXPECT_EQ(StringUtils::Strip("    H"), "H");
    EXPECT_EQ(StringUtils::Strip(" 3242342         "), "3242342");
}

TEST(StringUtilsTest, Center){
    std::string Base = "Hey";
    EXPECT_EQ(StringUtils::Center(Base, 5, ' '), " Hey ");
    EXPECT_EQ(StringUtils::Center(" Hi, There", 30, 'x'), "xxxxxxxxxx Hi, Therexxxxxxxxxx");
    EXPECT_EQ(StringUtils::Center("    Multiple    Spaces    ", 30, '*'), "**    Multiple    Spaces    **");
}

TEST(StringUtilsTest, LJust){
    std::string Base = "Hey";
    EXPECT_EQ(StringUtils::LJust(Base, 6, ' '), "Hey   ");
    EXPECT_EQ(StringUtils::LJust(" WORDS", 12, '-'), " WORDS------");
}

TEST(StringUtilsTest, RJust){
    std::string Base = "Hey";
    EXPECT_EQ(StringUtils::RJust(Base, 6, ' '), "   Hey");
    EXPECT_EQ(StringUtils::RJust(" WORDS", 12, '-'), "------ WORDS");
}

TEST(StringUtilsTest, Replace){
    std::string Base = "Replace this text";
    EXPECT_EQ(StringUtils::Replace(Base, "this", "that"), "Replace that text");
}

TEST(StringUtilsTest, Split){
    std::string Base = "Hi my sweet girl";
    std::vector<std::string> answer = {"Hi", "my", "sweet", "girl"};
    EXPECT_EQ(StringUtils::Split(Base, " "), answer);
}

TEST(StringUtilsTest, Join){
    std::vector<std::string> Base = {"Hi", "my", "sweet", "girl"};
    EXPECT_EQ(StringUtils::Join("#", Base), "Hi#my#sweet#girl");
}

TEST(StringUtilsTest, ExpandTabs){
    std::string Base = "\tThank\tGoodness";
    EXPECT_EQ(StringUtils::ExpandTabs(Base, 1), " Thank Goodness");
    EXPECT_EQ(StringUtils::ExpandTabs("\tTabs\tAre\tFun", 2), "  Tabs  Are Fun");
}

TEST(StringUtilsTest, EditDistance){
    EXPECT_EQ(StringUtils::EditDistance("sunday", "saturday", true), 3);
    EXPECT_EQ(StringUtils::EditDistance("kitten", "sitting", true), 3);
    EXPECT_EQ(StringUtils::EditDistance("Hello", "ama", true), 5);
    EXPECT_EQ(StringUtils::EditDistance("Hello", "hello", false), 1);
    EXPECT_EQ(StringUtils::EditDistance("Hello", "hello", true), 0);
}
