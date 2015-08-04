#!/bin/sh
cd ..;
git submodule update --depth 1;
git fetch --all;
git reset --hard origin/master:
