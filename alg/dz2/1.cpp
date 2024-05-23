// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
// Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
// 1_2. Для разрешения коллизий используйте двойное хеширование. ( мой вариант )
// Требования: В таблице запрещено хранение указателей на описатель элемента.
// Формат входных данных
// Каждая строка входных данных задает одну операцию над множеством. Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция.
// Тип операции  – один из трех символов:
//     +  означает добавление данной строки в множество;
//     -  означает удаление  строки из множества;
//     ?  означает проверку принадлежности данной строки множеству.
// При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве. При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.
// Формат выходных данных
// Программа должна вывести для каждой операции одну из двух строк OK или FAIL, в зависимости от того, встречается ли данное слово в нашем множестве

#include <iostream>
#include <stdlib.h>
#include <cassert>
#include <vector>

const size_t INITIAL_SIZE = 8;
const int first_default_hash_primmary = 78561;
const int second_default_hash_primmary = 897681;

template <typename T, typename C = std::equal_to<T>, typename H = std::hash<T>>
class HashTable
{
private:
    enum class HashType
    {
        KEY,
        DEL,
        NIL
    };
    class Node
    {
    public:
        T val;
        HashType type;
        Node(const T &_val = T(), HashType _type = HashType::NIL) : val(_val), type(_type) {}
    };
    C cmp;
    std::vector<Node> arr;
    size_t MaxSize;
    size_t size;
    H hasher1;
    H hasher2;
    void Domain_Expansion()
    {
        MaxSize *= 2;

        std::vector<Node> arr2 = std::move(arr);


        arr = std::vector<Node>(MaxSize);

        for (int i = 0; i < MaxSize / 2; i++)
        {
            if (arr2[i].type == HashType::KEY)
            {
                assert(!Has(arr2[i].val));
            }
        }
        for (int i = 0; i < MaxSize / 2; i++)
        {
            if (arr2[i].type == HashType::KEY)
            {
                Add(arr2[i].val);
                size--;
            }
        }
        for (int i = 0; i < MaxSize / 2; i++)
        {
            if (arr2[i].type == HashType::KEY)
            {
                assert(Has(arr2[i].val));
            }
        }
      
    }

public:
    HashTable(size_t _MaxSize = INITIAL_SIZE) : arr(_MaxSize), size(0),MaxSize(_MaxSize), cmp(C()),
                                                hasher1(H()), hasher2(H(second_default_hash_primmary)) {}

    bool Delete(const T &val)
    {
        size_t hash;
        for (int i = 0; i < MaxSize; i++)
        {
            hash = (hasher1(val) + i * (2*hasher2(val) + 1) ) % MaxSize; // Метод двойного хеширования
            if (arr[hash].type == HashType::KEY)
            {
                if (cmp(arr[hash].val, val))
                {
                    arr[hash].type = HashType::DEL;
                    size--;
                    return true;
                }
            }
            if (arr[hash].type == HashType::NIL)
            {
                return false;
            }
        }
        return false;
    }
    bool Has(const T &val)
    {
        size_t hash;
        for (int i = 0; i < MaxSize; i++)
        {
            hash = (hasher1(val) + i * (2*hasher2(val) + 1) ) % MaxSize; // Метод двойного хеширования
            if (arr[hash].type == HashType::KEY)
            {
                if (cmp(arr[hash].val, val))
                {
                    return true;
                }
            }
            if (arr[hash].type == HashType::NIL)
            {
                return false;
            }
        }
        return false;
    }
    bool Add(T &val)
    {
        if ((size / static_cast<double>(MaxSize)) >= 0.75)
            Domain_Expansion();


        size_t hash;
        int index_first_del = -1;
        for (int i = 0; i < MaxSize; i++)
        {
            hash = (hasher1(val) + i * (2*hasher2(val) + 1) ) % MaxSize; // Метод двойного хеширования
            if (arr[hash].type == HashType::KEY)
            {
                if (cmp(arr[hash].val, val))
                {
                    return false;
                }
            }
            if (arr[hash].type == HashType::DEL)
            {
                if (index_first_del == -1)
                {
                    index_first_del = hash;
                }
            }
            if (arr[hash].type == HashType::NIL)
            {
                if (index_first_del != -1)
                {
                    arr[index_first_del].val = val;
                    arr[index_first_del].type = HashType::KEY;
                    size++;
                    return true;
                }
                else
                {
                    arr[hash].val = val;
                    arr[hash].type = HashType::KEY;
                    size++;
                    return true;
                }
            }
        }
        arr[index_first_del].val = val;
        arr[index_first_del].type = HashType::KEY;
        size++;
        return true;
    }
};

class StringHasher
{
private:
    int primmary;

public:
    StringHasher(int pr = first_default_hash_primmary) : primmary(pr) {}
    size_t operator()(const std::string &str)
    {
        size_t hash = 0;
        for (int i = 0; i < str.size(); i++)
        {
            hash =  hash* primmary + str[i];
        }
        return hash;
    }
};

void test(int);

int main()
{
    test(100000);
    return 0;
    HashTable<std::string, std::equal_to<std::string>, StringHasher> StringSet;
    char action;
    std::string key;
    while (std::cin >> action >> key)
    {
        switch (action)
        {
        case '+':
            std::cout << ((StringSet.Add(key)) ? "OK" : "FAIL") << std::endl;
            break;
        case '-':
            std::cout << ((StringSet.Delete(key)) ? "OK" : "FAIL") << std::endl;
            break;
        case '?':
            std::cout << ((StringSet.Has(key)) ? "OK" : "FAIL") << std::endl;
            break;
        }
    }

    return 0;
}

#include <set>
#include <algorithm>
#include <string>
#include <vector>
#include <time.h>
using namespace std;

void test(int count)
{
    srand(2);
    const int string_size = 2;
    HashTable<std::string, std::equal_to<std::string>, StringHasher> StringSet;
    set<std::string> s;
    for (int i = 0; i < count; i++)
    {
        int op = rand() % 3;
        string key;
        for (int r = 0; r < string_size; r++)
            key += std::to_string((char)(rand() % ('z' - 'a') + 'a'));
        bool hashTableAnswer, setAnswer;
        if (op == 0) // добавление
        {
            setAnswer = !s.count(key);
            s.insert(key);
            hashTableAnswer = StringSet.Add(key);
            //assert(StringSet.Has(key));
        }
        else if (op == 1) // Проверка
        {
            setAnswer = s.count(key);
            hashTableAnswer = StringSet.Has(key);
        }
        else if (op == 2) // Удаление
        {
            setAnswer = s.count(key);
            s.erase(key);
            hashTableAnswer = StringSet.Delete(key);
            assert(!StringSet.Has(key));
        }
        if (hashTableAnswer != setAnswer)
        {
            vector<string> ops{"ADD", "CHECK", "DELETE"};
            cout << "Error @ iteration " << i << " operation " << ops[op] << endl;
        }
    }
}