#include <iostream>
#include <fstream>
using namespace std;

int main(void)
{
    // bufFile선언 및 bufferFile.txt 파일 열기
    filebuf bufFile;
    bufFile.open("bufferFile.txt", ios::in);

    // 파일 열기에 실패하면 프로그램 종료
    if (!bufFile.is_open())
    {
        cerr << "파일 열기 실패" << endl;
        exit (1);
    }

    // 현재 문자가 EOF가 아니면, 다음 문자를 계속 가져오면서 화면에 출력
    int current;
    while ((current = bufFile.sgetc()) != EOF)
    {
        cout.put((char)current);
        bufFile.snextc();
    }

    // bufFile을 닫기
    bufFile.close();
    return 0;
}