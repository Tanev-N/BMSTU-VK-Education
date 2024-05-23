#include <iostream>



int partition(unsigned long long arr[], int left, int right, unsigned long long mask) {
    int i = left, j = right;
    while (i <= j) {
        while (i <= j && !(arr[i] & mask)) i++;
        while (i <= j && (arr[j] & mask)) j--;
        if (i < j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    return i;
}

void msdBitSort(unsigned long long arr[], int left, int right, int bit) {
    if (left >= right || bit < 0) return;

    unsigned long long mask = 1ULL << bit;
    int pivot = partition(arr, left, right, mask);

    msdBitSort(arr, left, pivot - 1, bit - 1);
    msdBitSort(arr, pivot, right, bit - 1);
}

void sortArrayByBits(unsigned long long *arr, int n) {
    int maxBit = 63; 
    msdBitSort(arr, 0, n - 1, maxBit);
}

int main() {
    int n;
    std::cin >> n;
    unsigned long long *a = new unsigned long long[n];
    for(int i = 0; i<n; i++)
    {
    	std::cin >> a[i];
    }
    sortArrayByBits(a, n);

    for (int i = 0; i < n; ++i) {
        std::cout << a[i] << " ";
    }
	delete[] a;
    return 0;
}