#!/bin/bash

# Install script for MACOS
# Do not move the location of this script!

if [ "$EUID" -ne 0 ]
  then echo "Permission denied: script must be run as root!"
  exit
fi

cd "$(dirname "$0")"/../

mkdir -p /usr/local/include/sdl2-gdk
cp -r include/* /usr/local/include/sdl2-gdk/

# Change this to the location of the g++ binary on your system
/usr/local/Cellar/gcc/15.1.0/bin/g++-15 -dynamiclib -I/usr/local/include/ -fPIC -O2 -o libgdk.dylib src/*.cpp -framework OpenGL -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

mv libgdk.dylib /usr/local/lib/

cd -