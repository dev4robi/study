#include <iostream>
using namespace std;

int main(void)
{
    // 입력 받을 데이터와 io 상태를 체크할 변수 선언
    char data[256];
    long istate;
    int line_number = 0;

    // EOF 일때까지 cin.get() 수행
    while (!cin.eof())
    {
        cin.getline(data, sizeof(data));

        // cin.fail() 체크, 에러가 없으면 라인넘버 붙여 출력
        if (cin.fail())
        {
            istate = cin.rdstate();
            cerr << "istate 에러 검출, 값: " << istate << endl;
        }
        else
        {
            ++line_number;
            cout << line_number << '\t';
            cout << data << endl;
        }
    }

    return 0;
}