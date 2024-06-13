/*
  !! WARNING !!
  This demo is a complete mess. It is temporary and only for testing purposes,
  and does not exactly reflect the best way to use this library
*/
#include <sdl2-gdk.hpp>
#include <string>

#define BG_PATH "res/img/road.png"
#define RED_CAR_PATH "res/img/red_car.png"
#define GREEN_CAR_PATH "res/img/green_car.png"
#define BLUE_CAR_PATH "res/img/blue_car.png"
#define CRASHED_CAR_PATH "res/img/crashed_car.png"
#define CRASH_BLOCK_PATH "res/img/crash_block.png"
#define STICKMAN_PATH "res/img/stickman.png"
#define FONT_PATH "res/font/font.ttf"
#define CRASH_SOUND_PATH "res/audio/crash.wav"

#define COLOR_WHITE                                                            \
  { 255, 255, 255 }
#define COLOR_GREEN                                                            \
  { 0, 255, 0 }

#define PLAYER_SPEED 500

int main(int argc, char *argv[]) {

  gdkInit();

  SDL_DisplayMode display;
  SDL_GetCurrentDisplayMode(0, &display);

  int win_w = 1920;
  int win_h = 1080;

  SDL_Window *window = SDL_CreateWindow("sdl2-gdk: cars (collision demo)", 0, 0,
                                        win_w, win_h, SDL_WINDOW_INPUT_FOCUS);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Event event;

  GDK_ImageTexture tx_car_red(renderer, RED_CAR_PATH);
  GDK_ImageTexture tx_car_green(renderer, GREEN_CAR_PATH);
  GDK_ImageTexture tx_car_blue(renderer, BLUE_CAR_PATH);
  GDK_ImageTexture tx_background(renderer, BG_PATH);
  GDK_FontTexture ftx_fps(renderer, "FPS: 00000", FONT_PATH, 8, {0, 255, 0});
  GDK_AnimatedTexture atx_stickman(renderer, STICKMAN_PATH, 5, 100);

  GDK_CollideSprite cs_player(&atx_stickman, 100, 100, 80, 128);
  GDK_CollideSprite cs_car_left(&tx_car_green, -500, 100, 180, 120);
  GDK_CollideSprite cs_car_right(&tx_car_blue, 2500, 600, 180, 120);
  GDK_CollideSprite cs_car_top(&tx_car_red, 900, -400, 180, 120);
  GDK_CollideSprite cs_car_bottom(&tx_car_green, 500, 1400, 180, 120);
  GDK_CollideSprite cs_car_center(&tx_car_blue, 300, 300, 180, 120);
  GDK_Sprite s_background(&tx_background, 0, 0, win_w, win_h);
  GDK_Sprite s_fps(&ftx_fps, 5, 5);

  GDK_Camera camera({&s_background, &s_fps});
  GDK_Scene sc_game(renderer, &camera);

  sc_game.addSprite({&s_background, &s_fps});
  sc_game.addCollideSprite({&cs_player, &cs_car_left, &cs_car_right,
                            &cs_car_top, &cs_car_bottom, &cs_car_center});
  sc_game.setShowHitboxOutlines(true);

  double perf_multiplier;
  std::string fps_digit = "";

  while (event.type != SDL_QUIT) {
    SDL_PollEvent(&event);

    perf_multiplier = sc_game.getPerformanceMultiplier();

    fps_digit = std::to_string(sc_game.getFPS());

    while (fps_digit.length() < 5) {
      fps_digit = "0" + fps_digit;
    }

    fps_digit = "FPS: " + fps_digit;

    ftx_fps.setText(fps_digit.c_str());

    // Movement
    if (perf_multiplier != 1) {
      switch (event.key.keysym.sym) {
      case SDLK_w:
        cs_player.move(0, -PLAYER_SPEED * perf_multiplier);
        break;

      case SDLK_s:
        cs_player.move(0, PLAYER_SPEED * perf_multiplier);
        break;

      case SDLK_a:
        cs_player.move(-PLAYER_SPEED * perf_multiplier, 0);
        break;

      case SDLK_d:
        cs_player.move(PLAYER_SPEED * perf_multiplier, 0);
        break;
      // Animation play/pause
      case SDLK_SPACE:
        if (atx_stickman.isPlaying()) {
          atx_stickman.pause();
        } else {
          atx_stickman.play();
        }
      }

      // Player is off the screen
      if (!cs_player.isInBounds()) {
        if (cs_player.getX() >= win_w) {
          camera.move(-win_w, 0);
        } else if (cs_player.getX() + cs_player.getWidth() <= 0) {
          camera.move(win_w, 0);
        } else if (cs_player.getY() + cs_player.getHeight() <= 0) {
          camera.move(0, win_h);
        } else if (cs_player.getY() >= win_h) {
          camera.move(0, -win_h);
        }
      }
    }

    sc_game.draw();
  }

  sc_game.destroy();
  gdkQuit();

  return 0;
}