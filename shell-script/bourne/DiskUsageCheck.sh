#!/bin/sh

usage=df -k $1 | awk '{ rem = 0 } { n += 1 } { a = $3 } { b = $4 } \
n == 2 { rem = int(a/(a+b) * 100); print rem} \
END { }'

if [ $usage -ge 90 ]
then
    echo "DISK($usage) - 심각한 상태"
elif [ $usage -ge 70 ]
then
    echo "DISK($usage) - 주의요망 상태"
else
    echo "DISK($usage) - 양호한 상태"
fi