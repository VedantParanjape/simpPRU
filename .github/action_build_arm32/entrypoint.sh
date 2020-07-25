#!/usr/bin/env bash

uname -a

echo "Running aclocal"
aclocal

echo "Running autoconf"
autoconf

echo "Running automake"
automake --add-missing

echo "Running configure script"
./configure

echo "Running make"
make

echo "Running make check"
make check

echo "Running make distcheck"
make distcheck