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




int main() {

    int n;

    std::cin >> n;

    int *a = new int[n];

    for(int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    int k;

    std::cin >> k;
    std::cout << bynary_search(a, 0, n - 1, k) << std::endl;

    delete[] a;

    return 0;
}