#!/bin/sh

# start 또는 stop 등의 인수의 개수를 체크. 하나만 유효
if [ $# -eq 1 ]
then
    echo "인수는 한 개이며 내용은 <$1> 입니다."
    echo "\c"
else
    echo "인수는 하나도 없거나 너무 많아 그냥 종료합니다."
    echo "INVALIDARG"
    exit 0
fi

# 쉘 프로그램을 실행시킨 사용자를 체크, root만 유효
user=whoami
if [ $user = root ]
then
    echo "사용자는 루트입니다."
    echo "\c"
else
    echo "사용자가 루트가 아니어서 그냥 종료합니다."
    echo "NOTROOT"
    exit 0
fi

# 인수가 start인지 stop인지를 체크하고 해당 모듈을 실행
case $1 in
start)
# 프로세스가 이미 실행 중인지 체크하고 없으면 백엔드로 실행
    usage=ps -a | grep onlyOne | /bin/awk '{print $4}'
    if [ "$usage" = "onlyOne" ]
    then
        echo "onlyOne 프로세스가 이미 실행 중입니다."
        echo "ALREADY"
    else
        onlyOne &
        echo "RUNNING"
    fi;;
stop)
# 실행 중인 프로세스를 찾아서 kill 시킴
    usage=ps -a | grep onlnOne | /bin/awk '{print $4}'
    if [ "$usage" = "onlyOne" ]
    then
        kill ps -a | grep onlyOne | awk '{print $1}' > /dev/null 2>&1
        echo "STOPONE"
    else
        echo "실행 중인 프로세스가 없습니다."
        echo "ANYONEP"
    fi;;
*)
    echo "UNKOWN ARG"
esac
exit 1