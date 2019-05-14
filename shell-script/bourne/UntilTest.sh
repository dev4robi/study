#!/bin/sh

until [ $# -le 0 ]
do
    echo "< $1 명령 실행 >"
    $1
    shift
done