

#include <iostream>
#include <vector>
#include <cassert>
#include <climits>
#include <set>

struct IGraph {
    virtual ~IGraph() {}
        
    virtual void AddEdge(int from, int to, int value) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<std::pair<int,int>> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};



struct ListGraph: public IGraph
{
public:
    ListGraph(int size)
    : adjacencyLists(size)
    {
    }
    
    ListGraph(const IGraph &graph);
    
    ~ListGraph()
    {
    }
    
    void AddEdge(int from, int to, int value) override;
    
    
    int VerticesCount() const override;
    
    std::vector<std::pair<int,int>> GetNextVertices(int vertex) const override;
    
    std::vector<int> GetPrevVertices(int vertex) const override;
    
private:
    std::vector<std::vector<std::pair<int,int>>> adjacencyLists;
};



ListGraph::ListGraph(const IGraph &graph)
    {
        for (int i = 0; i < graph.VerticesCount(); ++i)
        {
                adjacencyLists[i] = (graph.GetNextVertices(i));
        }
    }
void ListGraph::AddEdge(int from, int to, int value)
    {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(std::pair<int,int>(to, value));
    }

int ListGraph::VerticesCount() const
    {
        return (int)adjacencyLists.size();
    }

std::vector<std::pair<int,int>> ListGraph::GetNextVertices(int vertex) const
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        return adjacencyLists[vertex];
    }
 std::vector<int> ListGraph::GetPrevVertices(int vertex) const 
    {
        assert(0 <= vertex && vertex < adjacencyLists.size());
        std::vector<int> prevVertices;
        
        for (int from = 0; from < adjacencyLists.size(); ++from)
        {
            for (auto to: adjacencyLists[from])
            {
                if (to.first == vertex)
                    prevVertices.push_back(from);
            }
        }
        return prevVertices;
    }



#include <deque>
#include <functional>
#include <queue>




int SearchMinRanges(const IGraph &graph, int u, int w)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::vector<int> min_ranges(graph.VerticesCount(), INT_MAX);

    min_ranges[u] = 0;
    visited[u] = true;




    std::set<std::pair<int,int>> hip;
    hip.insert({0, u});
    
    
    while (!hip.empty())
    {
        auto currentVertex = hip.extract(hip.begin()).value().second;
    
        
        for (auto nextVertex: graph.GetNextVertices(currentVertex))
        {
            if (!visited[nextVertex.first])
            {
                visited[nextVertex.first] = true;
                min_ranges[nextVertex.first] = min_ranges[currentVertex]+nextVertex.second;
                hip.insert({min_ranges[nextVertex.first], nextVertex.first});
            }
            else if (min_ranges[nextVertex.first] > min_ranges[currentVertex]+nextVertex.second)
            {
                hip.erase({min_ranges[nextVertex.first], nextVertex.first});
                min_ranges[nextVertex.first] = min_ranges[currentVertex]+nextVertex.second;
                hip.insert({min_ranges[nextVertex.first], nextVertex.first});
            }
        }
    }


    return min_ranges[w];
}




int main()
{
    int city, e;
    std::cin >> city >> e;
    
    ListGraph graph(city);
    
    for (int i = 0; i < e; ++i)
    {
        int from, to, value;
        std::cin >> from >> to >> value;
        graph.AddEdge(from, to, value);
        graph.AddEdge(to, from, value);
    }
    int u,w;
    std::cin >> u >> w;
    std::cout << SearchMinRanges(graph, u, w);
    return 0;
}