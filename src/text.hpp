#ifndef TEXT_HPP
#define TEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class UIText {
  public:
    int x = 0;
    int y = 0;
    int w;
    int h;

    UIText(const char *text, const char *font_path, int font_size,
           SDL_Color color, int x, int y) {
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

        MakeTexture(font_path, font_size, text, color);
    }

    void SetText(const char *text) {
        this->text = text;
        MakeTexture(font_path, font_size, text, color);
    }

    void SetColor(SDL_Color color) {
        this->color = color;
        MakeTexture(font_path, font_size, text, color);
    }

    void SetSize(int font_size) {
        this->font_size = font_size;
        MakeTexture(font_path, font_size, text, color);
    }

    void SetFont(const char *font_path) {
        this->font_path = font_path;
        MakeTexture(font_path, font_size, text, color);
    }

    void Draw() {
        rect.x = x;
        rect.y = y;

        SDL_RenderCopy(text_renderer, texture, NULL, &rect);
    }

  private:
    TTF_Font *font;
    SDL_Renderer *text_renderer;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Color color;
    SDL_Rect rect;
    const char *font_path;
    int font_size;
    const char *text;

    // Create texture of text based on given arguements
    void MakeTexture(const char *font_path, int font_size, const char *text,
                     SDL_Color color) {
        font = TTF_OpenFont(font_path, font_size);
        surface = TTF_RenderText_Solid(font, text, color);
        texture = SDL_CreateTextureFromSurface(text_renderer, surface);

        SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

        w = rect.w;
        h = rect.h;

        SDL_FreeSurface(surface);
    }
};

#endif