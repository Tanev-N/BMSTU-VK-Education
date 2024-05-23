// Дано число N и N строк. Каждая строка содержащит команду добавления или удаления натуральных чисел,
// а также запрос на получение k-ой порядковой статистики. Команда добавления числа A задается
//  положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
//  Запрос на получение k-ой порядковой статистики задается числом k. Требуемая скорость выполнения запроса - O(log n).

#include <iostream>
#include <stdlib.h>

template<typename T, typename C = std::less<T>>
class AvlTree {
    struct Node {
        Node(const T &data = T())
                : data(data), left(nullptr), right(nullptr), height(1), count(1) {
        }

        T data;
        Node *left;
        Node *right;
        size_t height;
        int count;
    };

public:
    AvlTree()
            : root(nullptr), cmp(C()) {
    }

    ~AvlTree() {
        destroyTree(root);
    }

    void Add(const T &data) {
        root = addInternal(root, data);
    }

    bool Has(const T &data) {
        Node *tmp = root;
        while (tmp) {
            if (cmp(tmp->data, data))
                tmp = tmp->right;
            else if (cmp(data, tmp->data))
                tmp = tmp->left;
            else
                return true;
        }
        return false;
    }

    void Delete(const T &data) {
        root = deleteInternal(root, data);
    }

    T SearchKth(int k) {
        if (k >= getCount(root)) {
            return -1; //TODO: throw exception
        }
        if (getCount(root) == 1)
            return root->data;
        return TheTruthKth(k, root);
    }

private:

    T TheTruthKth(int k, Node *node) {
        if (k > getCount(node->left)) {
            if (node->right != nullptr) {
                return TheTruthKth(k - getCount(node->left) - 1, node->right);
            } else {
                return TheTruthKth(k, node->left);
            }
        } else if (k < getCount(node->left)) {

            if (node->left != nullptr) {
                return TheTruthKth(k, node->left);
            } else {
                return TheTruthKth(k - getCount(node->left) - 1, node->right);
            }
        } else {
            return node->data;
        }

    }

    void destroyTree(Node *node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node *deleteInternal(Node *node, const T &data) {
        if (!node)
            return nullptr;
        if (cmp(node->data, data))
            node->right = deleteInternal(node->right, data);
        else if (cmp(data, node->data))
            node->left = deleteInternal(node->left, data);
        else {
            Node *left = node->left;
            Node *right = node->right;

            delete node;

            Node *result;

             if (!right)
                 return left;
             if (!left)
                 return right;

             if (right == nullptr && left == nullptr)
             {
                 return nullptr;
             }
            if (getHeight(right) < getHeight(left)) {
                result = findAndRemoveMin(right);
                result->left = left;
            } else {
                result = findAndRemoveMax(left);
                result->right = right;
            }


            return doBalance(result);
        }

        return doBalance(node);
    }

    Node *findMin(Node *node) {
        while (node->left)
            node = node->left;
        return node;
    }

    Node *removeMin(Node *node) {
        if (!node->left)
            return node->right;
        node->left = removeMin(node->left);
        return doBalance(node);
    }

    Node *findMax(Node *node) {
        while (node->right)
            node = node->right;
        return node;
    }

    Node *removeMax(Node *node) {
        if (!node->right)
            return node->left;
        node->right = removeMax(node->right);
        return doBalance(node);
    }

    Node *findAndRemoveMin(Node *right) {
        Node *node_ = findMin(right);
        node_->right = removeMin(right);
        return node_;
    }

    Node *findAndRemoveMax(Node *left) {
        Node *node_ = findMax(left);
        node_->left = removeMax(left);
        return node_;
    }

    Node *addInternal(Node *node, const T &data) {
        if (!node)
            return new Node(data);
        if (node->data <= data)
            node->right = addInternal(node->right, data);
        else
            node->left = addInternal(node->left, data);

        return doBalance(node);
    }

    size_t getHeight(Node *node) {
        return node ? node->height : 0;
    }

    int getCount(Node *node) {
        return node ? node->count : 0;
    }

    void fixHeight(Node *node) {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    void fixCount(Node *node) {
        node->count = 1 + getCount(node->left) + getCount(node->right);
    }

    int getBalance(Node *node) {
        return getHeight(node->right) - getHeight(node->left);
    }

    Node *rotateLeft(Node *node) {
        Node *tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixHeight(tmp);
        fixCount(node);
        fixCount(tmp);
        return tmp;
    }

    Node *rotateRight(Node *node) {
        Node *tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixHeight(tmp);
        fixCount(node);
        fixCount(tmp);
        return tmp;
    }

    Node *doBalance(Node *node) {
        fixHeight(node);
        fixCount(node);
        switch (getBalance(node)) {
            case 2: {
                if (getBalance(node->right) < 0)
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2: {
                if (getBalance(node->left) > 0)
                    node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

    C cmp;
    Node *root;
};

int main(int argc, const char *argv[]) {
    int n;
    std::cin >> n;
    AvlTree<int> tree;

    for (int i = 0; i < n; i++) {
        int num; //= rand() % 1000 - rand() % 912;

        int k; // rand() % 100;

        std::cin >> num;
        std::cin >> k;
        // if (i == 510)
        // {
        //     std::cout << "lol" << std::endl;
        // }
        if (num > 0)
            tree.Add(num);
        else
            tree.Delete(-num);

        std::cout << tree.SearchKth(k) << std::endl;
    }

    return 0;
}