#ifndef SET_GRAPH_H
#define SET_GRAPH_H

#include <iostream>
#include <vector>
#include <cassert>
#include <set>
#include "IGraph.h"


struct SetGraph: public IGraph
{
public:
    SetGraph(int size)
    : adjacencySets(size)
    {
    }
    
    SetGraph(const IGraph &graph);
    
    ~SetGraph()
    {
    }
    
    void AddEdge(int from, int to) override;
    
    
    int VerticesCount() const override;
    
    std::vector<int> GetNextVertices(int vertex) const override;
    
    std::vector<int> GetPrevVertices(int vertex) const override;
    
private:
    std::vector<std::set<int>> adjacencySets;
};


#endif