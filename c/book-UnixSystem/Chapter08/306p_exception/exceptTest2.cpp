#include <iostream>
#include <stdexcept>
using namespace std;

int divide(int a, int b)
{
    if (b == 0)
    {
        exception e;
        throw e;
    }
    
    return (a / b);
}

int main()
{
    int a = 10, b = 0;

    try
    {
        cout << "A / B 결과 : " << divide(a, b) << endl;
    }
    catch (exception &std_ex)
    {
        cout << "예외 상황 발생, b를 1로 처리함" << endl;
        b = 1;
        cout << "예외 처리후의 A / B 결과 : " << divide(a, b) << endl;
    }
    
    return 0;
}