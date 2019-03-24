#ifndef PLAYER_HPP
#define PALYER_HPP

#include <vector>

#include "SDL.h"

constexpr int playerWidth{45}, playerHeight{45};

struct LaserShot{
  SDL_Rect shape;

  LaserShot(int x, int y){
    shape.x = x;
    shape.y = y;
    shape.w = 5;
    shape.h = 10;
  }
  double velocity;
};

class Player{
public:
  SDL_Rect shape;
  explicit Player(int x, int y);
  double velocity, laserVelocity;
  bool laserFired;
  bool isDestroyed;
  std::vector<LaserShot> lasers;
  void moveRight();
  void moveLeft();
  void fireLaser();
  void updateShots();
  bool didLaserHit(SDL_Rect temp);
};
#endif
