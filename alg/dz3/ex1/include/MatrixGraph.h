#ifndef MATRIX_GRAPH_H
#define MATRIX_GRAPH_H

#include "IGraph.h"

struct MatrixGraph: public IGraph
{
public:
    MatrixGraph(int size);
    
    MatrixGraph(const IGraph &graph);
    
    ~MatrixGraph();
    
    void AddEdge(int from, int to) override;
    
    
    int VerticesCount() const override;
    
    std::vector<int> GetNextVertices(int vertex) const override;
    
    std::vector<int> GetPrevVertices(int vertex) const override;
    
private:
    int** EdgeMatrix;
    int _size;
};

#endif