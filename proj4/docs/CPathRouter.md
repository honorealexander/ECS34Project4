
# CPathRouter

The CPathRouter class helps us implement a graph representing bus routes and street maps as vertices and nodes. We can then use this representation to try to find the shortest path from one node to another or one street to another. We use TVertexID to represent std::size_t in this class. All members are public.

We have two constants in this class, InvalidVertexID and NoPathExists. The former is used when we have an invalid vertex ID or if it doesn't exist. The latter also is used to say that a path between two vertices doesn't exist. It is set to the maximum of 'double' to signify that no path exists. InvalidVertex is also set to the maximum to signify its absence.

## Destructor

~CPathRouter(){}; Is the destructor and is used to do cleanup when objects of the derived classes are destroyed.

## Member Functions

std::size_t VertexCount() const noexcept

This function simply returns the number of vertices within a graph. In the case of our BusSystem, it would represent StopIDs and for OpenStreetMap, it could represent intersections. The edges between these stops represent the path, route or street.






class CPathRouter{
    public:
        using TVertexID = std::size_t;

        static constexpr TVertexID InvalidVertexID = std::numeric_limits<TVertexID>::max();
        static constexpr double NoPathExists = std::numeric_limits<double>::max();

        virtual ~CPathRouter(){};

        virtual std::size_t VertexCount() const noexcept = 0;
        virtual TVertexID AddVertex(std::any tag) noexcept = 0;
        virtual std::any GetVertexTag(TVertexID id) const noexcept = 0;
        virtual bool AddEdge(TVertexID src, TVertexID dest, double weight, bool bidir = false) noexcept = 0;
        virtual bool Precompute(std::chrono::steady_clock::time_point deadline) noexcept = 0;
        virtual double FindShortestPath(TVertexID src, TVertexID dest, std::vector<TVertexID> &path) noexcept = 0;
};