#ifndef MULTI_GRAPH_H
#define MULTI_GRAPH_H

#include <vector>
#include <string>

struct GraphEdge
{
    std::string name;       // Name of the vertex
    float       weight[2];  // Weights of the edge
                            // (used on shortest path)
    int         endVertexIndex;
};

struct GraphVertex
{
    std::vector<GraphEdge> edges; // Adjacency List
    std::string            name;  // Name of the vertex
};

class MultiGraph
{
    private:
    std::vector<GraphVertex>    vertexList;

    static float Lerp(float w0, float w1, float alpha);

    protected:
    public:
    // Constructors & Destructor
                MultiGraph();
                MultiGraph(const std::string& filePath);

    // Connect Vertices
    void        InsertVertex(const std::string& vertexName);
    void        RemoveVertex(const std::string& vertexName);

    // Connect Vertices
    void        AddEdge(const std::string& edgeName,
                        const std::string& vertexFromName,
                        const std::string& vertexToName,
                        float weight0, float weight1);
    void        RemoveEdge(const std::string& edgeName,
                           const std::string& vertexFromName,
                           const std::string& vertexToName);
    // Shortest Path Functions
    bool        HeuristicShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                      const std::string& vertexNameFrom,
                                      const std::string& vertexNameTo,
                                      float heuristicWeight) const;
    bool        FilteredShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                     const std::string& vertexNameFrom,
                                     const std::string& vertexNameTo,
                                     float heuristicWeight,
                                     const std::vector<std::string>& edgeNames) const;

    // Other functions

    int         BiDirectionalEdgeCount() const;
    int         MaxDepthViaEdgeName(const std::string& vertexName,
                                   const std::string& edgeName) const;
    int findVertexIndex(const std::string& vertexName)const;
   
    float find0(const std::string& vertexNameFrom,
                                       const std::string& vertexNameTo,const std::string& edgeName)const;
    float find1(const std::string& vertexNameFrom,
                                       const std::string& vertexNameTo,const std::string& edgeName)const;
    bool find( const std::string& vertexNameFrom,
                                       const std::string& vertexNameTo, const std::string& edgeName) const;
    bool find( int vertexNameFrom,
                                       std::vector<bool> visited) const;
    bool find2( int startVertexIndex,
                                       int endVertexIndex,std::vector<std::vector<bool>>& visited) const;
    // Implemented Functions for Debugging
    void        PrintPath(const std::vector<int>& orderedVertexEdgeIndexList,
                          float heuristicWeight,
                          bool sameLine = false) const;
    void        PrintEntireGraph() const;
    void        OneVertexToDifferentUnvisitedVerticesAmount(std::vector<int>& non_vn, 
                                                            std::vector<bool>& visited) const;
    void        Dfs(int& vertex_index, const std::string& edge_name, std::vector<bool>& visited) const;
    int         FirstNonUtilizedEdge(std::vector<std::string>& spanning_edges, 
                                    int vertex_index, std::vector<bool>& visited) const;
    int         size() const{return vertexList.size();}
    const GraphVertex& operator[](int index) const{return vertexList[index];}


};

#endif // MULTI_GRAPH_H