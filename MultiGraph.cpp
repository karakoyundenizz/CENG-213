#include "MultiGraph.h"
#include "Exceptions.h"
#include "IntPair.h"
#include <iostream>
#include <iomanip>
#include <fstream>

//=======================//
// Implemented Functions //
//=======================//
MultiGraph::MultiGraph()
{}

MultiGraph::MultiGraph(const std::string& filePath)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    // Tokens
    std::string tokens[5];
    std::ifstream mapFile(filePath.c_str());

    if(!mapFile.is_open())
    {
        std::cout << "Unable to open " << filePath << std::endl;
        return;
    }

    // Read line by line
    std::string line;
    while (std::getline(mapFile, line))
    {
        // Empty Line Skip
        if(line.empty()) continue;
        // Comment Skip
        if(line[0] == '#') continue;

        // Tokenize the line
        int i = 0;
        std::istringstream stream(line);
        while(stream >> tokens[i]) i++;

        // Single token (Meaning it is a vertex)
        if(i == 1)
        {
            InsertVertex(tokens[0]);
        }
        // Exactly 5 tokens (Meaning it is an edge)
        else if(i == 5)
        {
            // Rename vars for readablity
            const std::string& vertexFromName = tokens[0];
            const std::string& vertexToName = tokens[1];
            const std::string& edgeName = tokens[2];
            float weight0 = static_cast<float>(std::atof(tokens[3].c_str()));
            float weight1 = static_cast<float>(std::atof(tokens[4].c_str()));
            AddEdge(edgeName, vertexFromName, vertexToName,
                    weight0, weight1);
        }
        else std::cerr << "Token Size Mismatch" << std::endl;
    }
}

void MultiGraph::PrintPath(const std::vector<int>& orderedVertexEdgeIndexList,
                           float heuristicWeight,
                           bool sameLine) const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this file !       //
    // ============================= //

    // Name is too long
    const std::vector<int>& ove = orderedVertexEdgeIndexList;
    // Invalid list
    // At least three items should be available
    if(ove.size() < 3) return;

    // Check vertex and an edge
    for(size_t i = 0; i < orderedVertexEdgeIndexList.size(); i += 2)
    {
        int vertexId = ove[i];
        if(vertexId >= static_cast<int>(vertexList.size()))
        {
            // Return if there is a bad vertex id
            std::cout << "VertexId " << vertexId
                      << " not found!" << std::endl;
            return;
        }


        const GraphVertex& vertex = vertexList[vertexId];
        std::cout << vertex.name;
        if(!sameLine) std::cout << "\n";
        // Only find and print the weight if next is available
        if(i == ove.size() - 1) break;
        int nextVertexId = ove[i + 2];
        if(nextVertexId >= static_cast<int>(vertexList.size()))
        {
            // Return if there is a bad vertex id
            std::cout << "VertexId " << vertexId
                    << " not found!" << std::endl;
            return;
        }

        // Find the edge between these two vertices
        int localEdgeId = ove[i + 1];
        if(localEdgeId >= static_cast<int>(vertex.edges.size()))
        {
            // Return if there is a bad vertex id
            std::cout << "EdgeId " << localEdgeId
                      << " not found in " << vertexId << "!" << std::endl;
            return;
        }

        const GraphEdge& edge = vertex.edges[localEdgeId];

        // Combine with heuristic (linear interpolation)
        float weight = Lerp(edge.weight[0], edge.weight[1],
                            heuristicWeight);

        std::cout << "-" << std::setfill('-')
                  << std::setw(4)
                  << weight << "->";
    }
    // Print endline on the last vertex if same line is set
    if(sameLine) std::cout << "\n";
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

void MultiGraph::PrintEntireGraph() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    for(size_t i = 0; i < vertexList.size(); i++)
    {
        const GraphVertex& v = vertexList[i];
        std::cout << v.name << "\n";
        for(size_t j = 0; j < v.edges.size(); j++)
        {
            const GraphEdge& edge = v.edges[j];

            // List the all vertex names and weight
            std::cout << "    -"
                      << std::setfill('-')
                      << std::setw(4) << edge.weight[0]
                      << "-"
                      << std::setw(4) << edge.weight[1]
                      << "-> ";
            std::cout << vertexList[edge.endVertexIndex].name;
            std::cout << " (" << edge.name << ")" << "\n";
        }
    }
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

//=======================//
//          TODO         //
//=======================//
float MultiGraph::Lerp(float w0, float w1, float alpha)
{
    /* TODO */
    
    return alpha*w1 + (1-alpha)*w0;
}

void MultiGraph::InsertVertex(const std::string& vertexName)
{
    /* TODO */
    for(int i = 0; i < vertexList.size(); i++){
        if(vertexList[i].name == vertexName){
            throw DuplicateVertexException(vertexName);
        }
    }
    GraphVertex new_vertex;
    new_vertex.name = vertexName;
    vertexList.push_back(new_vertex);
}

void MultiGraph::RemoveVertex(const std::string& vertexName)
{
    /* TODO */
    int Index = -1;
    for (int i = 0; i < vertexList.size(); i++) {
        if (vertexList[i].name == vertexName) {
            Index = i;
        }
    }
    if (Index == -1)
        throw VertexNotFoundException(vertexName);
    // I need two loops one for traversing the vertexList other for traversing vertexEdges
    for (int i = 0;i<vertexList.size();i++){
        for (int j = 0;j<vertexList[i].edges.size();j++){
            if (vertexList[i].edges[j].endVertexIndex == Index)
                RemoveEdge(vertexList[i].edges[j].name,vertexList[i].name,vertexList[Index].name);
        }
    }
    std::vector<GraphVertex>::iterator it;
    it = vertexList.begin() + Index;
    vertexList.erase(it);

}

void MultiGraph::AddEdge(const std::string& edgeName,
                         const std::string& vertexFromName,
                         const std::string& vertexToName,
                         float weight0, float weight1)
{
    /* TODO */
    int to = -1, from = -1;
    for (int i = 0; i < vertexList.size(); i++) {
        if (vertexList[i].name == vertexToName) {
            to = i;
        } else if (vertexList[i].name == vertexFromName) {
            from = i;
        }
    }
    if (from == -1) throw VertexNotFoundException(vertexFromName);
    if (to == -1) throw VertexNotFoundException(vertexToName);
    for (int i = 0; i < vertexList[from].edges.size(); i++) {
        if (vertexList[from].edges[i].endVertexIndex == to &&
            vertexList[from].edges[i].name == edgeName) {
            throw SameNamedEdgeException(edgeName, vertexFromName, vertexToName);
        }
    }
    GraphEdge new_edge;
    new_edge.name = edgeName;
    new_edge.weight[0] = weight0;
    new_edge.weight[1] = weight1;
    new_edge.endVertexIndex = to;  
    vertexList[from].edges.push_back(new_edge);
}

void MultiGraph::RemoveEdge(const std::string& edgeName,
                            const std::string& vertexFromName,
                            const std::string& vertexToName)
{
    /* TODO */
    int to = -1, from = -1;

    for (int i = 0; i < vertexList.size(); i++) {
        if (vertexList[i].name == vertexToName) {
            to = i;
        } else if (vertexList[i].name == vertexFromName) {
            from = i;
        }
    }

    if (from == -1) {
        throw VertexNotFoundException(vertexFromName);
    }

    if (to == -1) {
        throw VertexNotFoundException(vertexToName);
    }

    for (int i = 0; i < vertexList[from].edges.size(); i++) {
        if (vertexList[from].edges[i].endVertexIndex == to &&
            vertexList[from].edges[i].name == edgeName) {
            vertexList[from].edges.erase(vertexList[from].edges.begin() + i);
            return;  
        }
    }

    throw EdgeNotFoundException(vertexFromName, edgeName);
}

float MultiGraph::find0( const std::string& vertexNameFrom,
                                       const std::string& vertexNameTo, const std::string& edgeName) const{
    
    int from = -1;
    float w0;
    for (int i = 0; i < vertexList.size(); i++){
        if (vertexList[i].name == vertexNameFrom){
            from = i;
        }
    }
    if(from == -1) throw VertexNotFoundException(vertexNameFrom);
    for (int i = 0; i < vertexList[from].edges.size(); i++) {
        if (vertexList[from].edges[i].name == edgeName && vertexList[vertexList[from].edges[i].endVertexIndex].name == vertexNameTo) {
            w0 = vertexList[from].edges[i].weight[0];
        }
    }
    return w0;
}
float MultiGraph::find1( const std::string& vertexNameFrom,
                                       const std::string& vertexNameTo, const std::string& edgeName) const{
    
    int from = -1;
    float w1;
    for (int i = 0; i < vertexList.size(); i++){
        if (vertexList[i].name == vertexNameFrom){
            from = i;
        }
    }
    for (int i = 0; i < vertexList[from].edges.size(); i++) {
        if (vertexList[from].edges[i].name == edgeName && vertexList[vertexList[from].edges[i].endVertexIndex].name == vertexNameTo) {
            w1 = vertexList[from].edges[i].weight[1];
        }
    }
    return w1;
}
bool MultiGraph::find( const std::string& vertexNameFrom,
                                       const std::string& vertexNameTo, const std::string& edgeName) const{
    int from = -1,yes = 0;
    float w1;
    for (int i = 0; i < vertexList.size(); i++){
        if (vertexList[i].name == vertexNameFrom){
            from = i;
        }
    }
    if(from == -1) return 0;
    for (int i = 0; i < vertexList[from].edges.size(); i++) {
        if (vertexList[from].edges[i].name == edgeName && vertexList[vertexList[from].edges[i].endVertexIndex].name == vertexNameTo) {
            w1 = vertexList[from].edges[i].weight[1];
            yes = 1;
        }
    }
    return yes;
}
bool MultiGraph::find2( int startVertexIndex,
                                       int endVertexIndex,std::vector<std::vector<bool>>& visited) const{
     
     visited[startVertexIndex][endVertexIndex] = true;

    // If the current vertex is the destination, then there is a path
    if (startVertexIndex == endVertexIndex)
        return true;

    // Recur for all adjacent vertices
    for (const auto& edge : vertexList[startVertexIndex].edges)
    {
        int adjacentVertexIndex = edge.endVertexIndex;

        // If the adjacent vertex is not visited, recur for it
        if (!visited[startVertexIndex][adjacentVertexIndex] &&
            find2(adjacentVertexIndex, endVertexIndex, visited))
            return true;
    }

    return false;
}
                                  
                                       
int MultiGraph::findVertexIndex(const std::string& vertexName) const{
    for (size_t i = 0; i < vertexList.size(); ++i) {
        if (vertexList[i].name == vertexName) {
            return static_cast<int>(i);
        }
    }
    throw std::invalid_argument("Vertex not found: " + vertexName);
}

bool MultiGraph::HeuristicShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                       const std::string& vertexNameFrom,
                                       const std::string& vertexNameTo,
                                       float heuristicWeight) const {
   // Find the indices of the vertices
        int from = -1, to = -1;
        for (int i = 0; i < vertexList.size(); i++)
        {
            if (vertexList[i].name == vertexNameFrom)
            {
                from = i;
            }
            else if (vertexList[i].name == vertexNameTo)
            {
                to = i;
            }
        }

        // Check if the vertices are found
        if (from == -1 || to == -1)
        {
            // Handle vertex not found error
            std::cerr << "Error: Vertex not found." << std::endl;
            return false;
        }

        // Initialize data structures
        std::vector<float> dist(vertexList.size(), 999999);
        std::vector<int> pred(vertexList.size(), -1);

        // Priority queue to store vertices and their distances
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> pq;

        // Start from the source vertex
        dist[from] = 0;
        pq.push({0, from});

        while (!pq.empty())
        {
            int currentVertex = pq.top().second;
            pq.pop();

            // Relax the outgoing edges from the current vertex
            for (const GraphEdge &edge : vertexList[currentVertex].edges)
            {
                // Calculate the combined weight using the 

    float newDist = dist[currentVertex] + edge.weight[1] * heuristicWeight + heuristicWeight + edge.weight[0] * (1.0 - heuristicWeight);

    if (newDist < dist[edge.endVertexIndex])
    {
        dist[edge.endVertexIndex] = newDist;
        pred[edge.endVertexIndex] = currentVertex;
        pq.push({newDist, edge.endVertexIndex});
    }
            }
        }

        // Reconstruct the path
        orderedVertexEdgeIndexList.clear();
        for (int crawl = to; crawl != -1; crawl = pred[crawl])
        {
            orderedVertexEdgeIndexList.push_back(crawl);

            if (pred[crawl] != -1)
            {
                // Find the edge index with the minimum weight
                int minEdgeIndex = -1;
                float minWeight = 9999999;

                for (size_t i = 0; i < vertexList[pred[crawl]].edges.size(); ++i)
{
    const GraphEdge &edge = vertexList[pred[crawl]].edges[i];
    if (edge.endVertexIndex == crawl)
    {
        float edgeWeight = Lerp(edge.weight[0], edge.weight[1], heuristicWeight);

        if (edgeWeight < minWeight)
        {
            minWeight = edgeWeight;
            minEdgeIndex = static_cast<int>(i);
        }
    }
}

                // Add the index of the edge with the minimum weight
                if (minEdgeIndex != -1)
                {
                    orderedVertexEdgeIndexList.push_back(minEdgeIndex);
                }
            }
        }

        // Reverse the order of the path
        int start = 0;
        int end = orderedVertexEdgeIndexList.size() - 1;

    while (start < end)
    {
        std::swap(orderedVertexEdgeIndexList[start], orderedVertexEdgeIndexList[end]);
        start++;
        end--;
    }
   /* for(int i = 0;i < orderedVertexEdgeIndexList.size() - 1; i++){
        std::cout <<vertexList[orderedVertexEdgeIndexList[i]].name << " --> ";
        std::cout<<vertexList[orderedVertexEdgeIndexList[i]].edges[orderedVertexEdgeIndexList[i+1]].name;
    }*/
        return true;
    }
    
                                           
                                       

bool MultiGraph::FilteredShortestPath(std::vector<int>& orderedVertexEdgeIndexList,
                                      const std::string& vertexNameFrom,
                                      const std::string& vertexNameTo,
                                      float heuristicWeight,
                                      const std::vector<std::string>& edgeNames) const
{
    int from = -1, to = -1;
        for (int i = 0; i < vertexList.size(); i++)
        {
            if (vertexList[i].name == vertexNameFrom)
            {
                from = i;
            }
            else if (vertexList[i].name == vertexNameTo)
            {
                to = i;
            }
        }

        // Check if the vertices are found
        if (from == -1 || to == -1)
        {
            // Handle vertex not found error
            std::cerr << "Error: Vertex not found." << std::endl;
            return false;
        }

        // Initialize data structures
        std::vector<float> dist(vertexList.size(), 999999);
        std::vector<int> pred(vertexList.size(), -1);

        // Priority queue to store vertices and their distances
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> pq;

        // Start from the source vertex
        dist[from] = 0;
        pq.push({0, from});

        while (!pq.empty())
        {
            int currentVertex = pq.top().second;
            pq.pop();

            // Relax the outgoing edges from the current vertex
            for (const GraphEdge &edge : vertexList[currentVertex].edges)
            {
                int yes = 1;
                for(const std::string name: edgeNames){
                    if(name == edge.name){
                        yes = 0;
                    }
                }
                if(yes){
                    float newDist = dist[currentVertex] + edge.weight[0];

                    if (newDist < dist[edge.endVertexIndex])
                    {
                        dist[edge.endVertexIndex] = newDist;
                        pred[edge.endVertexIndex] = currentVertex;
                        pq.push({newDist, edge.endVertexIndex});
                    }
                }
            }
        }

        // Reconstruct the path
        orderedVertexEdgeIndexList.clear();
        for (int crawl = to; crawl != -1; crawl = pred[crawl])
        {
            orderedVertexEdgeIndexList.push_back(crawl);

            if (pred[crawl] != -1)
            {
                // Find the edge index
                int minEdgeIndex = -1;
                float minWeight = 9999999;
                for (size_t i = 0; i < vertexList[pred[crawl]].edges.size(); ++i)
{
    const GraphEdge &edge = vertexList[pred[crawl]].edges[i];
    if (edge.endVertexIndex == crawl)
    {
        float edgeWeight = Lerp(edge.weight[0], edge.weight[1], heuristicWeight);
        int in = 0;
        for(int j = 0; j < edgeNames.size(); j++){
            if(edge.name == edgeNames[j])in =1;
        }
        if (edgeWeight < minWeight && !in)
        {
            minWeight = edgeWeight;
            minEdgeIndex = static_cast<int>(i);
        }
    }
}

                // Add the index of the edge with the minimum weight
                if (minEdgeIndex != -1)
                {
                    orderedVertexEdgeIndexList.push_back(minEdgeIndex);
                }
            }
            
        }

        // Reverse the order of the path
        int start = 0;
        int end = orderedVertexEdgeIndexList.size() - 1;

    while (start < end)
    {
        std::swap(orderedVertexEdgeIndexList[start], orderedVertexEdgeIndexList[end]);
        start++;
        end--;
    }
    if(orderedVertexEdgeIndexList.size() == 0 || orderedVertexEdgeIndexList.size() ==1)return false;
        return true;
}

int MultiGraph::BiDirectionalEdgeCount() const
{
    /* TODO */
    int count = 0;
    int numVertices = vertexList.size();

    for (int i = 0; i < numVertices; ++i) {
        for (int j = 0; j < vertexList[i].edges.size(); ++j) {
            int endVertexIndex = vertexList[i].edges[j].endVertexIndex;

            if (0 <= endVertexIndex && endVertexIndex < numVertices) {
                GraphVertex endVertex = vertexList[endVertexIndex];
                std::string name = vertexList[i].edges[j].name;
                for (int k = 0; k < endVertex.edges.size(); ++k) {
                    if (endVertex.edges[k].endVertexIndex == i && name == endVertex.edges[k].name) {
                        count++;
                        break;
                    }
                }
            }
        }
    }

    return count/2;
}

int MultiGraph::MaxDepthViaEdgeName(const std::string& vertexName,
                                    const std::string& edgeName) const
{
    /* TODO */
   /* int depth = 0, max_depth;
    int from = -1;

    for (int i = 0; i < vertexList.size(); i++) {
        if (vertexList[i].name == vertexName) {
            from = i;
        }
    }
    if(from == -1) throw VertexNotFoundException(vertexName);
    for(int i = 0; i < vertexList[from].edges.size(); i++){
        if(vertexList[from].edges[i].name == edgeName){
            depth = 1 + MaxDepthViaEdgeName()
        }
    }*/
    /*int depth = 0, from = -1;
    for (int i = 0; i < vertexList.size(); i++) {
            if (vertexList[i].name == vertexName) {
                from = i;
            }
    }
    MaxPairHeap<int,int> max_heap;
    max_heap.push({0,7});
    for(int i = 0; i < vertexList[from].edges.size(); i++){
        if(vertexList[from].edges[i].name == edgeName){
            GraphVertex vertex = vertexList.[vertexList[from].edges[i].endVertexIndex];
            int next = MaxDepthViaEdgeName(vertex.name, edgeName);
            max_heap.push({next + 1,7});
        }
    }
    return max_heap.top().key;*/
    /*int max_depth = 0, from = -1;
    for (int i = 0; i < vertexList.size(); i++)
    {
        if (vertexList[i].name == vertexName)
        {
            from = i;
        }
    }
    if (from == -1)
        throw VertexNotFoundException(vertexName);

    std::vector<std::vector<bool>> visited(vertexList.size(), std::vector<bool>(vertexList.size(), false));

    MaxPairHeap<int, int> max_heap;
    max_heap.push({0, from});

    while (!max_heap.empty())
    {
        int current_depth = max_heap.top().key;
        
        GraphVertex current_vertex = vertexList[max_heap.top().value];
        int index = max_heap.top().value;
        max_heap.pop();

        if (current_depth > max_depth)
            max_depth = current_depth;

        for (int i = 0; i < current_vertex.edges.size(); i++)
        {
            if (current_vertex.edges[i].name == edgeName && !(visited[index][current_vertex.edges[i].endVertexIndex]))
            {
                max_heap.push({current_depth + 1, current_vertex.edges[i].endVertexIndex});
                visited[index][current_vertex.edges[i].endVertexIndex] = true;
            }
        }
    }
    return max_depth;*/
    int src = findVertexIndex(vertexName);
    std::vector<int> dist(vertexList.size(),99999999); // The output array.  dist[i] will hold the
                 // shortest
    // distance from src to i
    int max_depth = -1;
 
   std::vector<bool> sptSet(vertexList.size(),false); // sptSet[i] will be true if vertex i is
                    // included in shortest
    // path tree or shortest distance from src to i is
    // finalized
 
    // Initialize all distances as INFINITE and stpSet[] as
    // false

 
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
 
    // Find shortest path for all vertices
    for (int count = 0; count < vertexList.size() - 1; count++) {
        int min = 99999999, min_ndex;
 
        for (int k = 0; k < vertexList.size(); k++)
        if (sptSet[k] == false && dist[k] <= min)
            min = dist[k], min_ndex = k;
        int u = min_ndex;
 
        // Mark the picked vertex as processed
        sptSet[u] = true;
 
        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < vertexList[u].edges.size(); v++){
            int in = vertexList[u].edges[v].endVertexIndex;
            // Update dist[v] only if is not in sptSet,
            // there is an edge from u to v, and total
            // weight of path from src to  v through u is
            // smaller than current value of dist[v]
            if (!sptSet[in]
                && dist[u] != 99999999
                && dist[u] + 1 < dist[in] && vertexList[u].edges[v].name == edgeName)
                dist[in] = dist[u] + 1;}
    }
    for (int k = 0; k < dist.size(); k++)
        if (dist[k] != 99999999 && dist[k] > max_depth)
            max_depth = dist[k];
    return max_depth;
}




void MultiGraph::OneVertexToDifferentUnvisitedVerticesAmount(std::vector<int>& non_vn, std::vector<bool>& visited) const{
    int all_vertices= vertexList.size();
    for (int i=0; i<all_vertices; i++){ // From a vertex to different unvisited vertices amount
        std::vector<int> edged_vector_indexes;
        if (visited[i]){
            for (int j=0; j<vertexList[i].edges.size(); j++){
                bool exist_or_visited= false;
                if (edged_vector_indexes.size() == 0 && visited[vertexList[i].edges[j].endVertexIndex]) exist_or_visited=true;
                for (int k=0; k<edged_vector_indexes.size(); k++){
                    if (vertexList[i].edges[j].endVertexIndex == edged_vector_indexes[k]
                        || visited[vertexList[i].edges[j].endVertexIndex]) 
                            exist_or_visited= true;
                }
                if (!exist_or_visited) edged_vector_indexes.push_back(vertexList[i].edges[j].endVertexIndex);
            }
        }
        non_vn[i]= edged_vector_indexes.size();
        /*std::cout<< "inside "<<i<< "=";
        for (int h=0; h<non_vn[i]; h++) std::cout<<edged_vector_indexes[h]<< " ";
        std::cout<<std::endl;*/
    }
}

void MultiGraph::Dfs(int& vertex_index, const std::string& edge_name, std::vector<bool>& visited) const{
    MaxPairHeap<int, int> pq; // Strictly increasing number and index
    int count=0;
    pq.push({count++, vertex_index});
    
    while (!pq.empty()){
        int index= pq.top().value;
        int edge_amount= vertexList[index].edges.size();
        pq.pop();
        visited[index]=true;
        
        for (int i=0; i<edge_amount; i++){
            if (vertexList[index].edges[i].name == edge_name
            && !visited[vertexList[index].edges[i].endVertexIndex]){
                pq.push({count++, vertexList[index].edges[i].endVertexIndex});
            }
        }
    }
}

int MultiGraph::FirstNonUtilizedEdge(std::vector<std::string>& spanning_edges, int vertex_index, std::vector<bool>& visited) const{
    int edge_amount=vertexList[vertex_index].edges.size();
    for (int i=0; i<edge_amount; i++){
        bool exist= false;
        int spanning_size= spanning_edges.size();
        for (int j=0; j<spanning_size; j++){
            if (vertexList[vertex_index].edges[i].name == spanning_edges[j]) exist= true;
            else{ // edgeNames are different but check whether it takes you somewhere new
                int next_vertex= vertexList[vertex_index].edges[i].endVertexIndex;
                if (visited[next_vertex]) exist=true; // If new vertex is came across change it
            }
        }
        if (!exist){
            spanning_edges.push_back(vertexList[vertex_index].edges[i].name);
            return i;
        }
    }
    return -1; // If this returned means that all edges are already in the list
}