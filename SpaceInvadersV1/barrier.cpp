#include "barrier.hpp"

Barrier::Barrier(int x, int y, int w, int h)
:life{10}
{
  shape.x = x;
  shape.y = y;
  shape.w = w;
  shape.h = h;
}

int Barrier::getLife(){
  return life;
}

void Barrier::decreaseLife(){
  life--;
}

SDL_Rect Barrier::getShape(){
  return shape;
}
