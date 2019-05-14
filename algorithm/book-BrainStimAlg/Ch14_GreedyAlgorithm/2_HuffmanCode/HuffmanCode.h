#ifndef HUFFMAN_CODING_H
#define HUFFMAN_CODING_H

#define MAX_CHAR    256
#define MAX_BIT     8

typedef unsigned int  UINT;
typedef unsigned char UCHAR;

typedef struct _SymbolInfo
{
    UCHAR   ucSymbol;
    int     iFrequency;

} SymbolInfo;

typedef struct _HuffmanNode
{
    SymbolInfo          stData;
    struct _HuffmanNode *pstLeft;
    struct _HuffmanNode *pstRight;

} HuffmanNode;

typedef struct _BitBuffer
{
    UCHAR   *pucBuffer;
    UINT    uiSize;

} BitBuffer;

typedef struct _HuffmanCode
{
    UCHAR   ucCode[MAX_BIT];
    int     iSize;

} HuffmanCode;

HuffmanNode*    Huffman_CreateNode(SymbolInfo stNewData);
void            Huffman_DestroyNode(HuffmanNode *pstNode);
void            Huffman_DestroyTree(HuffmanNode *pstNode);
void            Huffman_AddBit(BitBuffer *pstBuffer, char cValue);
void            Huffman_Encode(HuffmanNode **pstTree, UCHAR *pucSource, BitBuffer *pstEncoded, HuffmanCode astCodeTable[MAX_CHAR]);
void            Huffman_Decode(HuffmanNode *pstTree, BitBuffer *pstEncoded, UCHAR *pucDecoded);
void            Huffman_BuildPrefixTree(HuffmanNode **pstTree, SymbolInfo astSymbolInfoTable[MAX_CHAR]);
void            Huffman_BuildCodeTable(HuffmanNode *pstTree, HuffmanCode astCodeTable[MAX_CHAR], UCHAR aucCode[MAX_BIT], int iSize);
void            Huffman_PrintBinary(BitBuffer *pstBuffer);

#endif