#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <map>
using namespace std;

// 시스템 변수의 이름인 "CONFIG_FILE" 선언
#define CONFIG_FILE_ENV_VAR "CONFIG_FILE"

// key/value의 조합이 string/string 인 config_pair 타입 선언
typedef map<string, string> config_pair;

// 시스템 변수가 없으면 디폴트로 사용할 ConfigFile 위치 지정
static string DEFAULT_CONFIG = "./ConfigFile";

// Config 클래스
class Config
{
public:
    // 싱글톤 패턴 구현을 위한 instance() 함수 선언
    static Config* instance();

    // Config 클래스의 전체 작업을 수행하는 init() 함수 선언
    bool init();

    // key를 입력받은 뒤, map에서 value를 조회하고 리턴하는 함수 선언
    string get_valueFromMap(string keyVal);

private:
    // 싱글톤 객체 선언
    static Config *the_config;

    // 파일 스트림을 통해 파일을 읽고 map에 저장하는 함수 선언
    bool readCfgIntoMemory(ifstream *ifstr);

    // config_pair(map 타입)를 이용하여 value_map 객체 선언
    config_pair value_map;
};

#endif