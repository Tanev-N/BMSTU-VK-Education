#include "ArcGraph.h"


ArcGraph::ArcGraph(int size) : countVertices(size)
{

}
    
ArcGraph::ArcGraph(const IGraph &graph)
{
    for(int i = 0; i < graph.VerticesCount(); i++)
    {
        std::vector<int> nextVertices = graph.GetNextVertices(i);
        for (int j = 0; j < nextVertices.size(); j++)
        {
            AddEdge(i, nextVertices[j]);
        }
    }
}
    
ArcGraph::~ArcGraph()
{

}
    
void ArcGraph::AddEdge(int from, int to)
{
    EdgeArray.push_back({from, to});
}
    
    
int ArcGraph::VerticesCount() const
{
    return countVertices;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const
{
    std::vector<int> nextVertices;
    for (int i = 0; i < EdgeArray.size(); i++)
    {
        if (EdgeArray[i].first == vertex)
        {
            nextVertices.push_back(EdgeArray[i].second);
        }
    }
    return nextVertices;
}
    
std::vector<int> ArcGraph::GetPrevVertices(int vertex) const 
{
    std::vector<int> prevVertices;
    for (int i = 0; i < EdgeArray.size(); i++)
    {
        if (EdgeArray[i].second == vertex)
        {
            prevVertices.push_back(EdgeArray[i].first);
        }
    }
    return prevVertices;
}