#!/bin/bash

# Install script for LINUX
# Do not move the location of this script!

if [ "$EUID" -ne 0 ]
  then echo "Permission denied: script must be run as root!"
  exit
fi

script_dir=$(dirname $(realpath $0))
cd $script_dir

g++ -I./include -fPIC -O2 -c src/*.cpp
g++ -shared -o libgdk.so animatedtexture.o camera.o collidesprite.o fonttexture.o imagetexture.o init.o performancetools.o scene.o sound.o sprite.o texture.o
rm -r animatedtexture.o camera.o collidesprite.o fonttexture.o imagetexture.o init.o performancetools.o scene.o sound.o sprite.o texture.o

mkdir /usr/local/include/sdl2-gdk
rm -r /usr/local/include/sdl2-gdk/*
cp -r include/* /usr/local/include/sdl2-gdk/
mv libgdk.so /usr/local/lib/
ldconfig

cd -