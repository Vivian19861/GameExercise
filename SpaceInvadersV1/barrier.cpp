#include "barrier.hpp"

Barrier::Barrier(int x, int y, int w, int h){
  shape.x = x;
  shape.y = y;
  shape.w = w;
  shape.h = h;
  life = 10;
}
