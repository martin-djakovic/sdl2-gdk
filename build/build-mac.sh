#!/bin/bash

g++ -dynamiclib -I./include -fPIC -O2 -o libgdk.dylib src/*.cpp -framework OpenGL -lSDL2 -lSDL2_image -lSdl2_ttf -lSDL2_mixer