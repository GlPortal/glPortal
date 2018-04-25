#!/bin/sh
while inotifywait -r -qq -e close_write,create --exclude '\.(sh|md)' ./source/ ./external/ ./include/; do
    sleep 1
    ninja
done
