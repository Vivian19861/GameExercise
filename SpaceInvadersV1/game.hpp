#ifndef PLAYER_HPP
#define PALYER_HPP

#include <memory>
#include <stdio.h>
#include <stdlib.h>

#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "alien.hpp"
#include "barrier.hpp"
#include "player.hpp"
#include "spaceInvader.hpp"
#include "utity.hpp"

class Game {
public:
  Game();
  ~Game();
  void run();

private:
  void init();
  SDL_Texture *loadBMPTexture(char *filename);
  void inputHandling();
  void update();
  void render();
  void close();

  SDL_Window *gWindow;
  SDL_Renderer *gRender;
  SDL_Texture *backgroundTexture;
  SDL_Rect backgroundRect;
  SDL_Texture *playerTexture;
  Player player;

  bool quit;
  std::vector<Alien> aliens;
  SDL_Texture *alienTexture;
  SDL_Texture *alienDeadTexture;
  std::vector<Barrier> barriers;
  SDL_Texture *barrierCompleteTexture;
  SDL_Texture *barrierDamagedTexture;
  SDL_Texture *barrierFinalDamagedTexture;
  bool alienMoveRight;
  bool alienMoveDown;
  Mix_Chunk *alien_hit_sound;

  // Fonts
  std::string font_name;
  SDL_Color font_color;
  SDL_Texture *font_image_score;
  SDL_Texture *font_image_winner;
  SDL_Texture *font_image_restart;
  int score;
  bool score_changed;
};
#endif
