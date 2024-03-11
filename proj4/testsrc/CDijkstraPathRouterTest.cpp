#include <gtest/gtest.h>

TEST(AlwaysPassingTest, TestAlwaysPasses) {
    // This test always passes because the condition is always true
    EXPECT_TRUE(true);
}