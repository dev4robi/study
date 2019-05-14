#!/bin/sh

case $1 in
ls)
    ls;;
ps)
    ps;;
pwd)
    pwd;;
*)
    echo "인수를 넣지 않았거나 존재하지 않는 명령어입니다.";;
esac