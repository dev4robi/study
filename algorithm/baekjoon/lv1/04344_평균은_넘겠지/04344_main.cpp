/*
 *  > 문제 : 대학생 새내기들의 90%는 자신이 반에서 평균은 넘는다고 생각한다. 당신은 그들에게 슬픈 진실을 알려줘야 한다.
 * 
 *  > 입력 : 첫째 줄에는 테스트 케이스의 개수 C가 주어진다.
 *           둘째 줄부터 각 테스트 케이스마다 학생의 수 N(1 ≤ N ≤ 1000, N은 정수)이 첫 수로 주어지고,
 *           이어서 N명의 점수가 주어진다. 점수는 0보다 크거나 같고, 100보다 작거나 같은 정수이다.
 * 
 *  > 출력 : 각 케이스마다 한 줄씩 평균을 넘는 학생들의 비율을 반올림하여 소수점 셋째 자리까지 출력한다.
 * 
 *  >> 예제입력1 :                                   >> 예제출력1 :
 *     5                                                40.000%
 *     5 50 50 70 80 100                                57.143%
 *     7 100 95 90 80 70 60 50                          33.333%
 *     3 70 90 80                                       66.667%
 *     3 70 90 81                                       55.556%
 *     9 100 99 98 97 96 95 94 93 91
 * 
 **/

#include <iostream>
#include <algorithm>
#include <vector>

int splitString(std::vector<std::string>& vecOutStrs, const std::string& inStr, const std::string& delimStr)
{
    const size_t delimLen = delimStr.length();
    int splitStrCnt = 0;
    size_t bgnIdx = 0;
    size_t endIdx = 0;

    while (true)
    {
        if ((endIdx = inStr.find(delimStr, bgnIdx)) != std::string::npos)
        {
            if (bgnIdx != endIdx)
            {
                vecOutStrs.push_back(inStr.substr(bgnIdx, endIdx - bgnIdx));
                ++splitStrCnt;
            }

            bgnIdx = endIdx + delimLen;
            continue;
        }
        else
        {
            if (bgnIdx != std::string::npos)
            {
                if ((endIdx = inStr.length() - bgnIdx) > 0)
                {
                    vecOutStrs.push_back(inStr.substr(bgnIdx, endIdx));
                    ++splitStrCnt;
                }
            }

            bgnIdx = endIdx = std::string::npos;
            break;
        }
    }

    return splitStrCnt;
}

int main(int argc, char **argv)
{
    int testCase = 0;
    std::vector<float> vecFloatAnswer;

    // input testcase
    std::cin >> testCase;
    std::cin.ignore();

    for (int i = 0; i < testCase; ++i)
    {
        // input line and split
        std::string inStr;
        std::vector<std::string> vecStrScore;

        std::getline(std::cin, inStr);
        splitString(vecStrScore, inStr, " ");

        // string vector to float vector
        const int vecStrScoreSize = vecStrScore.size();
        std::vector<float> vecFloatScore;

        for (int j = 0; j < vecStrScoreSize; ++j)
        {
            vecFloatScore.push_back(std::stof(vecStrScore[j]));
        }

        // get avg score
        const int scoreCnt = (int)vecFloatScore[0];
        float fScoreSum = 0.0f;

        for (int j = 0; j < scoreCnt; ++j)
        {
            fScoreSum += vecFloatScore[j + 1];
        }

        // find score over avg and save calculated avg to float
        const float fScoreAvg = fScoreSum / scoreCnt;
        int scoreOverAvgCnt = 0;

        for (int j = 0; j < scoreCnt; ++j)
        {
            float score = vecFloatScore[j + 1];

            if (score > fScoreAvg)
            {
                ++scoreOverAvgCnt;
            }
        }

        vecFloatAnswer.push_back((float)scoreOverAvgCnt * 100.0f / (float)scoreCnt);
    }

    // print answer
    const int answerCnt = vecFloatAnswer.size();

    std::cout.setf(std::ios::fixed);
    std::cout.precision(3);

    for (int i = 0; i < answerCnt; ++i)
    {
        std::cout << vecFloatAnswer[i] << '%' << std::endl;
    }

    std::cout.unsetf(std::ios::fixed);

    return 0;
}