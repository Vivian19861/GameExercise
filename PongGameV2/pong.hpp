#ifndef PONG_HPP
#define PONG_HPP

#include <string>

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "ball.hpp"
#include "paddle.hpp"

class Pong {
private:
  SDL_Window *win;
  SDL_Renderer *ren;

  Ball ball;
  Paddle leftPaddle;
  Paddle rightPaddle;

  Mix_Chunk *paddle_sound;
  Mix_Chunk *wall_sound;
  Mix_Chunk *update_score_sound;

  // Fonts
  std::string font_name;
  SDL_Color font_color;
  SDL_Texture *font_image_left_score;
  SDL_Texture *font_image_right_score;
  SDL_Texture *font_image_winner;
  SDL_Texture *font_image_restart;

  int left_score;
  int right_score;
  bool left_score_changed;
  bool right_scoure_changed;
  bool exit;

public:
  Pong();
  ~Pong();
  static const int SCREEN_WIDTH;
  static const int SCREEN_HEIGHT;
  void execute();
  void input();
  void update();
  void render();
};
#endif
