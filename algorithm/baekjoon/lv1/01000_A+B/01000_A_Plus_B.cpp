#include <iostream>

int main(int argc, char **argv)
{
    int a = 0, b = 0;
    
    std::cin >> a >> b;
    std::cin.ignore();
    
    std::cout << a + b;
    return 0;
}