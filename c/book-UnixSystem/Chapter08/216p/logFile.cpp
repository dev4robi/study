#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

// 로그 파일에 로그를 저장하는 함수
int putLog(char *logData)
{
    // Log.txt 파일을 추가 모드로 열기
    ofstream logFile("Log.txt", ios::app);

    if (!logFile.fail())
    {
        // 날짜와 시간 그리고 데이터를 파일에 입력
        char date[9], time[9];

        // 원래 이 함수였는데, 컴파일 오류를 보니 g++ 버전 혹은 UNIX <-> LINUX 표준 문제인듯
        //logFile << _strdate(date) << "," << _strtime(time) << '\t' << logData << endl;
        logFile << getdate(date) << "," << "22:04:05" << '\t' << logData << endl;
        logFile.close();
    }
    else
    {
        cerr << "로그 파일 열기 실패" << endl;
        return 0;
    }

    return 1;
}

int main(void)
{
    // 로그에 데이터를 입력
    if (!putLog("첫 번째 로그 저장"))
    {
        cerr << "첫 번째 로그 저장 실패" << endl;
    }
    if (!putLog("두 번째 로그 저장"))
    {
        cerr << "두 번째 로그 저장 실패" << endl;
    }
    if (!putLog("세 번째 로그 저장"))
    {
        cerr << "세 번째 로그 저장 실패" << endl;
    }
}