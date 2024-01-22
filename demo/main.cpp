#include "../src/sdl2-gdk.hpp"
#include <iostream>
#include <sstream>

#define WHITE_SQR_PATH "res/whitesqr.png"
#define RED_SQR_PATH "res/redsqr.png"
#define BG_PATH "res/bg.png"
#define FONT_PATH "res/mechanical.otf"
#define WIN_W 1280
#define WIN_H 720
#define PLAYER_SPEED 3

std::string ConvertDoubleToStr(double value) {
    std::stringstream ss;
    ss << value;
    std::string str = ss.str();
    return str;
}

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

    CollideSprite player(500, 500, 50, 50, RED_SQR_PATH, PLAYER_SPEED);
    BasicSprite player_hitbox(100, 100, 100, 100, BG_PATH);
    player.SetHitboxParams(10, 10, 30, 39);

    CollideSprite s1(100, 100, 50, 150, WHITE_SQR_PATH);  // Center sprite
    CollideSprite s2(100, -200, 100, 50, WHITE_SQR_PATH); // Top sprite
    s2.SetHitboxParams(50, 50, 50, 50);
    CollideSprite s3(-200, 100, 50, 70, WHITE_SQR_PATH);         // Left sprite
    CollideSprite s4(WIN_W + 100, 300, 80, 150, WHITE_SQR_PATH); // Right sprite
    CollideSprite s5(100, WIN_H + 100, 50, 50, WHITE_SQR_PATH); // Bottom sprite

    BasicSprite background(0, 0, 1280, 720, BG_PATH);

    TextLine hud("", FONT_PATH, 32, {0, 255, 0}, 20, 20);
    TextBlock system_info(GDK_GetSystemInfo(), FONT_PATH, 13, {0, 255, 0}, 10,
                          10);

    Camera game_camera({&background, &hud});
    Camera main_menu_camera;

    Scene game_scene(window, &game_camera);
    Scene main_menu(window, &main_menu_camera);

    game_scene.AddBasicSprite(&background);
    game_scene.AddCollideSprite({&s1, &s2, &s3, &s4, &s5, &player});
    game_scene.AddBasicSprite(&player_hitbox);
    game_scene.AddBasicSprite(&hud);

    main_menu.AddBasicSprite(&system_info);

    std::vector<CollideSprite *> moving_sprites;

    int mousex, mousey;

    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 7; j++) {
            moving_sprites.push_back(new CollideSprite(
                1280 + 80 * i, j * 100 + 50, 50, 50, WHITE_SQR_PATH, 1));
        }
    }

    for (int i = 0; i < moving_sprites.size(); i++) {
        game_scene.AddCollideSprite(moving_sprites.at(i));
    }

    main_menu.Draw();

    while (true) {
        SDL_PollEvent(&event);

        if (event.key.keysym.sym == SDLK_SPACE) {
            break;
        }
    }

    hud.SetColor({255, 50, 100});
    hud.SetSize(35);

    // Main game loop
    while (running) {
        player_hitbox.SetX(player.GetHitbox()->x);
        player_hitbox.SetY(player.GetHitbox()->y);
        player_hitbox.SetW(player.GetHitbox()->w);
        player_hitbox.SetH(player.GetHitbox()->h);
        
        game_scene.Draw();

        for (int i = 0; i < moving_sprites.size(); i++) {
            moving_sprites.at(i)->MoveX(-1);
        }

        while (SDL_PollEvent(&event)) {

            switch (event.key.keysym.sym) {
            case SDLK_w:
                player.MoveY(GDK_Direction::UP);
                break;

            case SDLK_a:
                player.MoveX(GDK_Direction::LEFT);
                break;

            case SDLK_s:
                player.MoveY(GDK_Direction::DOWN);
                break;

            case SDLK_d:
                player.MoveX(GDK_Direction::RIGHT);
                break;

            default:
                break;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mousex, &mousey);
                player.MoveTo(mousex, mousey);
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
            else if (player.GetX() + player.GetW() <= 0) {
                game_camera.x += WIN_W;
            }

            // Out of bounds top
            else if (player.GetY() + player.GetH() <= 0) {
                game_camera.y += WIN_H;
            }

            // Out of bounds bottom
            else if (player.GetY() >= WIN_H) {
                game_camera.y -= WIN_H;
            }
        }

        if (player.Collided()) {
            hud.SetColor({255, 0, 0});
            hud.SetText("COLLIDED!");
        } else {
            hud.SetColor({255, 255, 255});
            hud.SetText("NOT COLLIDED!");
        }

        // player.Rotate(0.5);

        // Lock refresh rate
        SDL_Delay(1000 / 60);
    }

    game_scene.Destroy();
    SDL_DestroyWindow(window);
    SDL2_GDK_Quit();

    return 0;
}