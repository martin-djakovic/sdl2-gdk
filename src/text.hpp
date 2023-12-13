#ifndef TEXT_SPRITE_HPP
#define TEXT_SPRITE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "basicsprite.hpp"

class Text : public BasicSprite {
  protected:
    TTF_Font *font;
    SDL_Color color;
    const char *font_path;
    int font_size;
    const char *text;

    void UpdateTexture() {
        font = TTF_OpenFont(font_path, font_size);
        surface = TTF_RenderText_Solid(font, text, color);
        texture = SDL_CreateTextureFromSurface(sprite_renderer, surface);

        SDL_QueryTexture(texture, NULL, NULL, &img_rect.w, &img_rect.h);

        w = img_rect.w;
        h = img_rect.h;

        SDL_FreeSurface(surface);
    }

  public:
    Text(const char *text, const char *font_path, int font_size,
         SDL_Color color, double x, double y)
        : BasicSprite() {
        this->text = text;
        this->font_path = font_path;
        this->font_size = font_size;
        this->color = color;
        this->x = x;
        this->y = y;
    }

    void ChangeImg(const char *img_path,
                   bool param_auto_set_size = false) override {
        throw "Text can't call BasicSprite::ChangeImg()";
    }

    void SetRenderer(SDL_Renderer *renderer) {
        sprite_renderer = renderer;

        UpdateTexture();
    }

    void SetText(const char *text) {
        this->text = text;
        UpdateTexture();
    }

    void SetColor(SDL_Color color) {
        this->color = color;
        UpdateTexture();
    }

    void SetSize(int font_size) {
        this->font_size = font_size;
        UpdateTexture();
    }

    void SetFont(const char *font_path) {
        this->font_path = font_path;
        UpdateTexture();
    }
};

#endif