#include "../src/sdl2-gdk.hpp"
#include <iostream>

#define WHITE_SQR_PATH "res/whitesqr.png"
#define RED_SQR_PATH "res/redsqr.png"
#define BG_PATH "res/bg.png"
#define FONT_PATH "res/mechanical.otf"
#define WIN_W 1280
#define WIN_H 720
#define PLAYER_SPEED 7

int main(int argc, char *argv[]) {

    SDL2_GDK_Init();

    SDL_DisplayMode display;
    SDL_GetCurrentDisplayMode(0, &display);

    SDL_Window *window = SDL_CreateWindow(
        "sdl2-gdk test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W,
        WIN_H, SDL_WINDOW_INPUT_FOCUS);

    SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

    Sprite player(500, 500, 50, 50, RED_SQR_PATH);

    Sprite s1(100, 100, 50, 150, WHITE_SQR_PATH);         // Center sprite
    Sprite s2(100, -200, 100, 50, WHITE_SQR_PATH);        // Top sprite
    Sprite s3(-200, 100, 50, 70, WHITE_SQR_PATH);         // Left sprite
    Sprite s4(WIN_W + 100, 300, 80, 150, WHITE_SQR_PATH); // Right sprite
    Sprite s5(100, WIN_H + 100, 50, 50, WHITE_SQR_PATH);  // Bottom sprite

    Sprite background(0, 0, 1280, 720, BG_PATH, false);

    UIText hud("HELLO WORLD!", FONT_PATH, 32, {0, 255, 0}, 20, 20);
    UIText main_menu_text("PRESS SPACE TO START", FONT_PATH, 32, {100, 50, 100},
                          WIN_W / 2, WIN_H / 2);

    Camera game_camera(&background);
    Camera main_menu_camera;

    Scene game_scene(window, &game_camera);
    Scene main_menu(window, &main_menu_camera);

    game_scene.AddSprite({&background, &s1, &s2, &s3, &s4, &s5, &player});
    game_scene.AddUI(&hud);

    main_menu.AddUI(&main_menu_text);

    std::vector<Sprite *> moving_sprites;

    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 7; j++) {
            moving_sprites.push_back(new Sprite(1280 + 80 * i, j * 100 + 50, 50,
                                                50, WHITE_SQR_PATH));
        }
    }

    for (int i = 0; i < moving_sprites.size(); i++) {
        game_scene.AddSprite(moving_sprites.at(i));
    }

    main_menu.Draw();

    while (true) {
        SDL_PollEvent(&event);

        if (event.key.keysym.sym == SDLK_SPACE) {
            break;
        }
    }

    // Main game loop
    while (running) {

        game_scene.Draw();

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

        // Check if player is out of bounds and move game_camera to player
        if (!player.IsVisible(WIN_W, WIN_H)) {
            // Out of bounds right
            if (player.GetX() >= WIN_W) {
                game_camera.x -= WIN_W;
            }

            // Out of bounds left
            else if (player.GetX() + player.w <= 0) {
                game_camera.x += WIN_W;
            }

            // Out of bounds top
            else if (player.GetY() + player.h <= 0) {
                game_camera.y += WIN_H;
            }

            // Out of bounds bottom
            else if (player.GetY() >= WIN_H) {
                game_camera.y -= WIN_H;
            }
        }

        if (player.Collided()) {
            game_scene.RemoveSprite(player.GetCollideSprite());
        }

        // Lock refresh rate
        SDL_Delay(1000 / 60);
    }

    game_scene.Destroy();
    SDL_DestroyWindow(window);
    SDL2_GDK_Quit();

    return 0;
}