#include "pong.hpp"
#include "utity.hpp"

const int Pong::SCREEN_WIDTH = 640;
const int Pong::SCREEN_HEIGHT = 480;

Pong::Pong()
    : ball(SCREEN_WIDTH / 2 - ball.LENGTH / 2,
           SCREEN_HEIGHT / 2 - ball.LENGTH / 2),
      leftPaddle(40, SCREEN_HEIGHT / 2 - Paddle::HEIGHT / 2, 10),
      rightPaddle(SCREEN_WIDTH - (40 + Paddle::WIDTH),
                  SCREEN_HEIGHT / 2 - Paddle::HEIGHT / 2, 10),
      left_score{0}, right_score{0}, left_score_changed{false},
      right_scoure_changed{false}, exit{false} {
  SDL_Init(SDL_INIT_EVERYTHING);

  win = SDL_CreateWindow("Pong V2 By Wei", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                         SDL_WINDOW_SHOWN);

  ren = SDL_CreateRenderer(
      win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // Sounds
  Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);

  paddle_sound = Mix_LoadWAV("resources/sounds/paddle_hit.wav");
  wall_sound = Mix_LoadWAV("resources/sounds/wall_hit.wav");
  update_score_sound = Mix_LoadWAV("resources/sounds/score_update.wav");

  // Fonts
  TTF_Init();
  font_color = {255, 255, 255, 255};
  font_name = "resources/fonts/NES-Chimera/NES-Chimera.ttf";
}

Pong::~Pong() {
  // Destroy Texture
  SDL_DestroyTexture(font_image_left_score);
  SDL_DestroyTexture(font_image_right_score);
  SDL_DestroyTexture(font_image_winner);
  SDL_DestroyTexture(font_image_restart);

  // Free sound effects
  Mix_FreeChunk(paddle_sound);
  Mix_FreeChunk(wall_sound);
  Mix_FreeChunk(update_score_sound);

  Mix_CloseAudio();

  // Destroy renderer and window
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  // shuts down SDL
  SDL_Quit();
}

void Pong::execute() {
  while (!exit) {
    input();
    update();
    render();
  }
}

void Pong::input() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      exit = true;
    }
    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        exit = true;
        break;

      case SDLK_UP:
        leftPaddle.updateLocation(-1);
        break;

      case SDLK_DOWN:
        leftPaddle.updateLocation(1);
        break;

      case SDLK_q:
        rightPaddle.updateLocation(-1);
        break;

      case SDLK_r:
        rightPaddle.updateLocation(1);
        break;
      }
    }
  }
}

// update game vales
void Pong::update() {
  ball.update_speed();

  // Collision
  if (ball.collides_with(leftPaddle)) {
    ball.bounces_off(leftPaddle);
    Mix_PlayChannel(-1, paddle_sound, 0);
  } else if (ball.collides_with(rightPaddle)) {
    ball.bounces_off(rightPaddle);
    Mix_PlayChannel(-1, paddle_sound, 0);
  }

  // Upper and bottom walls Collision
  if (ball.wall_collision()) {
    ball.vy *= -1;
    Mix_PlayChannel(-1, wall_sound, 0);
  }

  // update ball coordinates
  ball.x += ball.vx;
  ball.y += ball.vy;

  // Ball goes out
  if ((ball.x < 0) || (ball.x > SCREEN_WIDTH)) {
    if (ball.x < 0) {
      right_score++;
      right_scoure_changed = true;
    } else {
      left_score++;
      left_score_changed = true;
    }
    Mix_PlayChannel(-1, update_score_sound, 0);
    ball.reset();
  }
}

void Pong::render() {
  // Clear Screen
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);

  // Paddle color
  SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

  SDL_Rect paddle1 = {leftPaddle.x, leftPaddle.y, Paddle::WIDTH,
                      Paddle::HEIGHT};
  SDL_RenderFillRect(ren, &paddle1);

  SDL_Rect paddle2 = {rightPaddle.x, rightPaddle.y, Paddle::WIDTH,
                      Paddle::HEIGHT};
  SDL_RenderFillRect(ren, &paddle2);

  // Render Ball
  SDL_Rect pong_ball = {ball.x, ball.y, ball.LENGTH, ball.LENGTH};
  SDL_RenderFillRect(ren, &pong_ball);

  // render score
  if (left_score_changed) {
    font_image_left_score =
        renderText(std::to_string(left_score), font_name, font_color, 24, ren);
    left_score_changed = false;
    if (left_score == 5) {
      font_image_winner =
          renderText("Player 1 won!", font_name, font_color, 24, ren);
      font_image_restart =
          renderText("Press SPACE to restart", font_name, font_color, 14, ren);
    }
  }
  renderTexture(font_image_left_score, ren, SCREEN_WIDTH * 4 / 10,
                SCREEN_HEIGHT / 12);

  if (right_scoure_changed) {
    font_image_right_score =
        renderText(std::to_string(right_score), font_name, font_color, 24, ren);
    right_scoure_changed = false;
    if (right_score == 5) {
      font_image_winner =
          renderText("Player 2 won!", font_name, font_color, 24, ren);
      font_image_restart =
          renderText("Press SPACE to restart", font_name, font_color, 24, ren);
    }
  }
  renderTexture(font_image_right_score, ren, SCREEN_WIDTH * 6 / 10 - 12,
                SCREEN_HEIGHT / 12);
  if (left_score == 5) {
    renderTexture(font_image_winner, ren, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4);
    renderTexture(font_image_restart, ren, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3);
    left_score = 0;
    right_score = 0;
    left_score_changed = true;
    right_scoure_changed = true;

  } else if (right_score == 5) {
    renderTexture(font_image_winner, ren, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4);
    renderTexture(font_image_restart, ren, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3);
    left_score = 0;
    right_score = 0;
    left_score_changed = true;
    right_scoure_changed = true;
  }
  // Swap buffers
  SDL_RenderPresent(ren);
}
