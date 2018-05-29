#!/bin/bash

cd ./working/cmake
sudo ninja install

echo "-- Installing: /usr/local/bin/ckb -> /usr/local/bin/ckbuild"
sudo ln -sf /usr/local/bin/ckbuild /usr/local/bin/ckb