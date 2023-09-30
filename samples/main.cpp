#include "../src/sdl2-gdk.hpp"
#include <iostream>

#define WHITE_SQR_PATH "res/whitesqr.png"
#define RED_SQR_PATH "res/redsqr.png"
#define WIN_W 1280
#define WIN_H 720
#define PLAYER_SPEED 7

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("Fatal error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_DisplayMode display;
    SDL_GetCurrentDisplayMode(0, &display);

    SDL_Window *window = SDL_CreateWindow(
        "sdl2-gdk test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W,
        WIN_H, SDL_WINDOW_INPUT_FOCUS);

    bool running = true;
    SDL_Event event;

    Sprite player(500, 500, 50, 50, RED_SQR_PATH);

    Sprite s1(100, 100, 50, 150, WHITE_SQR_PATH);         // Center sprite
    Sprite s2(100, -200, 100, 50, WHITE_SQR_PATH);        // Top sprite
    Sprite s3(-200, 100, 50, 70, WHITE_SQR_PATH);         // Left sprite
    Sprite s4(WIN_W + 100, 300, 80, 150, WHITE_SQR_PATH); // Right sprite
    Sprite s5(100, WIN_H + 100, 50, 50, WHITE_SQR_PATH);  // Bottom sprite

    Camera camera(&s1);

    Scene scene(window, -1, SDL_RENDERER_ACCELERATED, &camera);

    scene.Add(&s1);
    scene.Add(&s2);
    scene.Add(&s3);
    scene.Add(&s4);
    scene.Add(&s5);
    scene.Add(&player);

    std::vector<Sprite *> moving_sprites;

    for (int i = 0; i < 50; i++) {
        moving_sprites.push_back(
            new Sprite(1280 + 80 * i, 50, 50, 50, WHITE_SQR_PATH, false));
        moving_sprites.push_back(
            new Sprite(1280 + 80 * i, 150, 50, 50, WHITE_SQR_PATH, false));
        moving_sprites.push_back(
            new Sprite(1280 + 80 * i, 250, 50, 50, WHITE_SQR_PATH, false));
        moving_sprites.push_back(
            new Sprite(1280 + 80 * i, 350, 50, 50, WHITE_SQR_PATH));
        moving_sprites.push_back(
            new Sprite(1280 + 80 * i, 450, 50, 50, WHITE_SQR_PATH));
        moving_sprites.push_back(
            new Sprite(1280 + 80 * i, 550, 50, 50, WHITE_SQR_PATH));
        moving_sprites.push_back(
            new Sprite(1280 + 80 * i, 650, 50, 50, WHITE_SQR_PATH));
    }

    for (int i = 0; i < moving_sprites.size(); i++) {
        scene.Add(moving_sprites.at(i));
    }

    // Main game loop
    while (running) {

        scene.Draw();

        for (int i = 0; i < moving_sprites.size(); i++) {
            moving_sprites.at(i)->MoveX(-1);
        }

        while (SDL_PollEvent(&event)) {

            switch (event.key.keysym.sym) {
            case SDLK_w:
                player.MoveY(-PLAYER_SPEED);
                break;

            case SDLK_a:
                player.MoveX(-PLAYER_SPEED);
                break;

            case SDLK_s:
                player.MoveY(PLAYER_SPEED);
                break;

            case SDLK_d:
                player.MoveX(PLAYER_SPEED);
                break;

            default:
                break;
            }
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Check if player is out of bounds and move camera to player
        if (!player.IsVisible(WIN_W, WIN_H)) {
            // Out of bounds right
            if (player.GetX() >= WIN_W) {
                camera.x -= WIN_W;
            }

            // Out of bounds left
            else if (player.GetX() + player.w <= 0) {
                camera.x += WIN_W;
            }

            // Out of bounds top
            else if (player.GetY() + player.h <= 0) {
                camera.y += WIN_H;
            }

            else if (player.GetY() >= WIN_H) {
                camera.y -= WIN_H;
            }
        }

        // Lock refresh rate
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}