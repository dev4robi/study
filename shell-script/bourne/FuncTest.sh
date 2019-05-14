#!/bin/sh

withoutArg()
{
    echo "Run withoutArg() Function"
}

withArg()
{
    echo "Run withArg() Function"

    while [ $# -gt 0 ]
    do
        echo "Func with $1"
        shift
    done
}

withoutArg
withArg Lee T Hoon