#!/bin/bash

cd ..

cd engine

ckb -e .


cd ..
cd testgame

ckb .

./install.sh

cd ..

cd testing

./TestGame
