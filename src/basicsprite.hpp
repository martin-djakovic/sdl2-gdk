#ifndef BASIC_SPRITE_HPP
#define BASIC_SPRITE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>
#include <string>
#include <vector>

#include "direction.hpp"

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
    virtual void GradualMovementIterator() {
        if (grad_mvmt_iter > 0) {
            MoveX(grad_mvmt_speedx);
            MoveY(grad_mvmt_speedy);

            grad_mvmt_iter--;

            if (grad_mvmt_iter == 0 && x != grad_mvmt_goalx &&
                y != grad_mvmt_goalx) {
                SetX(grad_mvmt_goalx);
                SetY(grad_mvmt_goaly);
            }
        }
    }

  public:
    BasicSprite() {
        this->x = 0;
        this->y = 0;
        SetW(0);
        SetH(0);
        this->speed = 0;
        this->img_path = "";
        this->auto_set_size = false;
        this->rotation_angle = 0;
        this->flip = SDL_FLIP_NONE;
        rotation_center = {w / 2, h / 2};
    }

    BasicSprite(double x, double y, int width, int height, const char *img_path,
                double speed = 0, bool auto_set_size = false,
                double rotation_angle = 0,
                SDL_RendererFlip flip = SDL_FLIP_NONE) {
        this->x = x;
        this->y = y;
        SetW(width);
        SetH(height);
        this->speed = speed;
        this->img_path = img_path;
        this->auto_set_size = auto_set_size;
        this->rotation_angle = rotation_angle;
        this->flip = flip;
        rotation_center = {w / 2, h / 2};
    }

    void Destroy() { SDL_DestroyTexture(texture); }

    // Set x position of sprite
    virtual void SetX(double x) { this->x = x; }
    int GetX() { return round(x); }

    // Move sprite along x axis by value of x
    virtual void MoveX(double x) { SetX(this->x + x); }

    // Move sprite along x axis by value of speed
    virtual void MoveX(GDK_Direction direction) {
        MoveX(speed * ConvertDirectionToInt(direction));
    }

    // Set y position of sprite
    virtual void SetY(double y) { this->y = y; }
    int GetY() { return round(y); }

    // Move sprite along y axis by value of y
    virtual void MoveY(double y) { SetY(this->y + y); }

    // Move sprite along y axis by value of speed
    virtual void MoveY(GDK_Direction direction) {
        MoveY(speed * ConvertDirectionToInt(direction));
    }

    // Set width
    void SetW(int w) {
        if (w >= 0) {
            this->w = w;
        } else {
            throw "Width cannot be less than 0";
        }
    }
    // Get width
    int GetW() { return w; }

    // Set height
    void SetH(int h) {
        if (h >= 0) {
            this->h = h;
        } else {
            throw "Height cannot be less than 0";
        }
    }
    // Get height
    int GetH() { return h; }

    void SetSpeed(double speed) { this->speed = speed; }
    double GetSpeed() { return speed; }

    // Moves sprite to given point
    // Evenly changes sprite x and y by given speed
    // until point is reached
    void MoveTo(int x, int y) {
        grad_mvmt_goalx = x;
        grad_mvmt_goaly = y;

        int xdif = this->x - x;
        int ydif = this->y - y;
        // Movement iterations along line between this position and given
        // position
        grad_mvmt_iter = sqrt(xdif * xdif + ydif * ydif) / speed;

        if (grad_mvmt_iter == 0) {
            return;
        }

        grad_mvmt_speedx = -((double)xdif / grad_mvmt_iter);
        grad_mvmt_speedy = -((double)ydif / grad_mvmt_iter);
    }

    // Set sprite renderer and create image passed in constructor
    // Renderer will be automatically set when sprite is added to a scene
    virtual void SetRenderer(SDL_Renderer *renderer) {
        sprite_renderer = renderer;

        SetImg(img_path.c_str(), auto_set_size);
    }

    // Change sprite image
    virtual void SetImg(const char *img_path,
                           bool param_auto_set_size = false) {
        SDL_DestroyTexture(texture);

        img_rect.w = w;
        img_rect.h = h;

        surface = IMG_Load(img_path);
        texture = SDL_CreateTextureFromSurface(sprite_renderer, surface);
        SDL_FreeSurface(surface);

        if (param_auto_set_size) {
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        }
    }

    // Set sprite flip to SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL or
    // SDL_FLIP_VERTICAL
    void SetFlip(SDL_RendererFlip flip) { this->flip = flip; }

    // Sets sprite rotation to angle
    void SetRotation(double angle) { rotation_angle = angle; }

    // Changes sprite rotation by angle
    void Rotate(double angle) { SetRotation(rotation_angle + angle); }

    virtual void Draw() {
        img_rect.x = x;
        img_rect.y = y;
        img_rect.w = w;
        img_rect.h = h;

        // Complete step of gradual movement on every frame (when sprite is
        // drawn)
        GradualMovementIterator();

        SDL_RenderCopyEx(sprite_renderer, texture, NULL, &img_rect,
                         rotation_angle, &rotation_center, flip);
    }

    // Checks if sprite is in window bounds
    bool IsVisible(int win_w, int win_h) {
        if (x + w > 0 && y + w > 0 && x < win_w && y < win_h) {
            return true;
        } else {
            return false;
        }
    }
};

#endif