#!/bin/sh
cd ..;
ditz html;
find ./html -name '*.html' -type f -exec sed -i "s/@/ at /g" {} \;