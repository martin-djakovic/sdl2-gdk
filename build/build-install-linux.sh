#!/bin/bash

# Install script for LINUX
# Do not move the location of this script!

if [ "$EUID" -ne 0 ]
  then echo "Permission denied: script must be run as root!"
  exit
fi

cd "$(dirname $(realpath "$0"))"/../

mkdir -p /usr/local/include/sdl2-gdk
cp -r include/* /usr/local/include/sdl2-gdk/

g++ -fPIC -shared -O2 -o libgdk.so src/*.cpp -lGL -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

mv libgdk.so /usr/local/lib/
ldconfig

cd -