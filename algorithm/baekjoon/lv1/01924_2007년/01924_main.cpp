/**
 *  오늘은 2007년 1월 1일 월요일이다. 그렇다면 2007년 x월 y일은 무슨 요일일까? 이를 알아내는 프로그램을 작성하시오.
 * 
 *  >  IN : 첫째 줄에 빈 칸을 사이에 두고 x(1≤x≤12)와 y(1≤y≤31)이 주어진다. 참고로 2007년에는 1, 3, 5, 7, 8, 10, 12월은 31일까지, 4, 6, 9, 11월은 30일까지, 2월은 28일까지 있다.
 *  > OUT : 첫째 줄에 x월 y일이 무슨 요일인지에 따라 SUN, MON, TUE, WED, THU, FRI, SAT중 하나를 출력한다.
 * 
 **/

#include <iostream>

int main(int argc, char **argv)
{
    const std::string dayStr[7] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
    const int dateOfMonths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int month = 0, date = 0;

    std::cin >> month >> date;

    int monthDelta = month - 1;
    int dateDelta = date - 1;
    int totalDays = 0;

    for (int i = 0; i < monthDelta; ++i)
    {
        totalDays += dateOfMonths[i];
    }

    totalDays += dateDelta;
    // std::cout << totalDays << std::endl;
    std::cout << dayStr[(totalDays + 1) % 7];

    return 0;
}