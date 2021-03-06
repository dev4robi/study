#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "파일명을 두개 입력하세요." << endl;
        exit(1);
    }

    // 명령 인자의 파일명을 이용하여 바이너리 모드의 입출력용으로 각각 오픈
    ifstream srcFile(argv[1], ios::binary);
    ofstream dstFile(argv[2], ios::binary);

    // 파일 열기 실패 체크
    if (srcFile.fail() || dstFile.fail())
    {
        cerr << "파일 열기 실패" << endl;
        exit(1);
    }

    char data[256];

    // 파일의 끝에 도달할 때 까지 계속 복사
    while (!srcFile.eof())
    {
        srcFile.read(data, sizeof(data));
        dstFile.write(data, sizeof(data));
    }

    // 파일 닫기
    srcFile.close();
    dstFile.close();

    return 0;
}