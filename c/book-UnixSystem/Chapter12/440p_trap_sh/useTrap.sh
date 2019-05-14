#!/bin/sh
# trap "command1; command2" SIGTYPE('kill -l' for check sigtype)
trap "echo Caught SIGINT...Removing tmp.txt; rm tmp.txt; exit" KILL
find . -name "*" -exec grep -l a {} \; > tmp.txt