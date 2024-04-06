#include <iostream>

template <typename T, typename C = std::less<T>>
void merge(T *arr, int left, int middle, int right, C cmp = C())
{
    T *arr_temp = new T[right - left + 1];
    int right_temp = middle + 1;
    int left_temp = left;
    int i = 0;
    while (right_temp <= right || left_temp <= middle)
    {
        if (right_temp == right + 1)
        {
            arr_temp[i] = arr[left_temp];
            left_temp++;
        }
        else if (left_temp == middle + 1)
        {
            arr_temp[i] = arr[right_temp];
            right_temp++;
        }
        else
        {
            if (cmp(arr[left_temp], arr[right_temp]))
            {
                arr_temp[i] = arr[left_temp];
                left_temp++;
            }
            else
            {
                arr_temp[i] = arr[right_temp];
                right_temp++;
            }
        }
        i++;
    }
    for (int i = 0; i < right - left + 1; i++)
    {
        arr[left + i] = arr_temp[i];
    }
    delete[] arr_temp;
}

template <typename T, typename C = std::less<T>>
void merge_sort(T *arr, int left, int right, C cmp = C())
{

    if (left >= right)
    {
        return;
    }

    int middle = left + (right - left) / 2;

    merge_sort(arr, left, middle, cmp);
    merge_sort(arr, middle + 1, right, cmp);

    merge(arr, left, middle, right, cmp);
}


struct box
{
    int number;
    int width;
    int height;
    int depth;
    box(int _number, int _width, int _height, int _depth) : number(_number), width(_width), height(_height), depth(_depth) {}
    box() : number(0), width(0), height(0), depth(0) {}
};

class box_cmp
{

public:
    box_cmp() {}
    bool operator()(box a, box b)
    {
        bool dont_rool = (a.depth < b.depth && a.height < b.height && a.width < b.width);
        bool swap_height_and_depth = (a.height < b.depth && a.depth < b.height && a.width < b.width);
        bool swap_width_and_depth = (a.width < b.depth && a.height < b.height && a.depth < b.width);
        bool swap_height_and_width = (a.depth < b.depth && a.width < b.height && a.height < b.width);
        return ( dont_rool || swap_height_and_depth || swap_width_and_depth || swap_height_and_width );
    }

};


int main() {
    int n;

    std::cin >> n;

    box *boxes = new box[n];

    for (int i = 0; i < n; i++)
    {
        int width, height, depth;
        std::cin>> width >> height >> depth;
        boxes[i] = box(i, width, height, depth);
    }

    merge_sort(boxes, 0, n - 1, box_cmp());
    
    for (int i = 0; i < n; i++)
    {
        std::cout << boxes[i].number << " ";
    }
}