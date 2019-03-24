#ifndef BARRIER_HPP
#define BARRIER_HPP

#include "SDL.h"

class Barrier{
public:
  SDL_Rect shape;
  explicit Barrier(int x, int y, int w, int h);
  int life;
};

#endif
