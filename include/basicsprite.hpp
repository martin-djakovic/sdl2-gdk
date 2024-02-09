#ifndef BASIC_SPRITE_HPP
#define BASIC_SPRITE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <direction.hpp>
#include <stdexcept>
#include <string>
#include <vector>

// Image that represents an object in-game
// Has basic functions for drawing and movement
class BasicSprite {

  protected:
    double x;
    double y;
    // Movement speed
    double speed;
    // Width
    int w;
    // Height
    int h;
    double rotation_angle;
    SDL_Point rotation_center;
    std::string img_path;
    SDL_RendererFlip flip;
    SDL_Renderer *sprite_renderer;
    SDL_Surface *surface;
    SDL_Texture *texture = nullptr;
    SDL_Rect img_rect;
    bool auto_set_size;

    // Variables used for gradual movement across multiple frames
    int grad_mvmt_iter = 0;
    double grad_mvmt_speedx;
    double grad_mvmt_speedy;
    double grad_mvmt_goalx;
    double grad_mvmt_goaly;

    // Used to gradually move sprite according to data from MoveTo()
    // Needed when movement is to be made across multiple frames
    // Is called on every draw
    virtual void GradualMovementIterator();

  public:
    BasicSprite();

    BasicSprite(double x, double y, int width, int height, const char *img_path,
                double speed = 0, bool auto_set_size = false,
                double rotation_angle = 0,
                SDL_RendererFlip flip = SDL_FLIP_NONE);

    void Destroy();

    // Set x position of sprite
    virtual void SetX(double x);
    int GetX();

    // Move sprite along x axis by value of x
    virtual void MoveX(double x);

    // Move sprite along x axis by value of speed
    virtual void MoveX(GDK_Direction direction);

    // Set y position of sprite
    virtual void SetY(double y);
    int GetY();

    // Move sprite along y axis by value of y
    virtual void MoveY(double y);

    // Move sprite along y axis by value of speed
    virtual void MoveY(GDK_Direction direction);

    // Set width
    void SetW(int w);
    // Get width
    int GetW();

    // Set height
    void SetH(int h);
    // Get height
    int GetH();

    void SetSpeed(double speed);
    double GetSpeed();

    // Moves sprite to given point
    // Evenly changes sprite x and y by given speed
    // until point is reached
    void MoveTo(int x, int y);

    // Set sprite renderer and create image passed in constructor
    // Renderer will be automatically set when sprite is added to a scene
    virtual void SetRenderer(SDL_Renderer *renderer);

    // Change sprite image
    virtual void SetImg(const char *img_path, bool auto_set_size = false);

    // Set sprite flip to SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL or
    // SDL_FLIP_VERTICAL
    void SetFlip(SDL_RendererFlip flip);

    // Sets sprite rotation to angle
    void SetRotation(double angle);

    // Changes sprite rotation by angle
    void Rotate(double angle);

    virtual void Draw();

    // Checks if sprite is in window bounds
    bool IsInBounds();
};

#endif