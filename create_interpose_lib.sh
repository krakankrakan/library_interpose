#!/bin/bash

# remove the old build
rm build
mkdir build

# build the interposer library
gcc -Wall main.c -o interpose
cd asm
make
cd ..

# create the assembly file for the shared library and the .symbol file for the given argumen
./interpose $1

# link the generated asm file	
gcc -O0 -fpic -c ./build/asm.s -g -fpic -m64 -o ./build/asm.o -ldl
gcc -O0 ./build/asm.o -shared -fpic -m64 -o ./build/libintercept.so -ldl

# build the test binary
gcc -Wall test/test.c -o test/test
