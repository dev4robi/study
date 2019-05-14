#!/bin/sh

commandName=0
commandOption=0
complete=0

while [ $# -gt 0 ]
do
    case $1 in
    -*)
        commandOption=$1 ;
        complete=2 ;
        shift ;;
    *)
        if [ $complete = 0 ]
        then
            commandName=$1
            complete=1
            shift

            if [ $# = 0 ]
            then
                $commandName
            fi
        elif [ $complete = 1 ]
        then
            $commandName
            commandName=$1
            shift

            if [ $# = 0 ]
            then
                $commandName
            fi
        fi;;
    esac
    if [ $complete = 2 ]
    then
        $commandName $commandOption
        complete=0
    fi
done