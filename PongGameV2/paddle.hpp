#ifndef PADDLE_HPP
#define PADDLE_HPP

class Paddle{
public:
  Paddle(int x, int y, int speed);
  ~Paddle();
  updateLocation(int relay);

  int x, y;
  int speed;

  static const int WIDTH;
  static const int HEIGHT;
};
#endif
