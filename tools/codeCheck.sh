#!/bin/sh
cd ..;
mkdir -p html/cppcheck/;
cppcheck --enable=all ./source --xml 2>  /tmp/cppCheck.xml
xsltproc -o html/cppcheck/index.html tools/cppcheckToHtml.xsl /tmp/cppCheck.xml;
