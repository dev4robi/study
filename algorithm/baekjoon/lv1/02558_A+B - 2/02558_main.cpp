#include <iostream>

int main(int argc, char **argv)
{
    int a = 0, b = 0;
    
    std::cin >> a;
    std::cin.ignore();
    std::cin >> b;
    std::cout << a + b;

    return 0;
}