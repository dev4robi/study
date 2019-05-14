/* Config OBJECT CXX file */
#include <iostream>
#include <fstream>
#include <cstring>
#include "Config.h"
using namespace std;

Config* Config::the_config = nullptr;

Config* Config::instance()
{
    if (!the_config)
    {
        the_config = new Config();
    }

    return (the_config);
}

bool Config::init()
{
    cout << "<< Config::init() 함수 >>" << endl;

    // 시스템 변수 체크, 없으면 디폴트로 지정된 파일 이름 사용
    char const* confFile = getenv(CONFIG_FILE_ENV_VAR);
    if (!confFile)
    {
        cout << "시스템 변수 조회 실패, 디폴트 파일 사용" << endl;
        confFile = DEFAULT_CONFIG.c_str();
    }

    // config 파일을 사용하기 위해 ifstream 객체를 이용하여 파일 열기
    ifstream *cfgFilePtr = new ifstream(confFile, ios::in);
    if ((!cfgFilePtr) || (cfgFilePtr->fail()))
    {
        cout << "파일을 여는데 실패했습니다." << endl;
        return false;
    }

    // ifstream 객체를 이용하여 readCfgIntoMemroy() 함수 호출
    if (!readCfgIntoMemory(cfgFilePtr))
    {
        return false;
    }

    delete cfgFilePtr;
    cfgFilePtr = 0;
    return true;
}

bool Config::readCfgIntoMemory(ifstream *ifstr)
{
    cout << "<< Config::readCfgIntoMemory() 함수 >>" << endl;
    
    while (!ifstr->eof())
    {
        char buf[256];
        ifstr->getline(buf, 256);

        if (strcmp(buf, "") == 0) continue;

        string oneLine(buf);
        if (oneLine[0] == '#') continue;

        int idx = oneLine.find("=");
        if (idx == string::npos)
        {
            cout << "Line = " << oneLine.c_str() << endl;
            cout << "config 문장속에 잘못된 라인 발견!" << endl;
            continue;
        }

        string key = oneLine;
        key.erase(idx);
        string value = oneLine;
        value.erase(0, idx + 1);
        value_map[key] = value;
    }

    return true;
}