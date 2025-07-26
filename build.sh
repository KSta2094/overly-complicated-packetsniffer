#!/bin/bash

clang *.c -o out
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

./out
