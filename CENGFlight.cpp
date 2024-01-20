#include "CENGFlight.h"
#include <iostream>

//=======================//
// Implemented Functions //
//=======================//
void CENGFlight::PrintCanNotHalt(const std::string& airportFrom,
                                 const std::string& airportTo,
                                 const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines is not found and cannot be halted"
              << std::endl;
}

void CENGFlight::PrintCanNotResumeFlight(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         const std::string& airlineName)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" via "
              << airlineName
              << " airlines cannot be resumed"
              << std::endl;
}

void CENGFlight::PrintFlightFoundInCache(const std::string& airportFrom,
                                         const std::string& airportTo,
                                         bool isCostWeighted)
{
    std::cout << "A flight path between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  " is found in cache." << std::endl;
}

void CENGFlight::PrintFlightCalculated(const std::string& airportFrom,
                                       const std::string& airportTo,
                                       bool isCostWeighted)
{
    std::cout << "A flight path is calculated between \""
              << airportFrom << "\" and \""
              << airportTo << "\" using "
              << ((isCostWeighted) ? "cost" : "price")
              <<  "." << std::endl;
}

void CENGFlight::PrintPathDontExist(const std::string& airportFrom,
                                    const std::string& airportTo)
{
    std::cout << "A flight path does not exists between \""
              << airportFrom << "\" and \""
              << airportTo <<"\"." << std::endl;
}

void CENGFlight::PrintSisterAirlinesDontCover(const std::string& airportFrom)
{
    std::cout << "Could not able to generate sister airline list from \""
              << airportFrom <<"\"." << std::endl;
}

void CENGFlight::PrintMap()
{
    navigationMap.PrintEntireGraph();
}

void CENGFlight::PrintCache()
{
    lruTable.PrintTable();
}

CENGFlight::CENGFlight(const std::string& flightMapPath)
    : navigationMap(flightMapPath)
{}

//=======================//
//          TODO         //
//=======================//
void CENGFlight::HaltFlight(const std::string& airportFrom,
                            const std::string& airportTo,
                            const std::string& airlineName)
{
    /* TODO */
    HaltedFlight halt;
    if(!navigationMap.find(airportFrom,airportTo,airlineName)){
        PrintCanNotHalt(airportFrom,
                                  airportTo,
                                  airlineName);
                                  return;
    }
    halt.airportFrom = airportFrom;
    halt.airportTo = airportTo;
    halt.airline = airlineName;
    halt.w0 = navigationMap.find0(airportFrom, airportTo, airlineName);
    halt.w1 = navigationMap.find1(airportFrom, airportTo, airlineName);
    haltedFlights.push_back(halt);
    navigationMap.RemoveEdge(airlineName,airportFrom, airportTo);
    
}

    // (Direct Function call)
void CENGFlight::ContinueFlight(const std::string& airportFrom,
                                const std::string& airportTo,
                                const std::string& airlineName)
{
    /* TODO */
    if(navigationMap.find(airportFrom,airportTo,airlineName)){
        PrintCanNotResumeFlight(airportFrom,
                                  airportTo,
                                  airlineName);
                                  return;
    }
    for(int i = 0; i < haltedFlights.size(); i++){
        if(haltedFlights[i].airportFrom == airportFrom && haltedFlights[i].airportTo == airportTo && haltedFlights[i].airline == airlineName){
            navigationMap.AddEdge(airlineName, airportFrom,airportTo, haltedFlights[i].w0, haltedFlights[i].w1);
            return;
        }
    }
    PrintCanNotResumeFlight(airportFrom, airportTo,airlineName);
}

void CENGFlight::FindFlight(const std::string& startAirportName,
                            const std::string& endAirportName,
                            float alpha)
{
    /* TODO */
    std::vector<int> orderedVertexEdgeIndexList;
    bool pathExists = navigationMap.HeuristicShortestPath(orderedVertexEdgeIndexList, startAirportName, endAirportName, alpha);
    bool isCostWeighted = true;
    std::vector<int> intArray;
    if (pathExists)
        {
            if (lruTable.Find(intArray, orderedVertexEdgeIndexList[0], orderedVertexEdgeIndexList[orderedVertexEdgeIndexList.size()- 1], isCostWeighted))
    {
        if(alpha == 0 || alpha == 1)lruTable.Insert(orderedVertexEdgeIndexList, isCostWeighted);
        PrintFlightFoundInCache(startAirportName, endAirportName, isCostWeighted);
        navigationMap.PrintPath(orderedVertexEdgeIndexList, alpha,true);
        return;
    }
    else
    {
        
        if(alpha == 0 || alpha == 1)lruTable.Insert(orderedVertexEdgeIndexList, isCostWeighted),PrintFlightCalculated(startAirportName, endAirportName, isCostWeighted);
            navigationMap.PrintPath(orderedVertexEdgeIndexList, alpha,true);
            return;
        
    }
            // Cache the path in the hash table
            
        }
        else
        {
            PrintPathDontExist(startAirportName, endAirportName);
            return;
        }
    
    
}

void CENGFlight::FindSpecificFlight(const std::string& startAirportName,
                                    const std::string& endAirportName,
                                    float alpha,
                                    const std::vector<std::string>& unwantedAirlineNames) const
{
    /* TODO */
    std::vector<int> orderedVertexEdgeIndexList;
    bool isCostWeighted = true;
    // Use FilteredShortestPath to find the path with specified constraints
    bool foundPath = navigationMap.FilteredShortestPath(orderedVertexEdgeIndexList,
                                          startAirportName,
                                          endAirportName,
                                          alpha,
                                          unwantedAirlineNames);
    if(foundPath){
        
        navigationMap.PrintPath(orderedVertexEdgeIndexList, alpha,true);
        
    }
    
}

void CENGFlight::FindSisterAirlines(std::vector<std::string>& airlineNames,
                               const std::string& startAirlineName,
                               const std::string& airportName) const
{
    /* TODO */
    

    // Initialize data structures
     int all_vertices= navigationMap.size();
    int vertex= navigationMap.findVertexIndex(airportName);
    if (vertex == -1){ // No airport named like that
        PrintSisterAirlinesDontCover(airportName);
        return;
    }
    
    int starting_edge_amount= navigationMap[vertex].edges.size();
    int startAirline_index=-1;
    for (int i=0; i<starting_edge_amount; i++){ // No airline from the airport
        if (navigationMap[vertex].edges[i].name == startAirlineName) startAirline_index=i;
    }
    if (startAirline_index == -1){
        PrintSisterAirlinesDontCover(airportName);
        return;
    }
    
    std::vector<std::string> spanning_lines;
    std::vector<bool> visited(all_vertices, false);
    std::vector<int> non_vn(all_vertices, 0); // Non-visited neighbours
    bool all_visited= false, change_visited=true;
    
    // non_vn is constructed with correct information
    spanning_lines.push_back(startAirlineName);
    navigationMap.Dfs(vertex, startAirlineName, visited); // First spanning with initial
    navigationMap.OneVertexToDifferentUnvisitedVerticesAmount(non_vn, visited);
    
    int count=0;
    while (!all_visited && change_visited){
        MaxPairHeap<int, int> most_non_vn; // non_vn numbers and index
        std::vector<bool> visited_inside= visited;
        
        for (int i=0; i<all_vertices; i++){
            if (visited[i]) most_non_vn.push({non_vn[i], i});
        }
        int max_non_vn= most_non_vn.top().key;
        int max_non_vn_index= most_non_vn.top().value;

        // If max_non_vn is 0 nowhere to go
        if (max_non_vn > 0){
            if (navigationMap.FirstNonUtilizedEdge(spanning_lines, max_non_vn_index, visited) == -1){
                PrintSisterAirlinesDontCover(airportName);
                return;
            }
            bool change_with_extra_airlines=true;
            // Span till no change
            while(change_with_extra_airlines){
                std::vector<bool> visited_with_extra_airlines= visited;
                int spanning_size= spanning_lines.size();
                
                for (int i=0; i<spanning_size; i++){
                    for (int j=0; j<all_vertices; j++){
                        if (visited[j]){
                            navigationMap.Dfs(j, spanning_lines[i], visited);
                        }
                    }
                }
                
                change_with_extra_airlines=false;
                if (visited_with_extra_airlines != visited) change_with_extra_airlines=true;
            }
        }
        
        all_visited=true;
        for (int i=0; i<all_vertices; i++){
            if (!visited[i]) all_visited=false;
        }
        if (visited_inside == visited 
            && count > navigationMap[max_non_vn_index].edges.size()) 
                change_visited= false; // Change it if visited is changed
        if (visited_inside == visited) count++;
        // Edit non_vn
        navigationMap.OneVertexToDifferentUnvisitedVerticesAmount(non_vn, visited);
    }
    
    if (!change_visited){
        PrintSisterAirlinesDontCover(airportName);
        return;
    }
    spanning_lines.erase(spanning_lines.begin());
    airlineNames= spanning_lines; // return airlines

}

int CENGFlight::FurthestTransferViaAirline(const std::string& airportName,
                                           const std::string& airlineName) const
{
    /* TODO */
    if(navigationMap.MaxDepthViaEdgeName(airportName,airlineName) == 0)return -1;
    
    return navigationMap.MaxDepthViaEdgeName(airportName,airlineName) ;
}