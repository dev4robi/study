#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
    // 파일명을 입력하지 않으면 종료
    if (argc < 2)
    {
        cerr << "파일명을 입력하세요." << endl;
        exit(1);
    }

    // 명령 라인에 입력된 파일을 이용하여 ifstream 객체 생성
    ifstream inputFile(argv[1]);

    // 파일 열기에 문제가 없으면 입력 연산을 수행
    if (!inputFile.fail())
    {
        char lineData[256];
        char lineNum = 0;

        // 파일의 끝에 도달할 때 까지 getline을 이용하여 파일 읽기
        while (!inputFile.eof())
        {
            inputFile.getline(lineData, sizeof(lineData));

            // 파일 읽기에 성공하면 화면에 출력
            if (inputFile.good())
            {
                cout << lineData << endl;

                // 10행이 될 때마다 사용자 입력 대기
                lineNum++;
                
                if (lineNum % 10 == 0)
                {
                    cout << "엔터키 입력..." << endl;
                    cin.get();
                }
            }
        }
    }
    else
    {
        cerr << "파일 열기 실패!" << endl;
        exit(1);
    }

    // 모든 작업이 끝났으면 파일을 닫음
    inputFile.close();
}