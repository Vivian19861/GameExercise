#ifndef PONG_HPP_
#define PONG_HPP_

#include <SDL.h>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

const int BALL_WIDTH = 10;
const int BALL_HEIGHT = 10;
const int CENTER_WIDTH = 5;

const float BALL_INIT_SPEED = 4.0f;
const float BALL_MAXSPEED = 8.0f;
const float BALL_ACCELERATE = 1.05f;
const float MAX_ANGLE = 3.142 / 5.0; // 72 degrees

void Init(SDL_Window **win, SDL_Renderer **ren);
void Cleanup(SDL_Window *win, SDL_Renderer *ren);
#endif
