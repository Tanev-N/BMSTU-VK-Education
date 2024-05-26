
#include <iostream>
#include <vector>
#include <cassert>
#include <queue>


struct IGraph {
    virtual ~IGraph() {}
        
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};


std::vector<char> reconstructPath(const std::vector<int> &predecessors, const std::vector<char> &moves, int currentVertex) {
    std::vector<char> path;
    while (currentVertex != -1) {
        path.push_back(moves[currentVertex]);
        currentVertex = predecessors[currentVertex];
    }
    std::reverse(path.begin(), path.end()); // Путь нужно инвертировать, так как он построен от конца к началу
    return path;
}

void outputSolution(const std::vector<char> &path) {
    for (char move : path) {
        std::cout << move << " ";
    }
    std::cout << std::endl;
}


// Можно использовать перечисление для представления ходов
enum class Move {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

Move determineMove(int fromVertex, int toVertex) {
    int fromRow = fromVertex / 4;
    int fromCol = fromVertex % 4;
    int toRow = toVertex / 4;
    int toCol = toVertex % 4;

    if (fromRow == toRow) {
        if (fromCol + 1 == toCol) {
            return Move::RIGHT;
        } else if (fromCol - 1 == toCol) {
            return Move::LEFT;
        }
    } else if (fromCol == toCol) {
        if (fromRow + 1 == toRow) {
            return Move::DOWN;
        } else if (fromRow - 1 == toRow) {
            return Move::UP;
        }
    }

    // Возвращаем недопустимое значение, если ход не может быть определён
    throw std::invalid_argument("Invalid move");
}

// Модификация функции reconstructPath для работы с перечислением Move
std::vector<Move> reconstructPath(const std::vector<int> &predecessors, const std::vector<Move> &moves, int currentVertex) {
    std::vector<Move> path;
    while (currentVertex != -1) {
        path.push_back(moves[currentVertex]);
        currentVertex = predecessors[currentVertex];
    }
    std::reverse(path.begin(), path.end()); // Путь нужно инвертировать
    return path;
}

// Функция для вывода решения в понятном формате
void outputSolution(const std::vector<Move> &path) {
    for (Move move : path) {
        switch (move) {
            case Move::UP:    std::cout << "UP "; break;
            case Move::DOWN:  std::cout << "DOWN "; break;
            case Move::LEFT:  std::cout << "LEFT "; break;
            case Move::RIGHT: std::cout << "RIGHT "; break;
        }
    }
    std::cout << std::endl;
}

// Функция, которая возвращает обратный ход
Move getOppositeMove(Move move) {
    switch (move) {
        case Move::UP:    return Move::DOWN;
        case Move::DOWN:  return Move::UP;
        case Move::LEFT:  return Move::RIGHT;
        case Move::RIGHT: return Move::LEFT;
    }
}

bool isFinalState(const std::vector<int>& currentVertex) {
    // Предполагаем, что финальное состояние это последовательность от 1 до 15, за которой следует 0.
    std::vector<int> finalState = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

    return currentVertex == finalState;
}


void BFS(const IGraph &graph, int startVertex, std::vector<bool> &visited, std::vector<int> &predecessors, std::vector<char> &moves)
{
    std::queue<int> qu;
    qu.push(startVertex);
    visited[startVertex] = true;
    predecessors[startVertex] = -1; // начальная вершина не имеет предшественника
    
    while (!qu.empty())
    {
        int currentVertex = qu.front();
        qu.pop();
        
        // Если currentVertex является конечной вершиной, восстанавливаем и выводим путь
        if (isFinalState(currentVertex))
        {
            std::vector<char> path = reconstructPath(predecessors, moves, currentVertex);
            outputSolution(path);
            return;
        }
        
        for (int nextVertex: graph.GetNextVertices(currentVertex))
        {
            if (!visited[nextVertex])
            {
                Move moveMade = determineMove(currentVertex, nextVertex);
                visited[nextVertex] = true;
                predecessors[nextVertex] = currentVertex;
                moves[nextVertex] = getOppositeMove(moveMade); // L, R, U, D
                qu.push(nextVertex);
            }
        }
    }
}

void mainBFS(const IGraph &graph, const std::function<void(int)> &func)
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    
    for (int i = 0; i < graph.VerticesCount(); ++i)
    {
        if (!visited[i])
        {
            BFS(graph, i, visited, func);
        }
    }
}


struct ListGraph: public IGraph
{
public:
    ListGraph(int size)
    : adjacencyLists(size), valuesLists(size)
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
    void FullFillEdge()
    {
        for (int i = 0; i < adjacencyLists.size(); ++i)
        {
            if ((i+1)%4 != 0 && (i+1) < adjacencyLists.size())
            {
                AddEdge(i, i+1);
            }
            if (i%4 != 0 && i > 0)
            {
                AddEdge(i, i-1);
            }
            if ((i+4) < adjacencyLists.size())
            {
                AddEdge(i, i+4);
            }
            if ((i-4) >= 0)
            {
                AddEdge(i, i-4);
            }
        }
    } 
    int checkInversion()
    {
    int inversion = 0;
    int i, j;
    for (i = 0; i < valuesLists.size(); ++i)
    {
        for (j = 0; j < valuesLists[i].size(); ++j)
        {
            int curr_value = valuesLists[i][j];
            if (curr_value != 0) // Пропускаем пустую ячейку
            {
                // Считаем инверсии для curr_value
                for (int m = i; m < valuesLists.size(); ++m)
                {
                    for (int n = (m == i ? j+1 : 0); n < valuesLists[m].size(); ++n)
                    {
                        if (valuesLists[m][n] != 0 && valuesLists[m][n] < curr_value)
                        {
                            ++inversion;
                        }
                    }
                }
            }
        }
    }
    return inversion;
    }

    bool checkSolution(int row_zero_position)
    {
        int check_inversion = checkInversion();
        if (check_inversion % 2 == 0 && row_zero_position % 2 != 0)
        {
            return true;
        }
        else if (check_inversion % 2 != 0 && row_zero_position % 2 == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void valuesAdd(int row, int col, int value)
    {
        valuesLists[row][col] = value;
    }   
    void setZeroPosition(int row, int col)
    {
        zero_position.first = row;
        zero_position.second = col;
    }
    void doStep(std::vector<char> &steps)
    {

    }

    std::pair<int, int> check_eff_step()
    {
        
        std::pair<int, int> min_step = std::make_pair(0, 0);
        for(int i = 0; i < 8; ++i)
        {

        }

    }
private:
    std::vector<std::vector<int>> adjacencyLists;
    std::vector<std::vector<int>> valuesLists;
    std::pair<int, int> zero_position;

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


int main()
{
    ListGraph graph(16);
    graph.FullFillEdge();
    int row_zero_position;
    for (int i = 0; i < 16; ++i)
    {
        int temp_value;
        std::cin >> temp_value;
        if (temp_value == 0)
        {
            row_zero_position = 4 - (i/4);
        }
        graph.valuesAdd(i/4, i%4, temp_value);
    }
    if (graph.checkSolution(row_zero_position))
    {
        std::vector<char> steps;
        while(graph.checkInversion() != 0)
        {
            graph.doStep(steps);
        }
        std::cout << steps.size() << std::endl;
       

    }
    else
    {
        std::cout << -1;
    }
     
}   