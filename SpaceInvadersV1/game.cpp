#include "game.hpp"

void Game::run() {
  while (!quit) {
    inputHandling();
    update();
    render();
  }
}

Game::Game()
    : player(windowWidth / 2 - playerWidth / 2, windowHeight - playerHeight),
      quit{false}, alienMoveRight{true}, alienMoveDown{false}, score{0},
      score_changed{false} {
  init();
}

void Game::init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  } else {
    gWindow = SDL_CreateWindow("SpaceInvaderV1 by Wei", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, windowWidth,
                               windowHeight, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    } else {
      gRender = SDL_CreateRenderer(
          gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }
  }
  // initialize background texture
  backgroundTexture = loadBMPTexture("background.bmp");
  backgroundRect.x = 0;
  backgroundRect.y = 0;
  backgroundRect.w = windowWidth;
  backgroundRect.h = windowHeight;

  // initialize Player
  playerTexture = IMG_LoadTexture(gRender, "player.png");
  if (playerTexture == nullptr) {
    printf("Failed to load playerTexture! SDL_Error: %s\n", SDL_GetError());
  }

  // initialize aliens
  alienTexture = IMG_LoadTexture(gRender, "invaders3.png");
  if (alienTexture == nullptr) {
    printf("Failed to load alienTexture! SDL_Error: %s\n", SDL_GetError());
  }
  alienDeadTexture = IMG_LoadTexture(gRender, "deadAlien.png");
  if (alienDeadTexture == nullptr) {
    printf("Failed to load alienDeadTexture! SDL_Error: %s\n", SDL_GetError());
  }
  aliens.reserve(countAlienX * countAlienY);
  for (int i = 0; i < countAlienX; ++i) {
    for (int j = 0; j < countAlienY; ++j) {
      aliens.emplace_back(i * (alienWidth + 20), j * (alienHeight + 10));
    }
  }

  // initialize barriers
  barrierCompleteTexture = IMG_LoadTexture(gRender, "barrier1.png");
  if (barrierCompleteTexture == nullptr) {
    printf("Failed to load barrierCompleteTexture! SDL_Error: %s\n",
           SDL_GetError());
  }
  barrierDamagedTexture = IMG_LoadTexture(gRender, "barrier2.png");
  if (barrierDamagedTexture == nullptr) {
    printf("Failed to load barrierDamagedTexture! SDL_Error: %s\n",
           SDL_GetError());
  }
  barrierFinalDamagedTexture = IMG_LoadTexture(gRender, "barrier3.png");
  if (barrierFinalDamagedTexture == nullptr) {
    printf("Failed to load barrierFinalDamagedTexture! SDL_Error: %s\n",
           SDL_GetError());
  }
  int w, h;
  SDL_QueryTexture(barrierCompleteTexture, NULL, NULL, &w, &h);
  barriers.reserve(3);
  for (int i = 0; i < 3; ++i) {
    barriers.emplace_back(i * 200 + 150, 500, w, h);
  }

  // Sound effects
  Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
  alien_hit_sound = Mix_LoadWAV("wall_hit.wav");

  // Fonts
  TTF_Init();
  font_color = {255, 255, 255, 255};
  font_name = "NES-Chimera/NES-Chimera.ttf";
}

SDL_Texture *Game::loadBMPTexture(char *filename) {
  SDL_Surface *surface = SDL_LoadBMP(filename);
  if (surface == nullptr) {
    printf("Failed to load surface! SDL_Error: %s\n", SDL_GetError());
    return nullptr;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(gRender, surface);
  SDL_FreeSurface(surface);
  return texture;
}

void Game::inputHandling() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quit = true;
    }
    if (event.type == SDL_KEYDOWN) {
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        quit = true;
        break;

      case SDLK_LEFT:
        player.moveLeft();
        break;

      case SDLK_RIGHT:
        player.moveRight();
        break;

      case SDLK_SPACE:
        player.fireLaser();
        break;
      }
    }
  }
}

void Game::update() {
  if (player.getLaserFired() == true) {
    player.updateShots();
  }

  int randomValue = rand() % (aliens.size() + 1);
  if (rand() % 150 <= 1) {
    aliens[randomValue].fireShot();
  }

  // alien movement
  for (auto &alien : aliens) {
    // check alien movement direction
    if (alien.getShape().x >= windowWidth) {
      alienMoveRight = false;
      alienMoveDown = true;
    }
    if (alien.getShape().x <= 0) {
      alienMoveRight = true;
      alienMoveDown = true;
    }

    // produce randow alien shots
    if (alien.getShotFired()) {
      alien.updateShots();
    }

    // check collision, player's shots with alien
    if (player.didLaserHit(alien.getShape())) {
      alien.setIsDestroyed(true);
      Mix_PlayChannel(-1, alien_hit_sound, 0);
      score += 30;
      score_changed = true;
    }

    // check collision, alien's shots with player
    if (alien.didShotHit(player.getShape())) {
      player.setIsDestroyed(true);
    }

    // check collision, alien's shots with barrier
    for (auto &barrier : barriers) {
      if (alien.didShotHit(barrier.getShape())) {
        barrier.decreaseLife();
      }
    }
  }

  // check collision, player's shots with barrier
  for (auto &barrier : barriers) {
    if (player.didLaserHit(barrier.getShape())) {
      barrier.decreaseLife();
    }
  }

  for (auto &alien : aliens) {
    if (alienMoveRight) {
      alien.moveRight();
    } else {
      alien.moveLeft();
    }
    if (alienMoveDown) {
      alien.moveDown();
    }
  }
  alienMoveDown = false;
}

void Game::render() {
  // clear screen
  SDL_RenderClear(gRender);
  // Render background
  SDL_RenderCopy(gRender, backgroundTexture, NULL, &backgroundRect);
  // Render player
  if (player.getIsDestroyed() == false) {
    SDL_Rect shape = player.getShape();
    SDL_RenderCopy(gRender, playerTexture, NULL, &shape);
    for (auto &laser : (player.lasers)) {
      SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255);
      SDL_RenderFillRect(gRender, &laser.shape);
    }
  } else {
    font_image_restart =
        renderText("Game Over!", font_name, font_color, 24, gRender);

    renderTexture(font_image_restart, gRender, windowWidth / 2,
                  windowHeight / 3);
  }

  // Render aliens
  for (auto it = aliens.begin(); it != aliens.end();) {
    SDL_Rect shape = it->getShape();
    if (it->getIsDestroyed() == true) {
      SDL_RenderCopy(gRender, alienDeadTexture, NULL, &shape);
      it = aliens.erase(it);
    } else {
      SDL_RenderCopy(gRender, alienTexture, NULL, &shape);
      for (auto &shot : (it->shots)) {
        SDL_SetRenderDrawColor(gRender, 255, 255, 255, 255);
        SDL_RenderFillRect(gRender, &shot.shape);
      }
      ++it;
    }
  }

  // Render barriers
  for (auto it = barriers.begin(); it != barriers.end();) {
    SDL_Rect shape = it->getShape();
    if (it->getLife() == 10) {
      SDL_RenderCopy(gRender, barrierCompleteTexture, NULL, &shape);
      ++it;
    } else if (it->getLife() < 10 && it->getLife() > 5) {
      SDL_RenderCopy(gRender, barrierDamagedTexture, NULL, &shape);
      ++it;
    } else if (it->getLife() <= 5 && it->getLife() > 0) {
      SDL_RenderCopy(gRender, barrierFinalDamagedTexture, NULL, &shape);
      ++it;
    } else if (it->getLife() == 0) {
      it = barriers.erase(it);
    }
  }

  // render score
  if (aliens.size() == 0) {
    font_image_winner =
        renderText("You won!", font_name, font_color, 24, gRender);

    renderTexture(font_image_winner, gRender, windowWidth / 2,
                  windowHeight / 3);
  }

  if (score_changed) {
    font_image_score =
        renderText(std::to_string(score), font_name, font_color, 14, gRender);
    score_changed = false;
  }
  renderTexture(font_image_score, gRender, 10, 10);

  SDL_RenderPresent(gRender);
}

void Game::close() {
  SDL_DestroyTexture(backgroundTexture);
  backgroundTexture = nullptr;
  SDL_DestroyTexture(playerTexture);
  playerTexture = nullptr;
  SDL_DestroyTexture(alienTexture);
  alienTexture = nullptr;
  SDL_DestroyTexture(alienDeadTexture);
  alienDeadTexture = nullptr;
  SDL_DestroyTexture(barrierCompleteTexture);
  barrierCompleteTexture = nullptr;
  SDL_DestroyTexture(barrierDamagedTexture);
  barrierDamagedTexture = nullptr;
  SDL_DestroyTexture(barrierFinalDamagedTexture);
  barrierFinalDamagedTexture = nullptr;
  SDL_DestroyTexture(font_image_score);
  font_image_score = nullptr;
  SDL_DestroyTexture(font_image_winner);
  font_image_winner = nullptr;
  SDL_DestroyTexture(font_image_restart);
  font_image_restart = nullptr;

  Mix_FreeChunk(alien_hit_sound);
  Mix_CloseAudio();

  SDL_DestroyRenderer(gRender);
  gRender = nullptr;
  SDL_DestroyWindow(gWindow);
  gWindow = nullptr;

  SDL_Quit();
}

Game::~Game() { close(); }
