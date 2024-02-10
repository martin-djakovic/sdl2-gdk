#include <textblock.hpp>

TextBlock::TextBlock(const std::vector<std::string> text, const char *font_path,
                     int font_size, SDL_Color color, double x, double y)
    : BasicSprite() {
    this->x = x;
    this->y = y;
    this->font_path = font_path;
    this->font_size = font_size;
    this->color = color;
    this->text = text;
}

void TextBlock::CreateTextLines() {
    if (sprite_renderer == nullptr) {
        throw "Cannot parse text before setting renderer";
        return;
    }

    for (int i = 0; i < lines.size(); i++) {
        // First destroy the texture of the line, then the line itself
        lines.at(i)->Destroy();
        delete lines.at(i);
    }

    lines.clear();

    for (int i = 0; i < text.size(); i++) {
        // Create lines of text
        lines.push_back(new TextLine(text.at(i).c_str(), font_path, font_size,
                                     color, x, y));

        TextLine *current_line = lines.at(i);

        current_line->SetRenderer(sprite_renderer);

        // Reposition text lines (we needed to create the lines first to
        // know their width and height)
        current_line->SetY(y + current_line->GetH() * i);
        // ^ Above code could cause problems if lines are different font
        // size

        // Set width and height of entire text block
        if (current_line->GetW() > w) {
            w = lines.at(i)->GetW();
        }

        h += lines.at(i)->GetH();
    }
}

void TextBlock::SetImg(const char *img_path, bool param_auto_set_size) {
    throw "Text can't call BasicSprite::SetImg()";
}

void TextBlock::SetRenderer(SDL_Renderer *renderer) {
    sprite_renderer = renderer;
    CreateTextLines();
}

void TextBlock::Draw() {
    for (int i = 0; i < lines.size(); i++) {
        lines.at(i)->Draw();
    }
}

void TextBlock::SetText(const std::vector<std::string> text) {
    this->text = text;
    CreateTextLines();
}

void TextBlock::SetColor(SDL_Color color) {
    this->color = color;
    CreateTextLines();
}

SDL_Color TextBlock::GetColor() { return color; }

void TextBlock::SetSize(int font_size) {
    this->font_size = font_size;
    CreateTextLines();
}

void TextBlock::SetFont(const char *font_path) {
    this->font_path = font_path;
    CreateTextLines();
}