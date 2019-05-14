#!/bin/sh

echo "grep에 사용될 옵션 입력, -l또는 -s를 입력해 주세요."
read option1
echo "검색하고자 하는 문장을 입력해 주세요."
read option2
grep $option1 $option2 *