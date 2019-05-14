#!/bin/sh
# 'MiniCalc' for study shell script!
#
# <Case.1>
# 1 +
# 결과 : 0+1
#
# <Case.2>
# 1 + 2 -
# 결과 : 0+1-2

result=0
number=0

for id in $*
do
    case $id in
    +)
        result=`expr $result+$number`;;
    -)
        result=`expr $result-$number`;;
    x)
        result=`expr $result\*$number`;;
    /)
        result=`expr $result/$number`;;
    *)
        number=$id;;
    esac
done

echo "결과: $result"