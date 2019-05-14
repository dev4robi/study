#include <iostream>

int main(int argc, char **argv)
{
    int a = 1, b = 1, c = 1;

    std::cin >> a >> b >> c;
    std::cout << (a + b) % c << std::endl;
    std::cout << (a % c + b % c) % c << std::endl;
    std::cout << (a * b) % c << std::endl;
    std::cout << (a % c * b % c) % c;

    return 0;
}