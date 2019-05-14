#include <iostream>
#include <stdlib.h>
using namespace std;

int main(void)
{
    // 입력 받을 데이터와 io 상태를 체크할 변수 선언
    char data;
    long istate;
    long ostate;

    // End Of File (eof) 일때까지 cin.get()을 수행
    while (!cin.eof())
    {
        data = cin.get();

        // cin.fail() 체크
        if (cin.fail())
        {
            istate = cin.rdstate();
            cerr << "istate 에러검출, 값: " << istate << endl;
        }

        // cout.put(toupper())을 통해 대문자로 변환
        cout.put((char)toupper(data));

        // cout.fail() 체크
        if (cout.fail())
        {
            ostate = cout.rdstate();
            cerr << "ostate 에러검출, 값: " << ostate << endl;
        }
    }

    return 0;
}