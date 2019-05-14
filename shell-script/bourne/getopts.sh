#!/bin/sh
while getopts ":ab:c" Option
do
    case $Option in
        a) echo "A 옵션으로 프로그램 실행";;
        b) echo "$OPTARG 데이터와 함께 B옵션으로 프로그램 실행";;
        c) echo "C 옵션으로 프로그램 실행";;
    esac
done
exit 0