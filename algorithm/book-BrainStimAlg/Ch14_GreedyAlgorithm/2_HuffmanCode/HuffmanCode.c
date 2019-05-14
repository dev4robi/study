#include "HuffmanCode.h"
#include "PriorityQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// 허프만 노드 생성
HuffmanNode* Huffman_CreateNode(SymbolInfo stNewData)
{
    HuffmanNode *pstNewNode = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    
    pstNewNode->pstLeft  = NULL;
    pstNewNode->pstRight = NULL;
    pstNewNode->stData   = stNewData;

    return pstNewNode;
}

// 허프만 노드 할당 해제
void Huffman_DestroyNode(HuffmanNode *pstNode)
{
    if (pstNode != NULL) { free(pstNode); }
}

// 허프만 트리 할당 해제 (재귀)
void Huffman_DestroyTree(HuffmanNode *pstNode)
{
    if (pstNode == NULL) { return; }

    Huffman_DestroyTree(pstNode->pstLeft);
    Huffman_DestroyTree(pstNode->pstRight);
    Huffman_DestroyNode(pstNode);
}

// 비트 버퍼에 코드값(cBit) 이어 붙이기
void Huffman_AddBit(BitBuffer *pstBuffer, char cBit)
{
    UCHAR   ucMask              = (UCHAR)0x80;          // 1000_0000 (1byte : 8bit)
    UINT    uiBufferSize        = pstBuffer->uiSize;
    UINT    uiBufferSizeDiv8    = uiBufferSize / 8;
    UINT    uiBufferSizeMod8    = uiBufferSize % 8;

    if (uiBufferSizeMod8 == 0) // 버퍼안의 비트수가 8의 배수(바이트 단위)
    {
        // 1바이트(8bit) 공간을 새로 할당후 초기화
        pstBuffer->pucBuffer = realloc(pstBuffer->pucBuffer, sizeof(UCHAR) * (uiBufferSizeDiv8 + 1));
        pstBuffer->pucBuffer[uiBufferSizeDiv8] = (UCHAR)0x00;
    }

    ucMask >>= uiBufferSizeMod8; // 바이트상에 cBit값을 세팅할 비트위치 선정

    if (cBit == (char)0x01)
    {
        // 선정한 위치에 1bit 설정
        pstBuffer->pucBuffer[uiBufferSizeDiv8] |= ucMask;
    }
    else
    {
        // 선정한 위치를 제외하고 그대로 카피, 선정한 위치는 0bit 설정
        pstBuffer->pucBuffer[uiBufferSizeDiv8] &= (~ucMask);
    }

    ++(pstBuffer->uiSize);
}

// 허프만 인코딩
void Huffman_Encode(HuffmanNode **pstTree, UCHAR *pucSource, BitBuffer *pstEncoded, HuffmanCode astCodeTable[MAX_CHAR])
{
    int         i = 0;
    SymbolInfo  astSymbolInfoTable[MAX_CHAR];
    UCHAR       ucTemporary[MAX_BIT];

    // 심볼 정보 테이블 초기화
    for (i = 0; i < MAX_CHAR; ++i)
    {
        astSymbolInfoTable[i].ucSymbol   = (UCHAR)i;
        astSymbolInfoTable[i].iFrequency = 0;
    }

    i = 0;

    // 입력 바이트의 개수를 심볼 정보 테이블에 기록
    // (ex: "http" -> astSymbolInfoTable[104](h):1, [116](t):2, [112](p):1)
    while (pucSource[i] != '\0')
    {
        ++(astSymbolInfoTable[pucSource[i++]].iFrequency);
    }

    //////////////////////////////////////////////////////////////// TEST - 심볼 정보 테이블 출력 시작 
    fprintf(stdout, "[ Printing symbol info table... ]\n");
    for (int k = 0; k < MAX_CHAR; ++k)
    {
        char byteChar = (!iscntrl(k) ? (char)k : '?');
        fprintf(stdout, "astSymbolInfoTable[%3d(%c)].ucSymbol:'%c' / .iFrequency:'%d'\n", k, byteChar,
                byteChar, astSymbolInfoTable[k].iFrequency);
    }
    fprintf(stdout, "[ Printing symbol info table end... ]\n\n");
    //////////////////////////////////////////////////////////////// TEST - 심볼 정보 테이블 출력  끝

    // 허프만 트리 생성
    Huffman_BuildPrefixTree(pstTree, astSymbolInfoTable);
    
    // 코드 테이블 생성
    Huffman_BuildCodeTable(*pstTree, astCodeTable, ucTemporary, 0);

    //////////////////////////////////////////////////////////////// TEST - 코드 테이블 출력 시작
    fprintf(stdout, "[ Printing code table... ]\n");
    for (int k = 0; k < MAX_CHAR; ++k)
    {
        char byteChar = (!iscntrl(k) ? (char)k : '?');
        char code[MAX_BIT + 1];
        fprintf(stdout, "astCodeTable[%3d(%c)].code = '", k, byteChar);
        for (int l = 0; l < astCodeTable[k].iSize; ++l)
        {
            fprintf(stdout, "%c", (UCHAR)astCodeTable[k].ucCode[l] + (UCHAR)'0'); // 0x00 -> '0', 0x01 -> '1'
        }

        fprintf(stdout, "'\n");
    }
    fprintf(stdout, "[ Printing code table end... ]\n\n");
    //////////////////////////////////////////////////////////////// TEST - 코드 테이블 출력 끝

    i = 0;

    // 원본 데이터를 코드 테이블의 값으로 치환
    while (pucSource[i] != '\0')
    {
        int j = 0;
        int iBitCount = astCodeTable[pucSource[i]].iSize;

        for (j = 0; j < iBitCount; ++j)
        {
            Huffman_AddBit(pstEncoded, astCodeTable[pucSource[i]].ucCode[j]);
        }

        ++i;
    }
}

// 허프만 디코딩
void Huffman_Decode(HuffmanNode *pstTree, BitBuffer *pstEncoded, UCHAR *pucDecoded)
{
    int         i           = 0;
    int         iIndex      = 0;
    HuffmanNode *pstCurrent = pstTree;

    for (i = 0; i <= pstEncoded->uiSize; ++i) // 입력된 '1','0'문자로 이루어진 문자열을 순회
    {
        UCHAR ucMask = (UCHAR)0x80;

        if (pstCurrent->pstLeft == NULL && pstCurrent->pstRight == NULL) // 리프 노드(1번이상 등장한 바이트의 횟수가 담김)
        {
            pucDecoded[iIndex++] = pstCurrent->stData.ucSymbol;
            pstCurrent = pstTree;
        }

        ucMask >>= i % 8; // 바이트를 못 채운 비트 수만큼 우측 쉬프팅 (ex: i=15 (1byte + 7bit) -> 7칸 우측 쉬프팅)

        if ((pstEncoded->pucBuffer[i / 8] & ucMask) != ucMask)
        {
            pstCurrent = pstCurrent->pstLeft;
        }
        else
        {
            pstCurrent = pstCurrent->pstRight;
        }
    }

    pucDecoded[iIndex] = '\0';
}

// 허프만 트리 생성
void Huffman_BuildPrefixTree(HuffmanNode **pstTree, SymbolInfo astSymbolInfoTable[MAX_CHAR])
{
    int             i = 0;
    PQNode          stResult;
    PriorityQueue   *pstPQ = PQ_Create(0);

    for (i = 0; i < MAX_CHAR; ++i)
    {
        // 심볼 정보 테이블에서 1번 이상 등장한 바이트를 트리 노드로 만들어서 우선순위큐에 추가
        if (astSymbolInfoTable[i].iFrequency > 0)
        {
            HuffmanNode *pstBitNode = Huffman_CreateNode(astSymbolInfoTable[i]);
            PQNode      stNewNode;

            stNewNode.Priority = astSymbolInfoTable[i].iFrequency;
            stNewNode.Data     = pstBitNode;
            
            // 바이트의 등장 주기가 '적으면' 큐의 가장 앞에 위치함
            // 즉, 허프만 트리의 높은 레벨(짧은 코드값을 가짐)에 등장주기가 적은 바이트가 위치하고,
            // 등장주기가 높은 바이트는 허프만 트리의 낮은 레벨(긴 코드를 가짐)에 위치함.
            PQ_Enqueue(pstPQ, stNewNode);
        }
    }

    // (★핵심★ : 우선순위 큐를 사용하여 허프만 트리 생성)
    while (pstPQ->UsedSize > 1) // 큐가 빌 때까지 반복
    {
        SymbolInfo  stNewData   = { 0, 0 };
        HuffmanNode *pstBitNode = Huffman_CreateNode(stNewData);
        HuffmanNode *pstLeft    = NULL;
        HuffmanNode *pstRight   = NULL;

        PQNode stQLeft, stQRight, stNewNode;

        PQ_Dequeue(pstPQ, &stQLeft);    // 우선순위 큐의 첫 번째 값을 제거하면서 획득
        PQ_Dequeue(pstPQ, &stQRight);   // 우선순위 큐의 두 번째 값을 제거하면서 획득

        pstLeft  = (HuffmanNode*)stQLeft.Data;
        pstRight = (HuffmanNode*)stQRight.Data;

        pstBitNode->stData.ucSymbol   = (UCHAR)0x00;
        pstBitNode->stData.iFrequency = pstLeft->stData.iFrequency + pstRight->stData.iFrequency;

        pstBitNode->pstLeft  = pstLeft;
        pstBitNode->pstRight = pstRight;

        stNewNode.Priority = pstBitNode->stData.iFrequency; // 새로 삽입되는 노드는 '기존 첫 번째 + 두 번째의 빈도'를 가지고,
        stNewNode.Data     = pstBitNode;                    // 좌측에는 첫 번째 노드, 우측에는 두 번째 노드를 포인팅

        PQ_Enqueue(pstPQ, stNewNode);
    }                                   

    PQ_Dequeue(pstPQ, &stResult);
    *pstTree = (HuffmanNode*)stResult.Data; // 최종 생성된 허프만 트리 포인팅
}

// 허프만 코드테이블 생성
void Huffman_BuildCodeTable(HuffmanNode *pstTree, HuffmanCode astCodeTable[MAX_CHAR], UCHAR aucCode[MAX_BIT], int iSize)
{
    if (pstTree == NULL) { return; }

    if (pstTree->pstLeft != NULL) // 허프만 트리의 왼쪽으로는 Bit:0
    {
        aucCode[iSize] = (UCHAR)0x00;
        Huffman_BuildCodeTable(pstTree->pstLeft, astCodeTable, aucCode, iSize + 1);
    }

    if (pstTree->pstRight != NULL) // 허프만 트리의 오른족으로는 Bit:1
    {
        aucCode[iSize] = (UCHAR)0x01;
        Huffman_BuildCodeTable(pstTree->pstRight, astCodeTable, aucCode, iSize + 1);
    }

    if (pstTree->pstLeft == NULL && pstTree->pstRight == NULL) // 리프 노드(바이트 데이터가 담긴 노드)
    {
        int i = 0;

        for (i = 0; i < iSize; ++i)
        {
            astCodeTable[pstTree->stData.ucSymbol].ucCode[i] = aucCode[i]; // 코드 테이블의 리프노드 바이트에 코드(0,1조합) 기록
        }

        astCodeTable[pstTree->stData.ucSymbol].iSize = iSize;   // 코드(0,1조합)의 길이 저장
    }                                                           // (ex: aucCode=00010000, iSize=4 -> 허프만 트리 왼쪽으로부터 두 번째 리프노드)
}

// 비트 버퍼 출력
void Huffman_PrintBinary(BitBuffer *pstBuffer)
{
    int i = 0;

    for (i = 0; i < pstBuffer->uiSize; ++i)
    {
        UCHAR ucMask = (UCHAR)0x80;

        ucMask >>= i % 8;

        fprintf(stdout, "%d", (pstBuffer->pucBuffer[i / 8] & ucMask) == ucMask);
    }
}

// 허프만코딩 테스트 메인
static int Test_HuffmanCode_main(int argc, char **argv)
{
    char *pcSource  = "http://www.seanlab.net";
    char *pcDecoded = "";

    HuffmanNode *pstTree = NULL;
    BitBuffer   stEncoded = { NULL, 0 };
    HuffmanCode stCodeTable[MAX_CHAR];

    memset(&stCodeTable, 0x00, sizeof(HuffmanCode) * MAX_CHAR);

    if (argv[1] != NULL) {
        pcSource = argv[1];
    }

    if (strlen(pcSource) < 2) {
        fprintf(stdout, "Original message is to short to compress\n");
        return 0;
    }

    Huffman_Encode(&pstTree, pcSource, &stEncoded, stCodeTable);

    fprintf(stdout, "Original Size:%ld Encoded Size:%u\n", (strlen(pcSource) + 1) * sizeof(char) * 8, stEncoded.uiSize);

    pcDecoded = (char*)malloc(sizeof(char) * (strlen(pcSource) + 1));
    Huffman_Decode(pstTree, &stEncoded, pcDecoded);

    fprintf(stdout, "Original : %s\n", pcSource);
    fprintf(stdout, "Encoded  : ");

    Huffman_PrintBinary(&stEncoded);

    fprintf(stdout, "\nDecoded  : %s\n", pcDecoded);

    Huffman_DestroyTree(pstTree);
    free(pcDecoded);

    return 0;
}

// 메인
int main(int argc, char **argv)
{
    return Test_HuffmanCode_main(argc, argv);
}