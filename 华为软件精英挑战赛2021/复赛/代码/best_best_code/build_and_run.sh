#!/bin/bash

SCRIPT=$(readlink -f "$0")
BASEDIR=$(dirname "$SCRIPT")
cd $BASEDIR

sh build.sh
cd bin
echo ----------------------------training-data-1----------------------------
time ./CodeCraft-2021 1
cd ../training-data
g++ -o judge judge.cpp
./judge 1
echo ----------------------------training-data-2----------------------------
cd ../bin
time ./CodeCraft-2021 2
cd ../training-data
g++ -o judge judge.cpp
./judge 2
