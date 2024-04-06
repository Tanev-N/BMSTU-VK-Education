#include <iostream>
#include <string>

template <typename T>
class Stack
{
    T *data;
    int size;
    int maxSize;

    void Domain_expansion()
    {
        if (maxSize == 0)
        {
            data = new T[1];
            size = 1;
            maxSize = 1;
        }
        else
        {
            T *data_temp = new T[size * 2];
            for (int i = 0; i < size; i++)
            {
                data_temp[i] = data[i];
            }
            delete[] data;
            data = data_temp;
            maxSize *= 2;
        }
    }

public:
    Stack()
    {
        size = 0;
        maxSize = 0;
        data = nullptr;
    }

    void push(T value)
    {
        ++size;
        if (size >= maxSize)
        {
            Domain_expansion();
        }
        data[size - 1] = value;
    }

    T pull()
    {
        --size;
        return data[size];
    }

    int get_size()
    {
        return size;
    }

    T top() const
    {
        if (size == 0)
        {
            throw std::out_of_range("Stack is empty");
        }
        return data[size - 1];
    }
};

bool isAnagram(std::string s1, std::string s2)
{

    if (s1.length() != s2.length())
    {
        return false;
    }

    Stack<char> stack;
    int i = 0, j = 0;

    while (i < s1.length())
    {
        stack.push(s1[i]);

        while (stack.get_size() > 0 && stack.top() == s2[j])
        {
            stack.pull();
            j++;
        }

        i++;
    }

    return j == s2.length();
}

int main()
{

    std::string s1, s2;
    std::cin >> s1;
    std::cin >> s2;

    if (isAnagram(s1, s2))
    {
        std::cout << "YES";
    }
    else
    {
        std::cout << "NO";
    }

    return 0;
}