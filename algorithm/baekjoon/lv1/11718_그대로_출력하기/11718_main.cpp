#include <iostream>
#include <cstring>

int main(int argc, char **argv)
{
    std::string inStr(std::istreambuf_iterator<char>(std::cin), std::istreambuf_iterator<char>());
    
    std::cout << inStr;
    
    return 0;
}