#!/bin/bash -l
cd ..;
gitinspector -mrT -Fhtml > index.html;
scp index.html henry@glportal.de:/home/henry/system/websites/stats.glportal.de;
rm index.html;
