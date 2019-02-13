#ifndef ALIEN_HPP
#define ALIEN_HPP

#include <vector>

#include "SDL.h"

constexpr int alienWidth{30}, alienHeight{30};

struct Shot{
  SDL_Rect shape;

  Shot(int x, int y){
    shape.x = x;
    shape.y = y;
    shape.w = 5;
    shape.h = 10;
  }
  double velocity;
};

class Alien{
public:
  SDL_Rect shape;
  Alien(int x, int y);
  double velocity, shotVelocity;
  bool shotFired;
  bool isDestroyed;
  std::vector<Shot> shots;
  void moveRight();
  void moveLeft();
  void moveDown();
  void updateRandomShot();
  void fireShot();
  void updateShots();
  bool didShotHit(SDL_Rect temp);
};

#endif
