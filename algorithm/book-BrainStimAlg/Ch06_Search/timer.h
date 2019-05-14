#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <string.h>

typedef long long INT64;

void ResetTimer()
{
    memset(&gST_TIMEVAL, 0x00, sizeof(gST_TIMEVAL));
}

int StartTimer()
{
    return mingw_gettimeofday(&gST_TIMEVAL, NULL);
}

int StopTimer()
{
    struct timeval stStopTv;
    INT64  i64Time = 0;
    int    sec, ms, us;

    mingw_gettimeofday(&stStopTv, NULL);

    stStopTv.tv_sec  -= gST_TIMEVAL.tv_sec;
    stStopTv.tv_usec -= gST_TIMEVAL.tv_usec;
    i64Time = stStopTv.tv_sec * 1000000 + stStopTv.tv_usec;

    sec = i64Time / 1000000;        // Second
    ms  = i64Time % 1000000 / 1000; // Milli second (1/1000)
    us  = i64Time % 1000;           // Micro second (1/1000000)

    memcpy(&gST_TIMEVAL, &stStopTv, sizeof(gST_TIMEVAL));
    
    fprintf(stdout, "Time Elapsed : %d.%03d%03ds\n", sec, ms, us);

    return i64Time;
}

#endif