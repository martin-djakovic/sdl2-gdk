#!/bin/bash

# Install script for LINUX
# Do not move the location of this script!

if [ "$EUID" -ne 0 ]
  then echo "Permission denied: script must be run as root!"
  exit
fi

script_dir=$(dirname $(realpath $0))
cd $script_dir

mkdir /usr/local/include/sdl2-gdk
rm -r /usr/local/include/sdl2-gdk/*
cp -r include/* /usr/local/include/sdl2-gdk/

g++ -I./include -fPIC -O2 -c src/*.cpp
g++ -shared -o libgdk.so *.o
rm -r *.o

mv libgdk.so /usr/local/lib/
ldconfig

cd -