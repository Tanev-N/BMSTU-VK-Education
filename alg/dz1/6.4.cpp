#include <iostream>
#include <random>

template <typename T, typename C = std::less<T>>
int particion(T *arr, int left, int right, C cmp = C())
{
    int pivot = left + rand() % (right - left + 1);
    std::swap(arr[left], arr[pivot]);
    T pivot_value = arr[left];

    int i = right;
    int j = right;

    while (true)
    {
        if (cmp(pivot_value, arr[j]))
        {
            std::swap(arr[i], arr[j]);
            i--;
        }
        j--;
        if (j <= left)
        {
            std::swap(arr[i], arr[left]);
            return i;
        }
    }
}

template <typename T, typename C = std::less<T>>
T Quickselect(T *arr, int left, int right, int k, C cmp = C())
{
    while(true)
    {
        int pivot_position = particion(arr, left, right, cmp);
        if (pivot_position == k)
        {
            return arr[pivot_position];
        }
        else if (pivot_position > k)
        {
            right = pivot_position - 1;
        }
        else
        {
            left = pivot_position + 1;
        }
    }
   
}

int main()
{
    int n;
    std::cin >> n;
    int *arr = new int[n];
    for (int i = 0; i < n; i++)
    {
        std::cin >> arr[i];
    }
    std::cout << Quickselect(arr, 0, n - 1, n / 10) << std::endl;
    std::cout << Quickselect(arr, 0, n - 1, n / 2) << std::endl;
    std::cout << Quickselect(arr, 0, n - 1, n * 9 / 10) << std::endl;

    delete[] arr;
    return 0;
}