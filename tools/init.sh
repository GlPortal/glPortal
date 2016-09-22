#!/bin/bash -l
git submodule update --init --recursive;
cd external/RadixEngine;
cmake ./; make;
cd ..; cd ..;
cmake ./; make;
