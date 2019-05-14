#include <iostream>

int main(int argc, char **argv)
{
    double a = 0, b = 1;

    std::cin >> a >> b;
    std::cout.precision(16);
    std::cout << a / b;

    return 0;
}