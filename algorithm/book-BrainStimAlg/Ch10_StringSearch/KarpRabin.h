#ifndef KARPRABIN_H
#define KARPRABIN_H

/*
    [ KarpRabin String Search ]

    1) Rabins Fingerprint Hash
    inputText = "tesstring"
    pattern   = "sts"

    {patternHash} = {hash("sts")} = {'s'*4 + 't'*2 + 's'*1} = {115*4 + 116*2 + 115  } = {807}
    {firstHash}   = {hash("tes")} = {'t'*4 + 'e'*2 + 's'*1} = {116*4 + 101*2 + 115*1} = {781}
    {nextHash}    = {hash("est")} = {(2 * firstHash - 's' * 4) + 't'} = {(2 * 781 - 115 * 4) + 116} = {1218} and {firstHash = nextHash}
    ...
    (해싱값이 너무 큰 경우를 대비해서 해시 결과값에 "mod(%) MaxInteger(0x7fffffff)"를 적용)

    2)
    위의 해싱을 반복하면서, patternHash == nextHash 가 되는 위치에서 BruteForce로 문자열을 검색한다.
*/

int KarpRabin(char* Text, int TextSize, int Start, char* Pattern, int PatternSize);
static int RabinsFingerprintHash(char* String, int Size);
static int ReHash(char* String, int Start, int Size, int HashPrev, int Coefficient);

#endif