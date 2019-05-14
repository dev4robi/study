#include <iostream>
#include <string>

using namespace std; // std 네임스페이스 사용

int main(void)
{
    string inputData;
    int outputLine = 0;

    // quit을 입력받을 때 까지 화면에 입력된 내용을 출력
    while (inputData != "quit")
    {
        cout << "데이터 입력(종료-quit): ";
        cin >> inputData;
        cout << ++outputLine << ": " << inputData << endl;
        fprintf(stdout, "%x", inputData.c_str()[0]);
    }

    return 0;
}