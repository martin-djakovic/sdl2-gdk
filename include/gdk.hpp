#ifdef __linux__
#include <sdl2-gdk/sprite.hpp>
#include <sdl2-gdk/camera.hpp>
#include <sdl2-gdk/sprite.hpp>
#include <sdl2-gdk/fonttexture.hpp>
#include <sdl2-gdk/init.hpp>
#include <sdl2-gdk/scene.hpp>
#include <sdl2-gdk/sound.hpp>
#include <sdl2-gdk/texture.hpp>
#include <sdl2-gdk/animatedtexture.hpp>
#include <sdl2-gdk/performancetools.hpp>

#elif _WIN32
#include <sprite.hpp>
#include <camera.hpp>
#include <sprite.hpp>
#include <fonttexture.hpp>
#include <init.hpp>
#include <scene.hpp>
#include <sound.hpp>
#include <texture.hpp>
#include <animatedtexture.hpp>
#include <performancetools.hpp>

#elif __APPLE__
#include <sprite.hpp>
#include <camera.hpp>
#include <sprite.hpp>
#include <fonttexture.hpp>
#include <init.hpp>
#include <scene.hpp>
#include <sound.hpp>
#include <texture.hpp>
#include <animatedtexture.hpp>
#include <performancetools.hpp>

#endif