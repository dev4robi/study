#!/bin/sh

awk \
'BEGIN { FS = "|"; print " === 주소록 출력 === "; print "" } \
    { n += 1 } { name = $1 } { telNo = $2 } \
    { mobNo = $3 } { addr = $4 } \
    { print " < " name " >" } \
    { print " 전화번호: " telNo } \
    { print " 휴대폰: " mobNo } \
    { print " 주소: " addr; print "" } \
END { print "총 " n " 명의 주소가 수록되어 있군요." }' \
awkTestAddr.txt