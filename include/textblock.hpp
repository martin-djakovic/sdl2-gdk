#ifndef TEXT_BLOCK_HPP
#define TEXT_BLOCK_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <textline.hpp>

// Contains multiple lines of text
class TextBlock : public BasicSprite {
  protected:
    std::vector<TextLine *> lines;
    const char *font_path;
    int font_size;
    SDL_Color color;
    std::vector<std::string> text;

    // Parse text vector into lines of text
    // Should only be called after SetRenderer() is set
    // Sets renderer for individual TextLines
    void CreateTextLines();

  public:
    TextBlock(const std::vector<std::string> text, const char *font_path,
              int font_size, SDL_Color color, double x, double y);

    void SetImg(const char *img_path,
                bool param_auto_set_size = false) override;

    void SetRenderer(SDL_Renderer *renderer);

    void Draw() override;

    void SetText(const std::vector<std::string> text);

    void SetColor(SDL_Color color);

    SDL_Color GetColor();

    void SetSize(int font_size);

    void SetFont(const char *font_path);
};

#endif