/*
  !! WARNING !!
  This demo is a complete mess. It is temporary and only for testing purposes,
  and does not exactly reflect the best way to use this library
*/
#include <iostream>
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

#define COLOR_WHITE {255, 255, 255}
#define COLOR_GREEN {0, 255, 0}

#define PLAYER_SPEED 10

int main(int argc, char *argv[]) {

  gdk::initAll();

  SDL_DisplayMode display;
  SDL_GetCurrentDisplayMode(0, &display);

  int win_w = 1920;
  int win_h = 1080;

  SDL_Window *window = SDL_CreateWindow("sdl2-gdk: cars (collision demo)", 0, 0,
                                        win_w, win_h, SDL_WINDOW_INPUT_FOCUS);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetVSync(renderer, 1);

  SDL_Event event;

  gdk::ImageTexture tx_car_red(renderer, RED_CAR_PATH);
  gdk::ImageTexture tx_car_green(renderer, GREEN_CAR_PATH);
  gdk::ImageTexture tx_car_blue(renderer, BLUE_CAR_PATH);
  gdk::ImageTexture tx_background(renderer, BG_PATH);
  gdk::FontTexture ftx_fps(renderer, "FPS: 000", FONT_PATH, 8, {0, 255, 0});
  gdk::AnimatedTexture atx_stickman(renderer, STICKMAN_PATH, 5, 100);

  gdk::CollideSprite cs_player(&atx_stickman, 100, 100, 80, 128, 3);
  gdk::CollideSprite cs_car_left(&tx_car_green, -500, 100, 180, 120);
  gdk::CollideSprite cs_car_right(&tx_car_blue, 480, 300, 180, 120);
  gdk::CollideSprite cs_car_top(&tx_car_red, 900, -400, 180, 120);
  gdk::CollideSprite cs_car_bottom(&tx_car_green, 300, 420, 180, 120);
  gdk::CollideSprite cs_car_center(&tx_car_blue, 300, 300, 180, 120);
  gdk::Sprite s_car_1(&tx_car_blue, 400, 600, 180, 120, 1);
  gdk::Sprite s_car_2(&tx_car_red, 500, 600, 180, 120, 2);
  gdk::Sprite s_car_3(&tx_car_green, 600, 600, 180, 120, 3);
  gdk::Sprite s_car_4(&tx_car_blue, 700, 600, 180, 120, 4);
  gdk::Sprite s_background(&tx_background, 0, 0, win_w, win_h);
  gdk::Sprite s_fps(&ftx_fps, 5, 5);

  gdk::Camera camera({&s_background, &s_fps});

  gdk::Scene sc_game(renderer);

  camera.attachScene(&sc_game);
  sc_game.addSprite(
      {&s_background, &s_fps, &s_car_1, &s_car_3, &s_car_4, &s_car_2});
  sc_game.addCollideSprite({&cs_player, &cs_car_left, &cs_car_right,
                            &cs_car_top, &cs_car_bottom, &cs_car_center});
  sc_game.setShowHitboxOutlines(true);

  std::string fps_digit;

  while (event.type != SDL_QUIT) {
    fps_digit = "FPS: " + std::to_string(gdk::calculateFPS());

    SDL_PollEvent(&event);

    ftx_fps.setText(fps_digit.c_str());

    // Movement
    switch (event.key.keysym.sym) {
    case SDLK_w:
      cs_player.move(0, -PLAYER_SPEED);
      break;

    case SDLK_s:
      cs_player.move(0, PLAYER_SPEED);
      break;

    case SDLK_a:
      cs_player.move(-PLAYER_SPEED, 0);
      break;

    case SDLK_d:
      cs_player.move(PLAYER_SPEED, 0);
      break;
    // Animation play/pause
    case SDLK_SPACE:
      if (atx_stickman.isPlaying()) {
        atx_stickman.pause();
      } else {
        atx_stickman.play();
      }
      break;
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

    sc_game.draw();
  }

  sc_game.destroy();
  gdk::quitAll();

  return 0;
}