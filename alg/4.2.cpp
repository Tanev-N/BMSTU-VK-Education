#include <iostream>
#include <random>



template <typename T, typename C = std::less<T>>
class Heap
{
    T *arr;
    C cmp;
    unsigned int size;
    unsigned int max_size;

    void Domain_expansion()
    {
        if (max_size == 0)
        {
            arr = new T[1];
            size = 1;
            max_size = 1;
        }
        else
        {
            T *arr_temp = new T[size * 2];
            for (int i = 0; i < size; i++)
            {
                arr_temp[i] = arr[i];
            }
            delete[] arr;
            arr = arr_temp;
            max_size *= 2;
        }
    }
    
    void siftDown(int i)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;
        if (left < size && cmp(arr[left], arr[largest]))
        {
            largest = left;
        }
        if (right < size && cmp(arr[right], arr[largest]))
        {
            largest = right;
        }
        if (largest != i)
        {
            T temp = arr[i];
            arr[i] = arr[largest];
            arr[largest] = temp;
            siftDown(largest);
        }
    }

    void siftUp(int i)
    {
        int parent = (i - 1) / 2;
        if (parent >= 0 && cmp(arr[i], arr[parent]))
        {
            T temp = arr[i];
            arr[i] = arr[parent];
            arr[parent] = temp;
            siftUp(parent);
        }
    }

public:
    Heap(C cmp_ = C())
    {
        arr = nullptr;
        size = 0;
        max_size = 0;
        cmp = cmp_;
    }

    Heap(T *a, int size_, C cmp_ = C())
    {
        arr = a;
        size = size_;
        max_size = size_;
        cmp = cmp_;
        for (int i = size / 2 - 1; i >= 0; i--)
        {
            siftDown(i);
        }
    }

    void print()
    {
        for (int i = 0; i < size; i++)
        {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    }

    int Size()
    {
        return size;
    }
    void Add(T x)
    {
        ++size;
        if (size >= max_size)
        {
            Domain_expansion();
        }
        arr[size - 1] = x;
        siftUp(size - 1);
    }

    T Top()
    {
        return arr[0];
    }

    T Extract()
    {
        T temp = arr[0];
        size--;
        arr[0] = arr[size];
        siftDown(0);
        return temp;
    }

    bool is_empty()
    {
        return size == 0;
    }

    ~Heap()
    {
        delete[] arr;
    }
};

struct UserFreq
{
    unsigned int user_id;
    unsigned int freq;
    UserFreq(unsigned int user_id_ = 0, unsigned int freq_ = 0) : user_id(user_id_), freq(freq_) {}
};

bool operator<=(const UserFreq &a, const UserFreq &b)
{
    return a.freq <= b.freq;
}


class User_Comparator
{

public:
    User_Comparator(){};
    bool operator()( const UserFreq &a, const UserFreq &b) 
    {
        return a.freq < b.freq;
    }
};



std::ostream &operator<<(std::ostream &out, const UserFreq &a)
{
    out << a.user_id;   //out << "user_id : " << a.user_id << " freq : " << a.freq << std::endl;
    return out;
}

std::istream &operator>>(std::istream &in,  UserFreq &a)
{
    in >> a.user_id;
    in >> a.freq;
    return in;
}



void deploy()
{
    unsigned int n;
    std::cin >> n;
    
    unsigned int k;
    std::cin >> k;

    User_Comparator cmp;
    Heap<UserFreq, User_Comparator> heap(cmp);


    for(int i = 0; i < k; ++i)
    {
        UserFreq temp;
        std::cin >> temp;
        heap.Add(temp);
    }


    for(int i = 0; i < n-k; ++i)
    {
        UserFreq temp;
        std::cin >> temp;
        heap.Add(temp);
        heap.Extract();
    }


    for (int i = 0; i < k; i++)
    {
        std::cout << heap.Extract() << " ";
    }
}


void test(int repeates)
{
    Heap<UserFreq, User_Comparator> heap;
    std::random_device rd;  // создаем случайное устройство для генерации случайных чисел
    std::mt19937 gen(rd()); // инициализируем генератор случайных чисел с помощью rd
    std::uniform_int_distribution<int> distribution_count_users(0, 10);
    std::uniform_int_distribution<int> distribution_freq_users(0, 100); // задаем диапазон для случайных чисел от 1 до 100
    for(int i = 0; i < repeates; i++)     //for (int i = 0; i < repeates; i++)
    {
        int num_id = 0;
        int user_count = distribution_count_users(gen);
        for (int i = 0; i < user_count; ++i)
        {
            int random_user_freq = distribution_freq_users(gen);
            UserFreq random_user(num_id, random_user_freq);
            heap.Add(random_user);
            num_id++;
        }
        int k = distribution_count_users(gen);
        while (k > user_count)
        {
            k = distribution_count_users(gen);
        }
        heap.print();
        std::cout << "k = " << k << std::endl;
       
        UserFreq *arr = new UserFreq[k];
        for(int i = 0; i < k; ++i)
        {
            arr[i] = heap.Extract();
            std::cout << arr[i] << " ";
        }
        
        bool flag = true;
        User_Comparator cmp;
        for (int i = 0; i < k-1; ++i)
        {
            if (! (arr[i] <= arr[i + 1] ))
            {
                flag = false;
                break;
            }
        }

        if (heap.is_empty() && flag == true)
        {
            std::cout << std::endl << "DONE" << std::endl;
        }
        else
        {
            std::cout << std::endl << "ERROR" << std::endl;
            return;
        }


        
    }
}
int main()
{
    
    //test(100);
    deploy();


    return 0;
}