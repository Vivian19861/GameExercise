#ifndef PLAYER_HPP
#define PALYER_HPP

#include <vector>

#include "SDL.h"

constexpr int playerWidth{45}, playerHeight{45};

struct LaserShot {
  SDL_Rect shape;

  LaserShot(int x, int y) {
    shape.x = x;
    shape.y = y;
    shape.w = 5;
    shape.h = 10;
  }
  double velocity;
};

class Player {
public:
  explicit Player(int x, int y);
  void moveRight();
  void moveLeft();
  void fireLaser();
  void updateShots();
  bool didLaserHit(SDL_Rect temp);
  bool getLaserFired() { return laserFired; }
  SDL_Rect getShape();
  void setIsDestroyed(bool isDestroyed);
  bool getIsDestroyed() { return isDestroyed; }
  std::vector<LaserShot> lasers;

private:
  SDL_Rect shape;
  double velocity, laserVelocity;
  bool laserFired;
  bool isDestroyed;
};
#endif
