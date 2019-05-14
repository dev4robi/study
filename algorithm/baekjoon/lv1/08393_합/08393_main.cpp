#include <iostream>

int main(int argc, char **argv)
{
    int n = 0, sum = 0;

    std::cin >> n;

    for (int i = n; i > 0; --i)
    {
        sum += i;
    }

    std::cout << sum;
    
    return 0;
}