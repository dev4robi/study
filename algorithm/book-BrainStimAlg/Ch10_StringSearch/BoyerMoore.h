#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include <stdio.h>
#include <stdlib.h>

int BoyerMoore(char* Text, int TextSize, int Start, char* Pattern, int PatternSize);
void BuildGST(char* Pattern, int PatternSize, int* Suffix, int* GST);                   // 나쁜 문자 이동 테이블
void BuildBCT(char* Pattern, int PatternSize, int* BST);                                // 착한 접두부 이동 테이블

#endif