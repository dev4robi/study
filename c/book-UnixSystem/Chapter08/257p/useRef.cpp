#include <iostream>
using namespace std;

void myswap(int &srcVal, int &destVal)
{
    int tempVal = srcVal;
    srcVal = destVal;
    destVal = tempVal;
}

int main(void)
{
    int a = 10, b = 5;

    cout << "before swap - a: " << a << " / " << "b: " << b << endl;

    myswap(a, b);

    cout << "after swap - a: " << a << " / " << "b: " << b << endl;

    return 0;
}