#ifndef TEXT_BLOCK_HPP
#define TEXT_BLOCK_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "textline.hpp"

// Contains multiple lines of text
class TextBlock : public BasicSprite {
  protected:
    std::vector<TextLine *> lines;
    const char *font_path;
    int font_size;
    SDL_Color color;
    std::vector<const char *> text;

    // Parse text vector into lines of text
    // Should only be called after SetRenderer() is set
    // Sets renderer for individual TextLines
    void CreateTextLines() {
        if (sprite_renderer == nullptr) {
            throw "Cannot parse text before setting renderer";
            return;
        }

        for (int i = 0; i < lines.size(); i++) {
            delete lines.at(i);
        }

        lines.clear();

        for (int i = 0; i < text.size(); i++) {
            // Create lines of text
            lines.push_back(
                new TextLine(text.at(i), font_path, font_size, color, x, y));
            
            lines.at(i)->SetRenderer(sprite_renderer);

            // Reposition text lines (we needed to create the lines first to
            // know their width and height)
            lines.at(i)->SetY(y + lines.at(i)->GetH() * i);
            // ^ Above code could cause problems if lines are different font
            // size
        }
    }

  public:
    TextBlock(const std::vector<const char *> text, const char *font_path,
              int font_size, SDL_Color color, double x, double y)
        : BasicSprite() {
        this->x = x;
        this->y = y;
        this->font_path = font_path;
        this->font_size = font_size;
        this->color = color;
        this->text = text;
    }

    void ChangeImg(const char *img_path,
                   bool param_auto_set_size = false) override {
        throw "Text can't call BasicSprite::ChangeImg()";
    }

    void SetRenderer(SDL_Renderer *renderer) {
        sprite_renderer = renderer;
        CreateTextLines();
    }

    void Draw() override {
        for (int i = 0; i < lines.size(); i++) {
            lines.at(i)->Draw();
        }
    }

    void SetText(const std::vector<const char *> text) {
        this->text = text;
        CreateTextLines();
    }

    void SetColor(SDL_Color color) {
        this->color = color;
        CreateTextLines();
    }

    void SetSize(int font_size) {
        this->font_size = font_size;
        CreateTextLines();
    }

    void SetFont(const char *font_path) {
        this->font_path = font_path;
        CreateTextLines();
    }
};

#endif