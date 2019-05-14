#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char **argv)
{
    std::vector<int> vecInput;

    int number = 0;

    for (int i = 0; i < 3; ++i)
    {
        std::cin >> number;
        vecInput.push_back(number);
    }

    std::sort(vecInput.begin(), vecInput.end(), std::greater<int>());
    std::cout << vecInput[1]; // 두 번째로 큰 정수 출력

    return 0;
}