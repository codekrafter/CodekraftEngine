#!/bin/bash

echo "Building Project"
echo

dotnet publish -v q -c Release -r linux-x64
rc=$?

if [[ $rc != 0 ]]; then
echo
echo "Building exited with $rc, Not completing install"
exit $rc
fi

echo
echo "Done Building Project"
echo

cd bin 

cd Release

cd netcoreapp2.0

cd linux-x64

echo "Installing Files..."

sudo mkdir -p /usr/local/lib/ckbuild/

echo "    Installing Executable"
sudo cp ckbuild /usr/local/lib/ckbuild/

echo "    Installing Required Linux Shared Libraries"
sudo cp ./*.so /usr/local/lib/ckbuild/

echo "    Installing Required Windows Shared Libraries"
sudo cp ./*.dll /usr/local/lib/ckbuild/

echo "    Installing non-binary files (.json and .pdb)"
#sudo cp ./*.txt /usr/local/lib/ckbuild/
sudo cp ./*.json /usr/local/lib/ckbuild/
sudo cp ./*.pdb /usr/local/lib/ckbuild/

echo "Done Installing Files"

echo

echo "Creating Symlinks..."

echo "    Creating ckbuild Symlink"
sudo ln -sf /usr/local/lib/ckbuild/ckbuild /usr/local/bin/ckbuild

echo "    Creating ckb Symlink"
sudo ln -sf /usr/local/lib/ckbuild/ckbuild /usr/local/bin/ckb

echo "Done Creating Symlinks"

echo

echo "Setting Permissions for..."

echo "    ckbuild"
sudo chmod +xr /usr/local/bin/ckbuild

echo "    ckb"
sudo chmod +xr /usr/local/bin/ckb

echo "Done Setting Permissions"

echo

echo "Starting Dry Run to generate caches"
ckbuild > /dev/null
echo "Finished Dry Run"

echo
echo
echo

echo "Installed Codekraft Build Utility to /usr/local/lib"
echo "Use 'ckbuild' or 'ckb' to Run It"
echo