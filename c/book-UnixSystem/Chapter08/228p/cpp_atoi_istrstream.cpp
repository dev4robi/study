#include <iostream>
#include <strstream>
using namespace std;

int main()
{
    // 문자열 데이터를 선언
    char *strData = "123";

    // 입력 스트림 정의
    istrstream strToInt(strData);

    // 정수 변수를 선언한 뒤, 스트링 스트림으로부터 데이터 입력
    int intData;
    strToInt >> intData;

    // 결과 값 출력
    cout << "1.INT DATA - 10: " << intData - 10 << endl;
    printf("2.INT DATA - 10: %d\n", intData - 10);

    cout << "1.STR DATA: " << strData << endl;
    printf("2.STR DATA: %s\n", strData);

    return 0;
}