#include <textline.hpp>

TextLine::TextLine(std::string text, const char *font_path, int font_size,
                   SDL_Color color, double x, double y)
    : BasicSprite() {
    this->text = text;
    this->font_path = font_path;
    this->font_size = font_size;
    this->color = color;
    this->x = x;
    this->y = y;
}

void TextLine::UpdateTexture() {
    SDL_DestroyTexture(texture);

    font = TTF_OpenFont(font_path, font_size);

    // Check if given font is valid
    if (font == NULL) {
        printf("Failed loading font: %s\n", font_path);
    }

    surface = TTF_RenderText_Solid(font, text.c_str(), color);
    texture = SDL_CreateTextureFromSurface(sprite_renderer, surface);

    SDL_QueryTexture(texture, NULL, NULL, &img_rect.w, &img_rect.h);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);

    w = img_rect.w;
    h = img_rect.h;
}

void TextLine::SetImg(const char *img_path, bool param_auto_set_size) {
    throw "Text can't call BasicSprite::SetImg()";
}

void TextLine::SetRenderer(SDL_Renderer *renderer) {
    sprite_renderer = renderer;
    UpdateTexture();
}

void TextLine::SetText(std::string text) {
    this->text = text;
    UpdateTexture();
}

void TextLine::SetColor(SDL_Color color) {
    this->color = color;
    UpdateTexture();
}

SDL_Color TextLine::GetColor() { return color; }

void TextLine::SetSize(int font_size) {
    this->font_size = font_size;
    UpdateTexture();
}

void TextLine::SetFont(const char *font_path) {
    this->font_path = font_path;
    UpdateTexture();
}