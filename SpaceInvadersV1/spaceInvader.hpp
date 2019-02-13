#ifndef SPACE_INVADER_HPP
#define SPACE_INVADER_HPP
#include "SDL.h"

constexpr int windowWidth{800}, windowHeight{600};
constexpr int countAlienX{11}, countAlienY{5};

void init();
SDL_Texture* loadBMPTexture(char* filename);
void inputHandling();
void update();
void render();
void close();
#endif
