#!/usr/bin/env bash

uname -a

echo "Making build directory"
mkdir build

echo "Going into build directory"
cd build

echo "Generating cmake files"
cmake ..

echo "Running make"
make -j 8

echo "Running make install"
make install

echo "Running make package"
make package

echo "testing simppru"
cd ..
python3 test.py