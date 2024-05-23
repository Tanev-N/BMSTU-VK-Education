// Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется
//  в правое поддерево root; иначе в левое поддерево root.
// Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.

// 2_1. Выведите элементы в порядке in-order (слева направо)

#include <iostream>
#include <stack>

template <typename T, typename C = std::less<T>>
class BynaryTree
{
    class Node
    {
    public:
        T data;
    
        Node *left;
        Node *right;
        Node(T _data) : data(_data), left(nullptr), right(nullptr) {}
    };
    Node *root;
    C cmp;

public:
    BynaryTree(C _cmp = C()) : cmp(_cmp), root(nullptr) {}

    void Add(T data)
    {
        if (root == nullptr)
        {
            root = new Node(data);
        }
        else
        {
            Node *cur = root;
            while (true)
            {
                if (cmp(data, cur->data))
                {
                    if (cur->left == nullptr)
                    {
                        cur->left = new Node(data);
                        break;
                    }
                    else
                    {
                        cur = cur->left;
                    }
                }
                else
                {
                    if (cur->right == nullptr)
                    {
                        cur->right = new Node(data);
                        break;
                    }
                    else
                    {
                        cur = cur->right;
                    }
                }
            }
        }
    }

    bool Has(T data)
    {
        Node *cur = root;
        while (cur != nullptr)
        {
            if (cmp(data, cur->data))
            {
                cur = cur->left;
            }
            else if (cmp(cur->data, data))
            {
                cur = cur->right;
            }
            else
            {
                return true;
            }
        }
        return false;
    }

    void Delete(T data)
    {
        if (!Has(data))
        {
            return;
        }
        Node *cur = root;
        Node *parent = nullptr;
        while (cur != nullptr)
        {
            if (cmp(data, cur->data))
            {
                parent = cur;
                cur = cur->left;
            }
            else if (cmp(cur->data, data))
            {
                parent = cur;
                cur = cur->right;
            }
            else
            {
                break;
            }
        }

        if (cur->left == nullptr && cur->right == nullptr)
        {
            if (parent == nullptr)
            {
                delete root;
                root = nullptr;
            }
            else if (cmp(data, parent->data))
            {
                delete parent->left;
                parent->left = nullptr;
            }
            else
            {
                delete parent->right;
                parent->right = nullptr;
            }
        }
        else if (cur->left == nullptr)
        {
            if (parent == nullptr)
            {
                Node *temp = root;
                root = cur->right;
                delete temp;
            }
            else if (cmp(data, parent->data))
            {
                delete parent->left;
                parent->left = cur->right;
            }
            else
            {
                delete parent->right;
                parent->right = cur->right;
            }
        }
        else if (cur->right == nullptr)
        {
            if (parent == nullptr)
            {
                Node *temp = root;
                root = cur->left;
                delete temp;
            }
            else if (cmp(data, parent->data))
            {
                delete parent->left;
                parent->left = cur->left;
            }
            else
            {
                delete parent->right;
                parent->right = cur->left;
            }
        }
        else
        {
            Node *min = cur->right;
            Node *min_parent = cur;
            while (min->left != nullptr)
            {
                min_parent = min;
                min = min->left;
            }
            cur->data = min->data;
            if (min->right != nullptr)
            {
                if (min_parent != cur)
                {
                    min_parent->left = min->right;
                }
                else
                {
                    cur->right = min->right;
                }

                delete min;
            }
            else
            {
                if (min_parent != cur)
                {
                    min_parent->left = nullptr;
                }
                else
                {
                    cur->right = nullptr;
                }
                delete min;
            }
        }
    }

    void print_in_order()
    {
        std::stack<Node *> st;

        Node *cur = root;

        while (cur != nullptr || !st.empty())
        {
            while (cur != nullptr)
            {
                st.push(cur);
                cur = cur->left;
            }

            cur = st.top();
            st.pop();

            std::cout << cur->data << " ";

            cur = cur->right;
        }
    }

    ~BynaryTree()
    {
        std::stack<Node *> st;

        Node *cur = root;

        while (cur != nullptr || !st.empty())
        {
            while (cur != nullptr)
            {
                st.push(cur);
                cur = cur->left;
            }

            cur = st.top();
            st.pop();

            while (cur != nullptr)
            {
                st.push(cur);
                cur = cur->right;
            }

            cur = st.top();
            st.pop();
            if (!st.empty())
            {
                Node *predoc = st.top();
                if (predoc->left == cur)
                {
                    predoc->left = nullptr;

                    delete cur;
                }
                else
                {
                    predoc->right = nullptr;
                    delete cur;
                }

                cur = st.top();
                st.pop();
            }
            else
            {
                delete cur;
                cur = nullptr;
            }
        }
    }
};

int main()
{

    BynaryTree<int> tree;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++)
    {
        int x;
        std::cin >> x;
        tree.Add(x);
    }

    //    std::cout << "Delete : ";
    //    std::cin >> n;
    //
    //
    //    for (int i = 0; i < n; i++)
    //    {
    //        int x;
    //        std::cin >> x;
    //        tree.Delete(x);
    //    }

    tree.print_in_order();
}