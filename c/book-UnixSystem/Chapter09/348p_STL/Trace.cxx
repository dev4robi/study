#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include "Config.h"
using namespace std;

int main(int argc, char **argv)
{
    if (Config::instance()->init())
    {
        cout << "MAIN() : Config 객체의 init() 메소드 실행 실패" << endl;
        return -1;
    }

    // 방법 1
    const char *traceVal = Config::instance()->get_valueFromMap("TRACE").c_str();
    if (!strncmp(traceVal, "ON", 2))
    {
        cout << "TRACE is ON" << endl;
    }

    // 방법 2
    if ((Config::instance()->get_valueFromMap("TRACE")).find("ON") != string::npos)
    {
        cout << "TRACE is ON" << endl;
    }

    return 0;
}