#include <gtest/gtest.h>
#include "DijkstraPathRouter.h"

using TVertexID = std::size_t; 
TEST(CDijkstraPathRouterTest, VertexCount) { //PASS
    CDijkstraPathRouter router;
    EXPECT_EQ(0, router.VertexCount());

    router.AddVertex(std::any());
    EXPECT_EQ(1, router.VertexCount());


}

TEST(CDijkstraPathRouterTest, GetVertexTag) {
    CDijkstraPathRouter router;
    std::any tag1 = std::string("Tag1");  // Use std::string here
    std::any tag2 = 123;  // Use int here

    auto vertex1 = router.AddVertex(tag1);
    auto vertex2 = router.AddVertex(tag2);

    try {
        EXPECT_EQ(std::any_cast<std::string>(tag1), std::any_cast<std::string>(router.GetVertexTag(vertex1)));
    } catch (const std::bad_any_cast& e) {
        FAIL() << "Exception thrown: " << e.what();
    }

    try {
        EXPECT_EQ(std::any_cast<int>(tag2), std::any_cast<int>(router.GetVertexTag(vertex2)));
    } catch (const std::bad_any_cast& e) {
        FAIL() << "Exception thrown: " << e.what();
    }

    // Add more tests for vertex tags
}
TEST(CDijkstraPathRouterTest, AddEdge) {
    CDijkstraPathRouter router;
    auto vertex1 = router.AddVertex(std::any());
    auto vertex2 = router.AddVertex(std::any());

    EXPECT_TRUE(router.AddEdge(vertex1, vertex2, 5.0, true));
    EXPECT_FALSE(router.AddEdge(vertex1, vertex2, -2.0, true)); // Negative weight should fail

    // Add more tests for adding edges
}

TEST(CDijkstraPathRouterTest, FindShortestPath) {
    CDijkstraPathRouter router;
    auto vertex1 = router.AddVertex(std::any());
    auto vertex2 = router.AddVertex(std::any());
    auto vertex3 = router.AddVertex(std::any());
    auto vertex4 = router.AddVertex(std::any());

    router.AddEdge(vertex1, vertex2, 1.0, true);
    router.AddEdge(vertex1, vertex3, 3.0, true);
    router.AddEdge(vertex2, vertex4, 2.0, true);
    router.AddEdge(vertex3, vertex4, 1.0, true);

    std::vector<CDijkstraPathRouter::TVertexID> path;
    double distance = router.FindShortestPath(vertex1, vertex4, path);

    EXPECT_DOUBLE_EQ(3.0, distance);
    EXPECT_EQ(3, path.size());

}

TEST(CDijkstraPathRouterTest, Precompute) {
    CDijkstraPathRouter router;

    // Add vertices and edges to the router (modify as needed)
    CDijkstraPathRouter::TVertexID vertexA = router.AddVertex(std::any());
    CDijkstraPathRouter::TVertexID vertexB = router.AddVertex(std::any());
    CDijkstraPathRouter::TVertexID vertexC = router.AddVertex(std::any());

    router.AddEdge(vertexA, vertexB, 5.0, true);
    router.AddEdge(vertexB, vertexC, 2.0, true);

    // Precompute the shortest paths
    EXPECT_TRUE(router.Precompute(std::chrono::steady_clock::now()));

    // Add more vertices and edges (modify as needed)
    CDijkstraPathRouter::TVertexID vertexD = router.AddVertex(std::any());
    CDijkstraPathRouter::TVertexID vertexE = router.AddVertex(std::any());

    router.AddEdge(vertexC, vertexD, 3.0, true);
    router.AddEdge(vertexD, vertexE, 4.0, true);

    // Precompute again after adding more edges
    EXPECT_TRUE(router.Precompute(std::chrono::steady_clock::now()));

    // Test cases to verify the precomputed information (modify as needed)
    std::vector<CDijkstraPathRouter::TVertexID> path;
    double distance;

    // Test shortest path from vertexA to vertexE
    distance = router.FindShortestPath(vertexA, vertexE, path);
    EXPECT_EQ(distance, 14.0);
    EXPECT_EQ(path.size(), 5);
    EXPECT_EQ(path[0], vertexA);
    EXPECT_EQ(path[1], vertexB);
    EXPECT_EQ(path[2], vertexC);
    
    // Add more test cases based on your specific scenarios
}

