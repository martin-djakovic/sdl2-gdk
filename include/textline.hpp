#ifndef TEXT_LINE_HPP
#define TEXT_LINE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <basicsprite.hpp>
#include <iostream>

// Contains single line of text
class TextLine : public BasicSprite {
  protected:
    TTF_Font *font;
    SDL_Color color;
    const char *font_path;
    int font_size;
    std::string text;

    void UpdateTexture();

  public:
    TextLine(std::string text, const char *font_path, int font_size,
             SDL_Color color, double x, double y);

    void SetImg(const char *img_path,
                bool param_auto_set_size = false) override;

    void SetRenderer(SDL_Renderer *renderer);

    void SetText(std::string text);

    void SetColor(SDL_Color color);

    SDL_Color GetColor();

    void SetSize(int font_size);

    void SetFont(const char *font_path);
};

#endif