#include "../../src/sdl2-gdk.hpp"

#define BG_PATH "res/img/road.png"
#define RED_CAR_PATH "res/img/red_car.png"
#define GREEN_CAR_PATH "res/img/green_car.png"
#define BLUE_CAR_PATH "res/img/blue_car.png"
#define CRASHED_CAR_PATH "res/img/crashed_car.png"
#define CRASH_BLOCK_PATH "res/img/crash_block.png"
#define FONT_PATH "res/font/font.ttf"
#define CRASH_SOUND_PATH "res/audio/crash.wav"

#define COLOR_WHITE                                                            \
    { 255, 255, 255 }
#define COLOR_GREEN                                                            \
    { 0, 255, 0 }

#define OBSTACLE_COUNT 3
#define CAR_COUNT 500
#define CAR_SPEED 10
#define CAR_SOUND_COUNT 70

#define CAR_BG_W_RATIO 0.0703125
#define CAR_BG_H_RATIO 0.0833333333333

int main(int argc, char *argv[]) {

    SDL2_GDK_Init();

    SDL_DisplayMode display;
    SDL_GetCurrentDisplayMode(0, &display);

    int win_w = 1920;
    int win_h = 1080;

    SDL_Window *window =
        SDL_CreateWindow("sdl2-gdk crash_test (collision demo)", 0, 0, win_w,
                         win_h, SDL_WINDOW_INPUT_FOCUS);

    SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;

    BasicSprite bs_background(0, 0, win_w, win_h, BG_PATH);
    Sound s_crash(CRASH_SOUND_PATH, 20);

    TextLine tl_loading("LOADING...", FONT_PATH, 10, COLOR_WHITE, 5, 5);
    TextBlock tl_start_game({"PRESS SPACE FOR OPTIMIZED MODE",
                             "PRESS BACKSPACE FOR BENCHMARK MODE"},
                            FONT_PATH, 10, COLOR_WHITE, 5, 5);
    // FPS and car count
    TextBlock tb_stats({""}, FONT_PATH, 8, COLOR_GREEN, 5, 5);

    Camera cam_main_menu;
    Camera cam_game;

    Scene sc_main_menu(window, &cam_main_menu);
    Scene sc_game(window, &cam_game);

    // Draw loading screen
    sc_main_menu.AddObject(&tl_loading);
    sc_main_menu.Draw();

    sc_game.AddObject(&bs_background);

    std::vector<const char *> car_colors = {RED_CAR_PATH, GREEN_CAR_PATH,
                                            BLUE_CAR_PATH};
    const char *car_color;
    int random_color_index;
    int car_x;
    int car_y;
    int car_w = bs_background.GetW() * CAR_BG_W_RATIO;
    int car_h = bs_background.GetH() * CAR_BG_H_RATIO;
    int car_speed = CAR_SPEED;
    CollideSprite *cs_car;
    std::vector<CollideSprite *> cs_cars;

    int obstacle_x;
    int obstacle_y;
    int obstacle_w = bs_background.GetW() / 30;
    int obstacle_h = bs_background.GetH() / 4;
    CollideSprite *cs_obstacle;

    GDK_SetRandomSeed((unsigned)time(NULL));

    // Generate cars
    for (int i = 0; i < CAR_COUNT; i++) {
        random_color_index = GDK_GetRandomInt(0, car_colors.size() - 1);
        car_color = car_colors[random_color_index];
        car_x = win_w * 1.5 + 2 * i * car_w;
        car_y = (i % 6) * (car_w * 1.2) + 0.5 * car_w;

        cs_car =
            new CollideSprite(car_x, car_y, car_w, car_h, car_color, car_speed);

        sc_game.AddCollideObject(cs_car);
        cs_cars.push_back(cs_car);
    }

    // Generate obstacles
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        obstacle_x = (OBSTACLE_COUNT + 1) * obstacle_w * 2 - i * obstacle_w * 3;
        obstacle_y = (i + 1) * obstacle_h / 4 + i * obstacle_h;

        cs_obstacle = new CollideSprite(obstacle_x, obstacle_y, obstacle_w,
                                        obstacle_h, CRASH_BLOCK_PATH);

        sc_game.AddCollideObject(cs_obstacle);
    }

    sc_game.AddObject(&tb_stats);

    // Draw start game screen
    sc_main_menu.RemoveObject(&tl_loading);
    sc_main_menu.AddObject(&tl_start_game);
    sc_main_menu.Draw();

    // Determines if demo should be optimized,
    // or if it should be a benchmark
    bool optimized_mode;

    while (true) {
        SDL_PollEvent(&event);

        if (event.key.keysym.sym == SDLK_SPACE) {
            optimized_mode = true;
            break;
        } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
            optimized_mode = false;
            break;
        }
    }

    int fps;
    float cs_car_speed;
    // Number of cars that make a sound upon collision
    int car_sound_count = CAR_SOUND_COUNT;

    sc_game.SetHitboxOutlineColor({255, 255, 0});
    sc_game.SetShowHitboxOutlines(true);

    while (true) {
        fps = sc_game.GetFPS();
        tb_stats.SetText({"FPS: " + std::to_string(fps),
                          "CAR COUNT: " + std::to_string(cs_cars.size())});

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }

        for (int i = 0; i < cs_cars.size(); i++) {
            if (fps > 0) {
                cs_car = cs_cars.at(i);
                cs_car_speed = -cs_car->GetSpeed() / (fps / 100.0f);

                // If running optimized mode, only check collisions for sprites
                // that are visible
                // If running benchmark mode, pointlessly check collisions for
                // all sprites
                if (optimized_mode) {
                    if (cs_car->IsInBounds()) {
                        cs_car->MoveX(cs_car_speed);
                    } else {
                        cs_car->MoveX(cs_car_speed, false);
                    }
                } else {
                    cs_car->MoveX(cs_car_speed);
                }

                // Crash event
                if (cs_car->Collided()) {
                    cs_car->SetImg(CRASHED_CAR_PATH);
                    cs_car->Rotate(rand() % 20 - 10);

                    if (car_sound_count > 0) {
                        s_crash.Play();
                        car_sound_count--;
                    }

                    cs_cars.erase(
                        std::remove(cs_cars.begin(), cs_cars.end(), cs_car),
                        cs_cars.end());
                }
            }
        }

        sc_game.Draw();
    }

    sc_main_menu.Destroy();
    sc_game.Destroy();
    SDL2_GDK_Quit();

    return 0;
}