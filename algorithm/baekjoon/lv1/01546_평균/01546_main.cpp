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

// 점수 중 최댓값(M), 모든 점수/M*100 수행
void juzak(std::vector<float>& vecOutFloats, const std::vector<std::string>& vecInStrs)
{
    int inSize = vecInStrs.size();

    for (int i = 0; i < inSize; ++i)
    {
        vecOutFloats.push_back(std::stof(vecInStrs[i]));
    }

    float M = *(std::max_element(vecOutFloats.begin(), vecOutFloats.end()));

    for (int i = 0; i < inSize; ++i)
    {
        float fScore = vecOutFloats[i];
        vecOutFloats[i] = fScore / M * 100.0f;
    }
}

int main(int argc, char **argv)
{
    // input
    int n = 0;
    std::string inStr;

    std::cin >> n;
    std::cin.ignore();
    std::getline(std::cin, inStr);

    // split
    std::vector<std::string> vecStrs;
    int numCnt = splitString(vecStrs, inStr, " ");

    // juzak
    std::vector<float> vecFloats;
    juzak(vecFloats, vecStrs);

    // average and print
    float fSum = 0.0f;

    for (int i = 0; i < numCnt; ++i)
    {
        fSum += vecFloats[i];
    }

    std::cout << (fSum / (float)numCnt);
    return 0;
}