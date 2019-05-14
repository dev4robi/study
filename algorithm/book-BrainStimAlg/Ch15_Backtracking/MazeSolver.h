#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 1024
#define INIT_VALUE -1

#define START   'S' // 시작점
#define GOAL    'G' // 탈출구
#define WAY     ' ' // 길
#define WALL    '#' // 벽
#define MARKED  '+' // 경로

typedef enum tagDIRECTION  { NORTH, SOUTH, EAST, WEST } DIRECTION;
typedef enum tagRESULT     { FAIL, SUCCEED } RESULT;

typedef struct tagPosition
{
    int X;
    int Y;

} Position;

typedef struct tagMazeInfo
{
    int ColumnSize;
    int RowSize;
    char** Data;

} MazeInfo;

RESULT Solve(MazeInfo* Maze);
RESULT MoveTo(MazeInfo* Maze, Position* Curernt, DIRECTION Direction);
RESULT GetNextStep(MazeInfo* Maze, Position* Current, DIRECTION Direction, Position* Next);
RESULT GetMaze(char* FilePath, MazeInfo* Maze);
void   PrintMaze(MazeInfo* Maze);

#endif