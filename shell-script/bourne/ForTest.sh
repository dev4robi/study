#!/bin/bash

for i in $*
do
    echo "< $i 명령 실행 >"
    $i
done 