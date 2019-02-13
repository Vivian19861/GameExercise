#include <stdlib.h>
#include <cmath>

#include "ball.hpp"
#include "pong.hpp"

const int Ball::LENGTH = 10;
const float Ball::BALL_INIT_SPEED = 4.0f;
const float Ball::BALL_MAX_SPEED = 8.0f;

Ball::Ball(int x, int y){
  this->x = x;
  this->y = y;
  vx = (rand()%2 == 0)? BALL_INIT_SPEED: -1*BALL_INIT_SPEED;
  vy = -0.5f;
  speed = 0;
  angle = 0.0f;
  hits = 0;
}

void Ball::update_speed(){
  if(hits == 5){
    speed++;
    hits = 0;
  }
  if(speed >= BALL_MAX_SPEED){
    speed = BALL_MAX_SPEED;
  }
}

bool Ball::collides_with(Paddle *paddle){
    if((x > paddle->x + paddle->WIDTH) ||
       (x + LENGTH < paddle->x) ||
       (y > paddle->y + paddle->HEIGHT) ||
       (y + LENGTH < paddle->y)){
         return false;
       } else {
         return true;
       }
}

void Ball::bounces_off(Paddle *paddle){
  if(paddle == nullptr) return;
  hits++;

  int sign = (paddle->x < Pong::SCREEN_WIDTH/2)? 1:-1;
  int relative_y = y-paddle->y+LENGTH;
  angle = (2.14f * relative_y -75.0f);
  vx = sign * speed * std::cos(angle*M_PI/180.0f);
  vy = speed*std::sin(angle*M_PI/180.0f);
}

bool Ball::wall_collision(){
  return (y+vy <0) || (y+LENGTH+vy >= Pong::SCREEN_HEIGHT);
}

void Ball::reset(){
  x = Pong::SCREEN_WIDTH/2 - LENGTH/2;
  y = Pong::SCREEN_HEIGHT/2;

  speed = BALL_INIT_SPEED;
  hits = 0;
}
