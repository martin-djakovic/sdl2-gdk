#!/bin/bash

# Install script for MACOS
# Do not move the location of this script!

cd "$(dirname "$0")"/../

# Change this to the location of the g++ binary on your system
gpp=/usr/local/Cellar/gcc/15.1.0/bin/g++-15 

$gpp -dynamiclib -I./include/ -fPIC -O2 -o libgdk.dylib src/*.cpp -framework OpenGL -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

cd -
