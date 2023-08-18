#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "sprite.hpp"

// Scene is used to group multiple sprites and draw them to the screen
// Only one scene at a time should be drawn in any single window
class Scene {

  public:
    SDL_Renderer *renderer;

    Scene(SDL_Window *window, int index, Uint32 flags) {
        renderer = SDL_CreateRenderer(window, index, flags);
    }

    ~Scene() {
        SDL_DestroyRenderer(renderer);
        sprites.clear();
    }

    void Add(Sprite sprite) {
        sprite.SetRenderer(renderer);
        sprites.push_back(sprite);        
    }

    // Draw all visible sprites in the scene
    void Draw() {
        //SDL_RenderClear(renderer);

        SDL_GetRendererOutputSize(renderer, &win_w, &win_h);

        for (int i = 0; i < sprites.size(); i++) {

            if (sprites.at(i).IsVisible(win_w, win_h)) {
                sprites.at(i).Draw();
            }
        }

        SDL_RenderPresent(renderer);
    }

  private:
    std::vector<Sprite> sprites;
    int win_w;
    int win_h;
};

#endif