#include "paddle.hpp"
#include "pong.hpp"

const int Paddle::WIDTH = 10;
const int Paddle::HEIGHT = 60;

Paddle::Paddle(int x, int y, int speed) : x{x}, y{y}, speed{speed} {}

Paddle::updateLocation(int relay) {
  y += (relay)*speed;
  if (y < 0)
    y = 0;
  if (y + HEIGHT > Pong::SCREEN_HEIGHT)
    y = Pong::SCREEN_HEIGHT - HEIGHT;
}
