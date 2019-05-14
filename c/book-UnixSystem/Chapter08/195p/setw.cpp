#include <iostream>
#include <iomanip>
using namespace std;

int main(void)
{
    int data = 0;
    int width = 0;

    // setfill 조작자로 setw가 공백 대신 '-'로 채울 수 있도록 변경
    cout << setfill('-');

    // setw 조작자를 이용하여 출력 너비 조절
    for (int loop = 0; loop < 11; ++loop)
    {
        if (loop < 6)
        {
            cout << data << setw(++width) << data << endl;
        }
        else
        {
            cout << data << setw(--width) << data << endl;
        }
    }

    cout << "======================================================" << endl;

    // setprecision 조작자로 소수점 출력
    float testFloat = 10.0 / 3.0;

    cout << testFloat << endl;

    for (int count = 0; count < 10; ++count)
    {
        cout << setprecision(count) << testFloat << endl;
    }

    return 0;
}