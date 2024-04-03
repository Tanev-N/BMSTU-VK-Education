#include <iostream>



int main()
{
    unsigned long long number;

    std::cin >> number; 

    int k; // bit's order number

    std::cin >> k;

    number = number ^ (1 << (k)); // invert k's bit

    std::cout << number << std::endl;


    return 0;

}