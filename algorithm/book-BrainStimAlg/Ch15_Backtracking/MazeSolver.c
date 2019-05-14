#include "MazeSolver.h"

RESULT Solve(MazeInfo* Maze)
{
    int i = 0, j = 0;
    RESULT StartFound = FAIL;
    RESULT Result = FAIL;

    Position Start;

    // 미로의 시작 위치(START)를 찾음
    for (i = 0; i < Maze->RowSize; ++i)
    {
        for (j = 0; j < Maze->ColumnSize; ++j)
        {
            if (Maze->Data[i][j] == START)
            {
                Start.X = j;
                Start.Y = i;
                StartFound = SUCCEED;
                break;
            }
        }
    }

    // 시작 위치(START)를 못찾았으면 실패 반환
    if (StartFound == FAIL)
        return FAIL;

    // 북,남,동,서 순서로 이동 시도
    if (MoveTo(Maze, &Start, NORTH))
        Result = SUCCEED;
    else if (MoveTo(Maze, &Start, SOUTH))
        Result = SUCCEED;
    else if (MoveTo(Maze, &Start, EAST))
        Result = SUCCEED;
    else if (MoveTo(Maze, &Start, WEST))
        Result = SUCCEED;

    Maze->Data[Start.Y][Start.X] = START;

    return Result;
}

RESULT MoveTo(MazeInfo* Maze, Position* Current, DIRECTION Direction)
{
    int i = 0;
    int Dirs[] = { NORTH, SOUTH, EAST, WEST };
    Position Next;

    // 목적지에 도착했는지 확인
    if (Maze->Data[Current->Y][Current->X] == GOAL)
        return SUCCEED;

    // 현재 위치를 지나간(MARKED) 위치로 설정
    Maze->Data[Current->Y][Current->X] = MARKED;

    PrintMaze(Maze); // test

    for (int i = 0; i < 4; ++i)
    {
        // 해당 방향(Dirs[i])으로 이동 가능한지 확인
        if (GetNextStep(Maze, Current, Dirs[i], &Next) == FAIL)
            continue;

        // 해당 방향으로 이동
        if (MoveTo(Maze, &Next, Dirs[i]) == SUCCEED)
            return SUCCEED;
    }

    // 모든 방향에 대해 이동에 실패했으므로 원래 길(WAY)로 표시한후 이전으로 백트래킹
    Maze->Data[Current->Y][Current->X] = WAY;

    return FAIL;
}

RESULT GetNextStep(MazeInfo* Maze, Position* Current, DIRECTION Direction, Position* Next)
{
    switch (Direction)
    {
        case NORTH:
            Next->X = Current->X;
            if ((Next->Y = Current->Y - 1) == -1) return FAIL; // Y - 1, 배열 범위 검사
            break;
        case SOUTH:
            Next->X = Current->X;
            if ((Next->Y = Current->Y + 1) == Maze->RowSize) return FAIL; // Y + 1, 배열 범위 검사
            break;
        case EAST:
            if ((Next->X = Current->X + 1) == Maze->ColumnSize) return FAIL; // X + 1, 배열 범위 검사
            Next->Y = Current->Y;
            break;
        case WEST:
            if ((Next->X = Current->X - 1) == -1) return FAIL; // X - 1, 배열 범위 검사
            Next->Y = Current->Y;
            break;
        default:
            return FAIL; // 미정의된 방향인경우 FAIL
    }

    if (Maze->Data[Next->Y][Next->X] == WALL)   return FAIL; // 벽으로 이동 불가능
    if (Maze->Data[Next->Y][Next->X] == MARKED) return FAIL; // 지나간 길 이동 불가능

    return SUCCEED;
}

RESULT GetMaze(char* FilePath, MazeInfo* Maze)
{
    int i = 0, j = 0;
    int RowSize = 0;
    int ColumnSize = INIT_VALUE;
    FILE* fp = NULL;
    char buffer[MAX_BUFFER];

    if ((fp = fopen(FilePath, "r")) == NULL)
    {
        fprintf(stdout, "Cannot open file : %s\n", FilePath);
        return FAIL;
    }

    while (fgets(buffer, MAX_BUFFER, fp) != NULL)
    {
        fprintf(stdout, "[%03d] %s", RowSize, buffer);

        ++RowSize;

        if (ColumnSize == INIT_VALUE) // 첫 번째 행(Row)을 읽은 경우
        {
            // 행의 길이 - 1 을 (배열은 0부터 시작) 열(Column)로 설정
            ColumnSize = strlen(buffer) - 1;
        }
        else if (ColumnSize != strlen(buffer) - 1) // 다음 행들이 첫 번째 행과 길이가 다른 경우 (미로가 직사각형이 아님)
        {
            fprintf(stdout, "Maze data in file: %s is not valid. (row: %d, col:%ld)\n", FilePath, RowSize, strlen(buffer));
            fclose(fp);
            return FAIL;
        }
    }
    
    // 미로 크기 설정
    Maze->RowSize = RowSize;
    Maze->ColumnSize = ColumnSize;
    Maze->Data = (char**)malloc(sizeof(char*) * RowSize);

    // 미로 크기만큼 행렬 동적 할당
    for (i = 0; i < RowSize; ++i)
        Maze->Data[i] = (char*)malloc(sizeof(char) * ColumnSize);

    // 파일 포인터를 처음으로 이동
    rewind(fp);

    // 행을 읽고 내용을 복사하여 미로 완성
    for (i = 0; i < RowSize; ++i)
    {
        fgets(buffer, MAX_BUFFER, fp);

        for (j = 0; j < ColumnSize; ++j)
        {
            Maze->Data[i][j] = buffer[j];
        }
    }

    fclose(fp);
    return SUCCEED;
}

void PrintMaze(MazeInfo* Maze)
{
    int i = 0, j = 0;

    for (i = 0; i < Maze->RowSize; ++i)
    {
        fprintf(stdout, "[%03d] ", i);

        for (j = 0; j < Maze->ColumnSize; ++j)
        {
            fprintf(stdout, "%c", Maze->Data[i][j]);
        }

        fprintf(stdout, "\n");
    }
}

int main(int argc, char** argv)
{
    MazeInfo Maze;

    if (argc < 2)
    {
        fprintf(stdout, "Usage: MazeSolver <MazeFile>\n");
        return 0;
    }

    // 미로 로딩
    fprintf(stdout, "\n[Input Maze]\n");

    if (GetMaze(argv[1], &Maze) == FAIL)
        return 0;

    // 미로 풀기
    fprintf(stdout, "\n[Maze Solving...]\n");

    if (Solve(&Maze) == FAIL)
        return 0;
    
    // 미로 풀이 출력
    fprintf(stdout, "\n[Maze Solved]\n");
    PrintMaze(&Maze);

    return 0;
}