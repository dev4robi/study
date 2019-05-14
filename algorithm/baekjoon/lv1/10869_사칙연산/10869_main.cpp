#include <iostream>

int main(int argc, char **argv)
{
    int a = 0, b = 1;

    std::cin >> a >> b;
    std::cout << a + b << std::endl;
    std::cout << a - b << std::endl;
    std::cout << a * b << std::endl;
    std::cout << a / b << std::endl;
    std::cout << a % b << std::endl;

    return 0;
}