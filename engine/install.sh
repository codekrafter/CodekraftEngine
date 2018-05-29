#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

echo "Creating Directory Structure"
mkdir -p /opt/CodekraftEngine/
mkdir -p /opt/CodekraftEngine/include/
mkdir -p /opt/CodekraftEngine/lib/
mkdir -p /opt/CodekraftEngine/scripts/

echo "Copying normal headers"
cd src
{ find . -name '*.hpp'; find . -name '*.h'; find . -name '*.inl'; } | cpio -pdmu --quiet /opt/CodekraftEngine/include/
cd ..

echo "Copying configured headers"
cp working/cmake/include/*.hpp /opt/CodekraftEngine/include/

echo "Copying shared library"
cp working/cmake/libCodekraftEngine.so /opt/CodekraftEngine/lib/

echo "copying build script (build.js"
cp ../scripts/build.js /opt/CodekraftEngine/scripts/