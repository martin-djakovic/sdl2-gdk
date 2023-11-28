# Introduction

**This project, including the documentation, is a work in progress. Bugs, unfinished features and other issues are to be expected. Report any issues you find to the issues tab on the project GitHub page.**

The goal of this project is to make development of simple 2D games in C++ easier, and keep code cleaner. SDL2-GDK is designed to be a lightweight, but reasonably capable 2D game engine. It utilizes the graphical capabilities of the SDL2 library, but provides classes and functions specifically tailored to 2D game development. It makes drawing images, text, collisions, movement, sprite organization, and many other things a lot simpler to accomplish, saving a lot of time and keeping the game code clean.

# Table of Contents
<a href="#introduction">Introduction<a><br>
<a href="#table-of-contetns">Code documentation<a>
<details><summary><a href="#code-documentation">Code documentation<a></summary>
&nbsp;&nbsp;&nbsp;&nbsp;<a href="#sprite">Sprite<a><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#sprite-1">Sprite()<a>
</details>

# Code documentation
## Sprite

Sprite class is used to allow for easy rendering of images to a window. It has functions for movement, rotation and collision detection. It is **strongly** recommended to group sprites into scenes.

Public attributes:\
`int w` - width of the sprite's image\
`int h` - height of the sprite's image\
`bool is_collider` - if `true`, sprite will not collide with other sprites, unless it is specifically allowed in the parameters of a function which uses collision checking\
`SDL_Point rotation_center` - coordinates of rotation center, relative to sprite's coordinates

Private attributes:\
`double x` - x coordinate of the sprite's image\
`double y` - y coordinate of the sprite's image\
`double rotation_angle` - angle at which sprite is currently rotated\
`std::string img_path` - relative/absolute file path of the sprite image (e.g. `res/player.png` or `res\\player.png` on Windows)\
`SDL_RendererFlip flip`:\
&nbsp;&nbsp;&nbsp;&nbsp;`SDL_FLIP_NONE` - sprite is not flipped\
&nbsp;&nbsp;&nbsp;&nbsp;`SDL_FLIP_HORIZONTAL` - sprite is flipped horizontally\
&nbsp;&nbsp;&nbsp;&nbsp;`SDL_FLIP_VERTICAL` - sprite is flipped vertically\
`SDL_Renderer *sprite_renderer` - renderer which sprite is drawn on. Automatically assigned when sprite is added to scene\
`SDL_Surface *surface` - surface used to create texture from image file\
`SDL_Texture *texture` - texture that is drawn to renderer\
`SDL_Rect img_rect` - rectangle that defines texture coordinates and boundries when drawing. Rectangle coordinates, width and height are set to `x`, `y`, `w` and `h`\
`SDL_Rect hitbox` - rectangle that defines the sprite hitbox. Hitbox properties are the only ones to affect collisision detection - sprite image location and size have nothing to do with collisions\
`double hitbox_xoffset` - hitbox offset on the x axis, relative to sprite coordinates\
`double hitbox_yoffset` - hitbox offset on the y axis, relative to sprite coordinates\
`std::vector<Sprite *> *colliders` - contains all sprites that will be used in functions to check collisions without requiring programmer input. Automatically set to all the sprites in the scene when added to a scene\
`Sprite *collide_sprite` - last sprite that was collided with. Only updated when checking for collisions manually, and the sprite collided with another sprite\
`bool auto_set_size` - if `true`, sprite image size will be automatically set to the original image size, otherwise width and height of the image need to be set manually\
`int grad_mvmt_iter` - number of iterations (frames) required for gradual movement to be completed\
`double grad_mvmt_speedx`, `double grad_mvmt_speedy` - horizontal and vertical speed of sprite during gradual movement\
`double grad_mvmt_goalx`, `double grad_mvmt_goaly` - goal coordinates of gradual movement, the point at which the sprite should be when gradual movement is completed.

### Sprite()


