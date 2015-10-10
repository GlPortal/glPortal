#!/bin/sh
cd ..;
find ./source -print0 | xargs -0 cccc
cd .cccc;
cp cccc.html index.html