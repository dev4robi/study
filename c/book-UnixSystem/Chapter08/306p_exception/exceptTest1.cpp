#include <iostream>
#include <stdexcept>
using namespace std;

int divide(int a, int b)
{
    return (a / b);
}

int main()
{
    int a = 10, b = 0;
    cout << "A / B 결과 : " << divide(a, b) << endl;
    return 0;
}