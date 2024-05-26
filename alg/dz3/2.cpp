

#include <iostream>
#include <vector>
#include <cassert>
#include <climits>

struct IGraph {
    virtual ~IGraph() {}
        
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
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
    
    void AddEdge(int from, int to) override;
    
    
    int VerticesCount() const override;
    
    std::vector<int> GetNextVertices(int vertex) const override;
    
    std::vector<int> GetPrevVertices(int vertex) const override;
    
private:
    std::vector<std::vector<int>> adjacencyLists;
};



ListGraph::ListGraph(const IGraph &graph)
    {
        for (int i = 0; i < graph.VerticesCount(); ++i)
        {
            adjacencyLists[i] = graph.GetNextVertices(i);
        }
    }
void ListGraph::AddEdge(int from, int to)
    {
        assert(0 <= from && from < adjacencyLists.size());
        assert(0 <= to && to < adjacencyLists.size());
        adjacencyLists[from].push_back(to);
    }

int ListGraph::VerticesCount() const
    {
        return (int)adjacencyLists.size();
    }

std::vector<int> ListGraph::GetNextVertices(int vertex) const
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
            for (int to: adjacencyLists[from])
            {
                if (to == vertex)
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
    std::vector<int> count_min_ranges(graph.VerticesCount(), 0);
    std::vector<int> min_ranges(graph.VerticesCount(), INT_MAX);

    count_min_ranges[u] = 1;
    min_ranges[u] = 0;
    visited[u] = true;

    std::queue<int> qu;
    qu.push(u);
    
    
    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();
        
        for (int nextVertex: graph.GetNextVertices(currentVertex))
        {
            if (!visited[nextVertex])
            {
                visited[nextVertex] = true;
                count_min_ranges[nextVertex] = count_min_ranges[currentVertex];
                min_ranges[nextVertex] = min_ranges[currentVertex]+1;
                qu.push(nextVertex);
            }
            else if (min_ranges[nextVertex] == min_ranges[currentVertex]+1)
            {
                count_min_ranges[nextVertex] += count_min_ranges[currentVertex];
            }
        }
    }


    return count_min_ranges[w];
}





void BFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    std::queue<int> qu;
    qu.push(vertex);
    visited[vertex] = true;
    
    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();
        
        func(currentVertex);
        
        for (int nextVertex: graph.GetNextVertices(currentVertex))
        {
            if (!visited[nextVertex])
            {
                visited[nextVertex] = true;
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::vector<int> minranges(graph.VerticesCount(), 0);
    
    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        if (!visited[i])
        {
            BFS(graph, i, visited, func);
        }
    }
}

void DFS(const IGraph &graph, int vertex, std::vector<bool> &visited, const std::function<void(int)> &func)
{
    visited[vertex] = true;
    func(vertex);
    
    for (int nextVertex: graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
        {
            DFS(graph, nextVertex, visited, func);
        }
    }
}

void mainDFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    
    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        if (!visited[i])
        {
            DFS(graph, i, visited, func);
        }
    }
}

void topologicalSortInternal(const IGraph &graph, int vertex, std::vector<bool> &visited, std::deque<int> &sorted)
{
    visited[vertex] = true;
    
    for (int nextVertex: graph.GetNextVertices(vertex))
    {
        if (!visited[nextVertex])
        {
            topologicalSortInternal(graph, nextVertex, visited, sorted);
        }
    }
    
    sorted.push_front(vertex);
}

std::deque<int> topologicalSort(const IGraph &graph)
{
    std::deque<int> sorted;
    std::vector<bool> visited(graph.VerticesCount(), false);
    
    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        if (!visited[i])
        {
            topologicalSortInternal(graph, i, visited, sorted);
        }
    }
    
    return sorted;
}



int main()
{
    int v, e;
    std::cin >> v >> e;
    
    ListGraph graph(v);
    
    for (int i = 0; i < e; ++i)
    {
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }
    int u,w;
    std::cin >> u >> w;
    std::cout << SearchMinRanges(graph, u, w);
    return 0;
}