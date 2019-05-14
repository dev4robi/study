#include <iostream>
#include <fstream>
using namespace std;

int main(void)
{
    // inout.file 파일을 읽고 쓰기 모드로 열기
    fstream inoutFile("inout.file", ios::in | ios::out);

    if (inoutFile.fail())
    {
        cerr << "파일 열기 실패" << endl;
        exit(1);
    }

    // 파일 포인터와 토큰으로 활용할 변수 선언
    int pointer = 0;
    char token;

    while (!inoutFile.eof())
    {
        // 파일을 읽어나가다 토큰(:)을 만나면 -를 입력
        token = inoutFile.get();

        if (token == ':')
        {
            inoutFile.seekp(pointer, ios::beg);
            inoutFile << '-';
        }

        pointer++;
    }

    // 파일 닫기
    inoutFile.close();
}