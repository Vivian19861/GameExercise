#ifndef BALL_HPP
#define BALL_HPP

#include "paddle.hpp"

class Ball {
public:
  Ball(int x, int y);
  ~Ball();
  void launch_ball();
  void updateLocation();
  void reset();
  void update_speed();
  bool collides_with(const Paddle &paddle);
  void bounces_off(const Paddle &paddle);
  bool wall_collision();

  static const int LENGTH;
  static const float BALL_INIT_SPEED;
  static const float BALL_MAX_SPEED;
  int x, y;
  int vx, vy;
  float speed;
  float angle;
  int hits;
};
#endif
