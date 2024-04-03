#include <iostream>

int bynary_search(int a[], int left_, int right_, int num)
{
    while (left_ <= right_)
    {
        int middle = (left_ + right_) / 2;
        if (a[middle] == num)
        {
            return middle;
        }
        else if (a[middle] > num)
        {
            right_ = middle - 1;
        }
        else
        {
            left_ = middle + 1;
        }
    }

    return right_ + 1;
}

int exp_search(int a[], int size, int num)
{
    int left = 0;
    int right = 1;
    while (a[right] < num && right < size)
    {
        left = right;
        right *= 2;
    }

    if (right >= size)
    {
        right = size - 1;
    }

    return bynary_search(a, left, right, num);
}

int main()
{
    int n, m;

    std::cin >> n >> m;

    int *a = new int[n];
    int *b = new int[m];

    for (int i = 0; i < n; i++)
    {
        std::cin >> a[i];
    }
    for (int i = 0; i < m; i++)
    {
        std::cin >> b[i];
    }

    for (int i = 0; i < m; i++)
    {
        std::cout << exp_search(a, n, b[i]) << " ";
    }

    delete a;
    delete b;

    return 0;
}