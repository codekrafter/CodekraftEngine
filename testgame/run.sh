#!/bin/bash

cd ..

cd engine

ckb -i .


cd ..
cd testgame

ckb .

./install.sh

cd ..

cd testing

./TestGame
