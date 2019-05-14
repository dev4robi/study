/**
 *  첫째 줄에는 별 1개, 둘째 줄에는 별 2개, N번째 줄에는 별 N개를 찍는 문제
 *  하지만, 오른쪽을 기준으로 정렬한 별(예제 참고)을 출력하시오.
 * 
 *  >  IN : 첫째 줄에 N(1 ≤ N ≤ 100)이 주어진다.
 *  > OUT : 첫째 줄부터 N번째 줄까지 차례대로 별을 출력한다.
 * 
 **/

#include <iostream>

int main(int argc, char **argv)
{
    int n = -1;

    std::cin >> n;

    int limit = n + 1;

    for (int i = 1; i < limit; ++i)
    {
        for (int k = 1; k < limit - i; ++k)
        {
            std::cout << " ";
        }

        for (int j = 0; j < i; ++j)
        {
            std::cout << "*";
        }

        std::cout << std::endl;
    }

    return 0;
}