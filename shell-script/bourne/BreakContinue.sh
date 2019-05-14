#!/bin/sh

for var in $*
do
    if [ $var = java ]
    then
        echo "이번 에디션에서 자바 언어는 다루지 않습니다."
        continue
    elif [ $var = Quit ]
    then
        echo "Quit을 만나 for문을 종료합니다."
        break
    else
        echo "$var 언어는 이번 에디션에서 다루는 언어입니다."
    fi
done