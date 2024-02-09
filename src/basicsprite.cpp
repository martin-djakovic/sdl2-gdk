#include <basicsprite.hpp>

BasicSprite::BasicSprite() {
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

BasicSprite::BasicSprite(double x, double y, int width, int height,
                         const char *img_path, double speed, bool auto_set_size,
                         double rotation_angle, SDL_RendererFlip flip) {
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

void BasicSprite::Destroy() { SDL_DestroyTexture(texture); }

void BasicSprite::SetX(double x) { this->x = x; }
int BasicSprite::GetX() { return round(x); }

void BasicSprite::MoveX(double x) { SetX(this->x + x); }

void BasicSprite::SetY(double y) { this->y = y; }
int BasicSprite::GetY() { return round(y); }

void BasicSprite::MoveY(double y) { SetY(this->y + y); }

void BasicSprite::SetW(int w) {
    if (w >= 0) {
        this->w = w;
    } else {
        throw "Width cannot be less than 0";
    }
}

int BasicSprite::GetW() { return w; }

void BasicSprite::SetH(int h) {
    if (h >= 0) {
        this->h = h;
    } else {
        throw "Height cannot be less than 0";
    }
}

int BasicSprite::GetH() { return h; }

void BasicSprite::SetSpeed(double speed) { this->speed = speed; }
double BasicSprite::GetSpeed() { return speed; }

void BasicSprite::MoveTo(int x, int y) {
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

void BasicSprite::GradualMovementIterator() {
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

void BasicSprite::SetRenderer(SDL_Renderer *renderer) {
    sprite_renderer = renderer;

    SetImg(img_path.c_str(), auto_set_size);
}

void BasicSprite::SetImg(const char *img_path, bool auto_set_size) {
    SDL_DestroyTexture(texture);

    surface = IMG_Load(img_path);

    // Check if given image is valid
    if (surface == NULL) {
        printf("Failed loading image: %s\n", img_path);
    }

    texture = SDL_CreateTextureFromSurface(sprite_renderer, surface);
    SDL_FreeSurface(surface);

    if (auto_set_size) {
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    }

    img_rect.w = w;
    img_rect.h = h;
}

void BasicSprite::SetFlip(SDL_RendererFlip flip) { this->flip = flip; }

void BasicSprite::SetRotation(double angle) { rotation_angle = angle; }

void BasicSprite::Rotate(double angle) { SetRotation(rotation_angle + angle); }

void BasicSprite::Draw() {
    img_rect.x = x;
    img_rect.y = y;
    img_rect.w = w;
    img_rect.h = h;

    // Complete step of gradual movement on every frame (when sprite is
    // drawn)
    GradualMovementIterator();

    SDL_RenderCopyEx(sprite_renderer, texture, NULL, &img_rect, rotation_angle,
                     &rotation_center, flip);
}

bool BasicSprite::IsInBounds() {
    int win_w, win_h;
    SDL_GetRendererOutputSize(sprite_renderer, &win_w, &win_h);

    if (x + w > 0 && y + w > 0 && x < win_w && y < win_h) {
        return true;
    } else {
        return false;
    }
}