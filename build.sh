#!/bin/bash

mkdir -p build
g++ -c main.cpp -o build/main.o
windres resource.rc -O coff -o build/resource.res
g++ -o build/fcd.exe -mwindows build/main.o build/resource.res

echo "Done!"
