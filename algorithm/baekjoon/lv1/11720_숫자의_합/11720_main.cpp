/**
 *   N개의 숫자가 공백 없이 쓰여있다. 이 숫자를 모두 합해서 출력하는 프로그램을 작성하시오.
 * 
 *   >   IN: 첫째 줄에 숫자의 개수 N (1 ≤ N ≤ 100)이 주어진다. 둘째 줄에 숫자 N개가 공백없이 주어진다.
 *   >  OUT: 입력으로 주어진 숫자 N개의 합을 출력한다.
 * 
 *   Ex)
 *   >  IN1: 5
 *           54321
 *   > OUT1: 15
 * 
 *   >  IN2: 25
 *           7000000000000000000000000
 *   > OUT2: 7
 *
 **/

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    int len = 0, sum = 0;
    std::string inStr;

    std::cin >> len;    
    std::cin >> inStr;

    const char *pStr = inStr.c_str();

    for (int i = 0; i < len; ++i)
    {
        sum += (pStr[i] - '0');
    }

    std::cout << sum;
    
    return 0;
}