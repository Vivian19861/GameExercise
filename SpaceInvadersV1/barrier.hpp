#ifndef BARRIER_HPP
#define BARRIER_HPP

#include "SDL.h"

class Barrier{
public:
  explicit Barrier(int x, int y, int w, int h);
  int getLife();
  void decreaseLife();
  SDL_Rect getShape();

private:
  int life;
  SDL_Rect shape;
};

#endif
