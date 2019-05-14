#!/bin/sh

if [ $# -eq 2 ]
then
    echo "인수는 두 개이며 내용은 <$1>, <$2> 입니다."
elif [ $# -eq 1 ]
then
    echo "인수는 한 개이며 내용은 <$1> 입니다."
elif [ $# -ge 3 ]
then
    echo "인수는 $#개이며 내용들은 <$*> 입니다."
else
    echo "인수가 하나도 없습니다."
fi