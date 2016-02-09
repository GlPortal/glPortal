#!/bin/sh
cd ..;
rm -rf documentation/specificationCompiled/;
mkdir documentation/specificationCompiled/;
cp -r documentation/specification/* documentation/specificationCompiled/
# Using https://github.com/hhirsch/markdown-gh-to-dox
find documentation/specificationCompiled/ -exec gh2dox {} \;
doxygen;
