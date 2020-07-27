#!/usr/bin/env bash

uname -a

echo "Making build directory"
mkdir build

echo "Going into build directory"
cd build

echo "Generating cmake files"
cmake ..

echo "Running make"
make

echo "Running make install"
make install

echo "Running make package"
make package