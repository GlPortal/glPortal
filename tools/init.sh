#!/bin/sh
# Run this from the project root
# it will prepare your submodules
# and build radix and glportal
git submodule update --init --recursive;
cd external/RadixEngine;
cmake ./; make;
cd ..; cd ..;
cmake ./; make;
