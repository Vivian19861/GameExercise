#include <string>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

SDL_Texture *renderText(const std::string &msg, const std::string &fontPath, SDL_Color color, int fontSize, SDL_Renderer *ren){
  TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontSize);
  if(font == nullptr){
    printf("Failed to load font! SDL_Error: %s\n", SDL_GetError());
    return nullptr;
  }

  SDL_Surface *surface = TTF_RenderText_Blended(font, msg.c_str(), color);
  if(surface == nullptr){
    TTF_CloseFont(font);
    printf("Failed to render text to a surface! SDL_Error: %s\n", SDL_GetError());
    return nullptr;
  }

  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surface);
  if(tex == nullptr){
    printf("Unable to render surface to a Texture! SDL_Error: %s\n", SDL_GetError());
  }

  SDL_FreeSurface(surface);
  TTF_CloseFont(font);

  return tex;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w=-1, int h=-1){
  SDL_Rect dest;
  dest.x = x;
  dest.y = y;
  dest.w = w;
  dest.h = h;

  if(w==-1 || h == -1)
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

  SDL_RenderCopy(ren, tex, NULL, &dest);
}
