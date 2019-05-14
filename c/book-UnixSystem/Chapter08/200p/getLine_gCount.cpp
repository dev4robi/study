#include <iostream>
using namespace std;

int main(void)
{
    char data[64];

    cout << "현재 세팅된 플래그: " << showbase << hex << cout.flags() << endl;
    cout << dec;

    cout << "문자열 입력(':'로 입력 완성): ";
    cin.getline(data, sizeof(data), ':');
    cout << "입력 문자: " << data << endl;
    cout << "문자 개수: " << cin.gcount() << endl;
    
    cout << endl << "=================================" << endl << endl;

    cout << "문자 입력(앞의 6문자 무시): ";
    cin.ignore(6);
    cin.getline(data, sizeof(data));
    cout << "입력 문자: " << data << endl;
    cout << "문자 개수: " << cin.gcount() << endl;

    return 0;
}