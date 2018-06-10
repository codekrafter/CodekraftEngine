#!/bin/bash

if [ $(echo ${PWD##*/}) == "scripts" ]; then
    cd ..
fi

if [ $(echo ${PWD##*/}) != "CKEngine" ] && [ $(echo ${PWD##*/}) != "CodekraftEngine" ]; then
    echo "Please Execute Script in the scripts directory or the base directory of the repo"
    exit 0
fi

if [ ! -d "testing" ]; then
    mkdir testing
    echo "Created new testing directory"
fi

if [ ! -d "build" ]; then
    echo "Please build the executable before deploying"
fi

cd build

if [ ! -d "testgame" ]; then
    echo "no 'testgame' directory in build directory"
fi

cd testgame

if [[ ! -x "TestGame" ]]
then
    echo "Please build the executable before deploying"
    exit 0
fi

cp ./TestGame ../../testing/
cd ../../testing/