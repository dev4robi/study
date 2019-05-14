/**
 *   상근이는 요즘 설탕공장에서 설탕을 배달하고 있다. 상근이는 지금 사탕가게에 설탕을 정확하게 N킬로그램을 배달해야 한다. 설탕공장에서 만드는 설탕은 봉지에 담겨져 있다. 봉지는 3킬로그램 봉지와 5킬로그램 봉지가 있다.
 *   상근이는 귀찮기 때문에, 최대한 적은 봉지를 들고 가려고 한다. 예를 들어, 18킬로그램 설탕을 배달해야 할 때, 3킬로그램 봉지 6개를 가져가도 되지만, 5킬로그램 3개와 3킬로그램 1개를 배달하면, 더 적은 개수의 봉지를 배달할 수 있다.
 *   상근이가 설탕을 정확하게 N킬로그램 배달해야 할 때, 봉지 몇 개를 가져가면 되는지 그 수를 구하는 프로그램을 작성하시오.
 * 
 *   >  IN: 첫째 줄에 N이 주어진다. (3 ≤ N ≤ 5000)
 *   > OUT: 상근이가 배달하는 봉지의 최소 개수를 출력한다. 만약, 정확하게 N킬로그램을 만들 수 없다면 -1을 출력한다.
 **/

/**
 *  백트래킹을 사용하여 접근해 보았음.
 *  먼저 5kg단위로 감소시키다가, 감소시킬 수 없는 단위가 나오면 이전 단계로 돌아가서 3kg을 시도해봄.
 *  3kg로도 감소시킬 수 없는 단위가 나오면, 전전 단계로 돌아가서 3kg을 시도해 보는 식.
 **/

#include <iostream>

int checkStack(int kgRemain, int kg3Cnt, int kg5Cnt)
{
    // std::cout << "kgRemain:" << kgRemain << ", kg3Cnt:" << kg3Cnt << ", kg5Cnt:" << kg5Cnt << std::endl;

    if (kgRemain < 2 && kgRemain != 0) return -1;
    else if (kgRemain == 0) return kg3Cnt + kg5Cnt;
    
    int result = -1;

    if ((result = checkStack(kgRemain - 5, kg3Cnt, kg5Cnt + 1)) == -1)
    {
        if ((result = checkStack(kgRemain - 3, kg3Cnt + 1, kg5Cnt)) == -1)
        {
            return -1;
        }
    }

    return result;
}

int main(int argc, char **argv)
{
    int n;

    std::cin >> n;
    std::cout << checkStack(n, 0, 0);

    return 0;
}