#ifndef TEXT_HPP
#define TEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class Text {
  public:
    int x = 0;
    int y = 0;
    SDL_Rect hitbox;

    Text(const char *text, const char *font_path, int font_size, SDL_Color color, int x, int y){
        this->text = text;
        this->font_path = font_path;
        this->font_size = font_size;
        this->color = color;
        this->x = x;
        this->y = y;
    }

    void Destroy() { SDL_DestroyTexture(texture); }

    // Set text renderer and create image passed in constructor
    // Renderer will be automatically set when text is added to a scene
    void SetRenderer(SDL_Renderer *renderer) {
        text_renderer = renderer;

        font = TTF_OpenFont(font_path, font_size);
        surface = TTF_RenderText_Solid(font, text, color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_QueryTexture(texture, NULL, NULL, &hitbox.w, &hitbox.h);

        SDL_FreeSurface(surface);
    }

    void SetText(const char *text) {

        font = TTF_OpenFont(font_path, font_size);
        surface = TTF_RenderText_Solid(font, text, color);
        texture = SDL_CreateTextureFromSurface(text_renderer, surface);

        SDL_QueryTexture(texture, NULL, NULL, &hitbox.w, &hitbox.h);

        SDL_FreeSurface(surface);
    }

    void Draw() {
        hitbox.x = x;
        hitbox.y = y;

        SDL_RenderCopy(text_renderer, texture, NULL, &hitbox);
    }

  private:
    TTF_Font *font;
    SDL_Renderer *text_renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color color;
    const char *font_path;
    int font_size;
    const char *text;
};

#endif