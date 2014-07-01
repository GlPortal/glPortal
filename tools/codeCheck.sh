#!/bin/bash -l
cd ..;
mkdir -p html/cppcheck/;

if [ -z ${cppcheckpath+x} ]; then cppcheck --enable=all ./source --xml 2>  /tmp/cppCheck.xml; else $cppcheckpath --enable=all ./source --xml 2>  /tmp/cppCheck.xml; fi
xsltproc -o html/cppcheck/index.html tools/cppcheckToHtml.xsl /tmp/cppCheck.xml;
