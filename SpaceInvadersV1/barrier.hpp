#ifndef BARRIER_HPP
#define BARRIER_HPP

#include "SDL.h"

class Barrier{
public:
  explicit Barrier(int x, int y, int w, int h);
  void decreaseLife();
  SDL_Rect getShape();
  inline int getLife() { return life; }

private:
  int life;
  SDL_Rect shape;
};

#endif
