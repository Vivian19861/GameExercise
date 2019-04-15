#include <vector>
#include <cstdlib>
#include <cmath>

#include "SDL.h"
#include "SDL_mixer.h"

constexpr int windowWidth{800}, windowHeight{600};
constexpr float ballLength{10.0f}, ballVelocity{8.0f};
constexpr float paddleWidth{200.0f}, paddleHeight{20.0f}, paddleVelocity{6.0f};
constexpr float blockWidth{60.0f}, blockHeight{20.0f};
constexpr int countBlockX{14}, countBlockY{4};


struct Ball{
  SDL_Rect shape;
  float vx = -ballVelocity;
  float vy = -ballVelocity;

  Ball(float x, float y){
    shape.x = x;
    shape.y = y;
    shape.w = ballLength;
    shape.h = ballLength;
  }
  void update(){
    shape.x += vx;
    shape.y += vy;
    if(left() < 0) vx = ballVelocity;
    if(right() > windowWidth) vx = -ballVelocity;
    if(top() < 0) vy = ballVelocity;
    if(bottom() > windowHeight) vy = -ballVelocity;
  }

  float left() { return shape.x; }
  float right() { return shape.x+ballLength; }
  float top() { return shape.y; }
  float bottom() {return shape.y+ballLength; }
};

struct Paddle{
  SDL_Rect shape;

  Paddle(float x, float y){
    shape.x = x;
    shape.y = y;
    shape.w = paddleWidth;
    shape.h = paddleHeight;
  }

  void update(int relay){
    shape.x += paddleVelocity * relay;
    if(left() < 0) shape.x = 0;
    if(right() > windowWidth) shape.x = windowWidth-paddleWidth;
  }

  float left() { return shape.x; }
  float right() { return shape.x+paddleWidth; }
  float top() { return shape.y; }
  float bottom() { return shape.y+paddleHeight; }
};

struct Brick{
  SDL_Rect shape;

  bool destroyed{false};
  Brick(float x, float y){
    shape.x = x;
    shape.y = y;
    shape.w = blockWidth;
    shape.h = blockHeight;
  }

  float left() { return shape.x; }
  float right() { return shape.x+blockWidth; }
  float top() { return shape.y; }
  float bottom() { return shape.y+blockHeight; }
};

void testCollision(const Paddle& paddle, Ball& ball, Mix_Chunk* paddle_sound){
  if(!SDL_HasIntersection(&paddle.shape, &ball.shape)) return;

  Mix_PlayChannel(-1, paddle_sound, 0);
  ball.vy = -ballVelocity;
  if(ball.shape.x < paddle.shape.x){
    ball.vx = -ballVelocity;
  } else {
    ball.vx = ballVelocity;
  }
}

void testCollision(Brick& brick, Ball& ball, Mix_Chunk* brick_sound){
  if(!SDL_HasIntersection(&brick.shape, &ball.shape)) return;

  Mix_PlayChannel(-1, brick_sound, 0);
  brick.destroyed = true;
  float overlapLeft{ball.right()-brick.left()};
  float overlapRight{brick.right()-ball.left()};
  float overlapTop{ball.bottom()-brick.top()};
  float overlapBottom{brick.bottom()-ball.top()};

  bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));
  bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

  float minOverlapX{ballFromLeft? overlapLeft:overlapRight};
  float minOverlapY{ballFromTop? overlapTop:overlapBottom};

  if(abs(minOverlapX) < abs(minOverlapY)){
    ball.vx = ballFromLeft? -ballVelocity:ballVelocity;
  } else {
    ball.vy = ballFromTop? -ballVelocity:ballVelocity;
  }
}

int main(int argc, char* argv[]){

  //Initialize window and renderer
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window *win;
  SDL_Renderer *ren;
  win = SDL_CreateWindow("Arkanoids by Wei",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         windowWidth,
                         windowHeight,
                         SDL_WINDOW_SHOWN);
  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  SDL_Event event;
  bool quit = false;

  //Sounds
  Mix_Chunk* paddle_sound;
  Mix_Chunk* brick_sound;
  Mix_Chunk* background_sound;
  Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
  paddle_sound = Mix_LoadWAV("resources/sounds/paddle_hit.wav");
  brick_sound = Mix_LoadWAV("resources/sounds/wall_hit.wav");
  background_sound = Mix_LoadWAV("resources/sounds/music.mp3");

  Ball ball{windowWidth/2, windowHeight/2};
  Paddle paddle{windowWidth/2, windowHeight-paddleHeight};
  std::vector<Brick> bricks;
  std::vector<Brick>::iterator it;

  for(int i=0; i<countBlockX; ++i){
    for(int j=0; j<countBlockY; ++j){
      bricks.emplace_back((i)*(blockWidth+3), (j)*(blockHeight+3));
    }
  }

  Mix_PlayChannel(-1, background_sound, 0);
  //Game loop
  while(!quit){
    //Input handling
    while(SDL_PollEvent(&event) != 0){
      if(event.type == SDL_QUIT){
        quit = true;
      }
      if(event.type == SDL_KEYDOWN){
        switch(event.key.keysym.sym){
          case SDLK_ESCAPE:
            quit = true;
            break;

          case SDLK_LEFT:
            paddle.update(-1);
            break;

          case SDLK_RIGHT:
            paddle.update(1);
            break;
        }
      }
    }

    //Clear Screen
    SDL_SetRenderDrawColor(ren, 0,0,0,255);
    SDL_RenderClear(ren);

    //update status
    ball.update();
    testCollision(paddle, ball, paddle_sound);
    for(auto& brick:bricks){
      testCollision(brick, ball, brick_sound);
    }

    for(auto it=bricks.begin(); it!=bricks.end(); ++it){
      if(it->destroyed == true){
        bricks.erase(it);
      }
    }

    //Renderer, draw ball
    SDL_SetRenderDrawColor(ren, 255,255,0,255);
    SDL_RenderFillRect(ren, &ball.shape);

    //Draw paddle
    SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
    SDL_RenderFillRect(ren, &paddle.shape);

    //Draw bricks
    for(auto& brick:bricks){
      SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
      SDL_RenderFillRect(ren, &brick.shape);
    }

    //Swap buffers
    SDL_RenderPresent(ren);
  }

  //CleanUp
  Mix_FreeChunk(paddle_sound);
  Mix_FreeChunk(brick_sound);
  Mix_CloseAudio();

  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  SDL_Quit();
  return 0;
}
