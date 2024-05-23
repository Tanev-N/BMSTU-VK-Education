#include "SetGraph.h"

SetGraph::SetGraph(const IGraph &graph)
    {
        for (int i = 0; i < graph.VerticesCount(); ++i)
        {
            std::vector<int> nextVertices = graph.GetNextVertices(i);
            adjacencySets[i].insert(nextVertices.begin(), nextVertices.end());
        }
    }
void SetGraph::AddEdge(int from, int to)
    {

        adjacencySets[from].insert(to);
    }

int SetGraph::VerticesCount() const
    {
        return (int)adjacencySets.size();
    }

std::vector<int> SetGraph::GetNextVertices(int vertex) const
    {
        std::vector<int> next;

        for (int to: adjacencySets[vertex])
        {
            next.push_back(to);
        }
        return next;
    }
 std::vector<int> SetGraph::GetPrevVertices(int vertex) const 
    {
        std::vector<int> prevVertices;
        
        for (int from = 0; from < adjacencySets.size(); ++from)
        {
            for (int to: adjacencySets[from])
            {
                if (to == vertex)
                    prevVertices.push_back(from);
            }
        }
        return prevVertices;
    }