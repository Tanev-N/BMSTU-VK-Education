#include "MatrixGraph.h"


MatrixGraph::MatrixGraph(int size)
{
    EdgeMatrix = new int*[size];
    for (int i = 0; i < size; i++)
    {
        EdgeMatrix[i] = new int[size]();
    }
    _size = size;

}
    
MatrixGraph::MatrixGraph(const IGraph &graph)
{
    _size = graph.VerticesCount();
    EdgeMatrix = new int*[_size];
    for (int i = 0; i < _size; i++)
    {
        EdgeMatrix[i] = new int[_size]();
        std::vector<int> next = graph.GetNextVertices(i);
        for (int j = 0; j < next.size(); j++)
        {
            AddEdge(i, next[j]);
        }

    } 
}
    
MatrixGraph::~MatrixGraph()
{
    for (int i = 0; i < _size; i++)
    {
        delete[] EdgeMatrix[i];
    }
    delete[] EdgeMatrix;

}

    
void MatrixGraph::AddEdge(int from, int to)
{
    EdgeMatrix[from][to] = 1;
}
    
    
int MatrixGraph::VerticesCount() const
{
    return _size;
}
    
std::vector<int> MatrixGraph::GetNextVertices(int vertex) const 
{
    std::vector<int> next;
    for (int i = 0; i < _size; i++)
    {
        if (EdgeMatrix[vertex][i] == 1)
        {
            next.push_back(i);
        }
    }
    return next;
}
    
std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const 
{
    std::vector<int> prev;
    for (int i = 0; i < _size; i++)
    {
        if (EdgeMatrix[i][vertex] == 1)
        {
            prev.push_back(i);
        }
    }
    return prev;
}