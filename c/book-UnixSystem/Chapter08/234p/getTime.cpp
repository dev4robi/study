#include <iostream>
#include <strstream>
#include <ctime>
using namespace std;

void getTime(char *currentTime, int bufSize)
{
    struct tm _tm;
    time_t ctime;

    ctime = time(NULL);
    strftime(currentTime, bufSize, "%T", localtime(&ctime));
}

int main(void)
{
    char currentTime[9];
    char data[256];
    ostrstream dataStream(data, sizeof(data));

    const char *newData = "중요데이터";
    getTime(currentTime, sizeof(currentTime));

    dataStream << currentTime << ": " << newData;
    cout << data << endl;

    return 0;
}