#include <math.h>
#include <stdio.h>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "pong.hpp"
#include "utity.hpp"

typedef struct {
  float x;
  float y;
  float vx;
  float vy;
  float speed;
} ball;

typedef struct {
  SDL_Rect pos;
  int score;
  int speed;
} player;

float cal_angle(const float &y1, const float &y2, const int &height) {
  float rely = y1 + height / 2 - y2;
  rely /= height / 2.0;
  return rely * MAX_ANGLE;
}

void Init(SDL_Window **win, SDL_Renderer **ren) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL could not initialized! SDL_Error: %s\n", SDL_GetError());
  } else {
    *win = SDL_CreateWindow("Pong Game by Wei", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (*win == NULL) {
      printf("window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
      *ren = SDL_CreateRenderer(
          *win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
      if (*ren == NULL) {
        printf("renderer could not be created! SDL_Error: %s\n",
               SDL_GetError());
      }
    }
  }

  const int flags = IMG_INIT_PNG | IMG_INIT_JPG;
  if (IMG_Init(flags) != flags) {
    printf("Failed to load images! SDL_Error: %s\n", SDL_GetError());
  }

  if (TTF_Init() != 0) {
    printf("Failed to load TTF extension! SDL_Error: %s\n", SDL_GetError());
  }
}

int main(int argc, char *argv[]) {
  SDL_Event e;
  SDL_Window *gWindow = NULL;
  SDL_Renderer *gRenderer = NULL;

  Init(&gWindow, &gRenderer);

  int board_width;
  int board_height;
  SDL_Texture *squareTex = IMG_LoadTexture(gRenderer, "./img/pong_board.png");
  if (squareTex == NULL) {
    printf("Failed to load texture! SDL_Error: %s\n", SDL_GetError());
  }
  SDL_QueryTexture(squareTex, NULL, NULL, &board_width, &board_height);

  SDL_Color whiteColor = {255, 255, 255};
  SDL_Surface *fpsCounter;

  SDL_Rect b_rect;
  b_rect.w = BALL_WIDTH;
  b_rect.h = BALL_HEIGHT;

  player p1;
  player p2;

  ball b;
  b.x = SCREEN_WIDTH / 2;
  b.y = SCREEN_HEIGHT / 2;
  b.speed = BALL_INIT_SPEED;
  b.vx = (rand() % 2 == 0) ? BALL_INIT_SPEED : -1 * BALL_INIT_SPEED;
  b.vy = -0.5f;

  p1.score = p2.score = 0;
  p1.pos.w = p2.pos.w = board_width;
  p1.pos.h = p2.pos.h = 150;
  p1.speed = 10;
  p2.speed = 3.5;

  p1.pos.x = board_width / 2 + 10;
  p2.pos.x = SCREEN_WIDTH - p2.pos.w - 10 - p2.pos.w / 2;
  p1.pos.y = SCREEN_HEIGHT / 2 - p1.pos.h / 2;
  p2.pos.y = SCREEN_HEIGHT / 2 - p2.pos.h / 2;

  unsigned int prevTime = SDL_GetTicks();
  bool quit = false;
  int frames = 0;
  float fps;
  char buffer[512];
  const Uint8 *keystates = SDL_GetKeyboardState(NULL);

  while (!quit) {
    ++frames;
    unsigned int currTime = SDL_GetTicks();
    float elapsed = (currTime - prevTime);

    if (elapsed > 100) {
      fps = round(frames / (elapsed / 1000.0));
      frames = 0;
      prevTime = currTime;
    }

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;
      if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
          quit = true;
          break;
        }
      }
    }

    if (keystates[SDL_SCANCODE_UP]) {
      p1.pos.y -= p1.speed;
    }
    if (keystates[SDL_SCANCODE_DOWN]) {
      p1.pos.y += p1.speed;
    }

    if (keystates[SDL_SCANCODE_Q]) {
      p2.pos.y -= p2.speed;
    }
    if (keystates[SDL_SCANCODE_R]) {
      p2.pos.y += p2.speed;
    }

    /*
    //Basic AI
    if(b.y < p2.pos.y+p2.pos.h/2){
      p2.pos.y -= p2.speed;
    }

    if(b.y > p2.pos.y+p2.pos.h/2){
      p2.pos.y += p2.speed;
    }
    */

    if (b.vx > BALL_MAXSPEED) {
      b.vx = BALL_MAXSPEED;
    }

    if (b.vy > BALL_MAXSPEED) {
      b.vy = BALL_MAXSPEED;
    }

    b.x += b.vx;
    b.y += b.vy;

    if (b.y < 0) {
      b.y = 0;
      b.vy *= -1;
    }

    if (b.y + BALL_HEIGHT >= SCREEN_HEIGHT) {
      b.y = SCREEN_HEIGHT - BALL_HEIGHT - 1;
      b.vy *= -1;
    }

    if (b.x < 0) {
      p2.score += 1;
      b.x = p1.pos.x + p1.pos.w;
      b.y = p1.pos.y + p1.pos.h / 2;
      b.vx = BALL_INIT_SPEED;
      b.speed = BALL_INIT_SPEED;
    }

    if (b.x + BALL_WIDTH >= SCREEN_WIDTH) {
      p1.score += 1;
      b.x = p2.pos.x - BALL_WIDTH;
      b.y = p2.pos.y + p2.pos.h / 2;
      b.vx = -1 * BALL_INIT_SPEED;
      b.speed = BALL_INIT_SPEED;
    }

    if (p1.pos.y < 0)
      p1.pos.y = 0;
    if (p1.pos.y + p1.pos.h > SCREEN_HEIGHT)
      p1.pos.y = SCREEN_HEIGHT - p1.pos.h;
    if (p2.pos.y < 0)
      p2.pos.y = 0;
    if (p2.pos.y + p2.pos.h > SCREEN_HEIGHT)
      p2.pos.y = SCREEN_HEIGHT - p2.pos.h;

    // Update the b_rect structure
    b_rect.x = (int)b.x;
    b_rect.y = (int)b.y;

    if (SDL_HasIntersection(&p1.pos, &b_rect)) {
      b.x = p1.pos.x + p1.pos.w;
      b.speed = b.speed * BALL_ACCELERATE;
      float angle = cal_angle(p1.pos.y, b.y, p1.pos.h);
      b.vx = b.speed * cos(angle);
      b.vy = ((b.vy > 0) ? -1 : 1) * b.speed * sin(angle);
    }

    if (SDL_HasIntersection(&p2.pos, &b_rect)) {
      b.x = p2.pos.x - BALL_WIDTH;
      b.speed = b.speed * BALL_ACCELERATE;
      float angle = cal_angle(p2.pos.y, b.y, p2.pos.h);
      b.vx = -1 * b.speed * cos(angle);
      b.vy = ((b.vy > 0) ? -1 : 1) * b.speed * sin(angle);
    }

    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, squareTex, NULL, &p1.pos);
    SDL_RenderCopy(gRenderer, squareTex, NULL, &p2.pos);

    renderTexture(squareTex, gRenderer, SCREEN_WIDTH / 2 - CENTER_WIDTH / 2, 0,
                  CENTER_WIDTH, SCREEN_HEIGHT);
    renderTexture(squareTex, gRenderer, b.x, b.y, BALL_WIDTH, BALL_HEIGHT);

    sprintf(buffer, "%d", p1.score);
    SDL_Texture *p1score =
        renderText(buffer, "./fonts/sample.ttf", whiteColor, 40, gRenderer);
    sprintf(buffer, "%d", p2.score);
    SDL_Texture *p2score =
        renderText(buffer, "./fonts/sample.ttf", whiteColor, 40, gRenderer);

    int width;
    SDL_QueryTexture(p1score, NULL, NULL, &width, NULL);
    renderTexture(p1score, gRenderer, SCREEN_WIDTH / 2 - width - 10, 10);
    renderTexture(p2score, gRenderer, SCREEN_WIDTH / 2 + 10, 10);

    SDL_DestroyTexture(p1score);
    SDL_DestroyTexture(p2score);

    sprintf(buffer, "%.0f", fps);
    SDL_Texture *fpsCounter =
        renderText(buffer, "./fonts/sample.ttf", whiteColor, 20, gRenderer);
    renderTexture(fpsCounter, gRenderer, SCREEN_WIDTH - 20, 0);
    SDL_DestroyTexture(fpsCounter);

    SDL_RenderPresent(gRenderer);
  }

  SDL_DestroyTexture(squareTex);
  Cleanup(gWindow, gRenderer);
  return 0;
}

void Cleanup(SDL_Window *win, SDL_Renderer *ren) {
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}
