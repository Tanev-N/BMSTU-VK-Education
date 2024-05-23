#include <iostream>
#include <random>


class Deque
{
    int *arr;
    unsigned int max_size;
    unsigned int size;
    unsigned int left;
    unsigned int right;

    void Domain_Expansion()
    {

        if (max_size == 0)
        {
            arr = new int[1];
            max_size = 1;
            return;
        }

        int *arr_temp = new int[max_size * 2];
        for (int i = 0; i < max_size; i++)
        {
            arr_temp[i] = arr[(left + i) % max_size];
        }
        left = 0;
        right = max_size - 1;

        delete arr;
        arr = arr_temp;
        max_size *= 2;
    }

public:
    Deque(int n)
    {
        max_size = n;
        size = 0;
        arr = new int[max_size];
        left = 0;
        right = 0;
    }

    int get_size()
    {
        return size;
    }

    Deque()
    {
        max_size = 0;
        size = 0;
        arr = nullptr;
        left = 0;
        right = 0;
    }

    void push_back(int x)
    {
        if (size == max_size)
        {
            Domain_Expansion();
        }
        right = (right + 1) % max_size;
        arr[right] = x;
        size++;
    }

    void push_front(int x)
    {
        if (size == max_size)
        {
            Domain_Expansion();
        }
        left = (left - 1 + max_size) % max_size;
        arr[left] = x;
        size++;
    }

    int pop_back()
    {
        if (size == 0)
        {
            return -1;
        }
        int num = arr[right];
        right = (right - 1 + max_size) % max_size;
        --size;
        return num;
    }

    int pop_front()
    {
        if (size == 0)
        {
            return -1;
        }
        int num = arr[left];
        left = (left + 1) % max_size;
        --size;
        return num;
    }

    Deque(const Deque &other)
    {
        max_size = other.max_size;
        arr = new int[max_size];
        for (unsigned int i = 0; i < max_size; i++)
        {
            arr[i] = other.arr[i];
        }
        left = other.left;
        right = other.right;
    }

    ~Deque()
    {
        delete[] arr;
    }
};

// std::ostream operator <<(std::ostream &out, const Deque &a)
// {
//     for 
//     return out;
// }

void process_commands()
{
    int n;
    std::cin >> n;

    Deque deque;
    bool is_correct = true;
    for (int i = 0; i < n; i++)
    {
        int command;
        int num;
        std::cin >> command;
        std::cin >> num;
        switch (command)
        {
        case 1:
            deque.push_front(num);
            break;
        case 2:
        {
            int front_num = deque.pop_front();
            if (front_num != num)
            {
                is_correct = false;
            }
        }
        break;
        case 3:
            deque.push_back(num);
            break;
        case 4:
        {
            int back_num = deque.pop_back();
            if (back_num != num)
            {
                is_correct = false;
            }
        }
        break;
        default:
            break;
        }
    }

    if (is_correct)
    {
        std::cout << "YES" << std::endl;
    }
    else
    {
        std::cout << "NO" << std::endl;
    }
}


// void test()
// {
//     std::random_device rd;  // создаем случайное устройство для генерации случайных чисел
//     std::mt19937 gen(rd()); // инициализируем генератор случайных чисел с помощью rd
//     std::uniform_int_distribution<int> distribution_count_command(0, 10);
//     std::uniform_int_distribution<int> distribution_number_of_command(1, 4);
//     std::uniform_int_distribution<int> distribution_number(-100, 100);


//     int random_count_command = distribution_count_command(gen);

//     Deque deque;
//     bool is_correct = true;
//     for (int i = 0; i < random_count_command; i++)
//     {
//         int command = distribution_number_of_command(gen);
//         int num = distribution_number(gen);
//         switch (command)
//         {
//         case 1:
//             deque.push_front(num);
//             break;
//         case 2:
//         {
//             int front_num = deque.pop_front();
//             if (front_num != num)
//             {
//                 is_correct = false;
//             }
//         }
//         break;
//         case 3:
//             deque.push_back(num);
//             break;
//         case 4:
//         {
//             int back_num = deque.pop_back();
//             if (back_num != num)
//             {
//                 is_correct = false;
//             }
//         }
//         break;
//         default:
//             break;
//         }
//     }

//     if (is_correct)
//     {
//         std::cout << "YES" << std::endl;
//     }
//     else
//     {
//         std::cout << "NO" << std::endl;
//     }
// }



int main()
{

    process_commands();


    return 0;
}
