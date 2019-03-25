#include <stdlib.h>

#include "alien.hpp"
#include "spaceInvader.hpp"

Alien::Alien(int x, int y)
:velocity{2.0},
shotVelocity{3},
shotFired{false},
isDestroyed{false}
{
  shape.x = x;
  shape.y = y;
  shape.w = alienWidth;
  shape.h = alienHeight;
}

void Alien::moveRight(){
  shape.x += velocity;
}

void Alien::moveLeft(){
  shape.x -= velocity;
}

void Alien::moveDown(){
  shape.y += 5;
}

void Alien::fireShot(){
  Shot tempS(this->shape.x, this->shape.y);
  tempS.velocity = this->shotVelocity;
  shots.push_back(tempS);
  shotFired = true;
}

void Alien::updateShots(){
  for(auto& shot:shots){
    shot.shape.y += shot.velocity;
  }
}

bool Alien::didShotHit(SDL_Rect temp){
  for(auto it = shots.begin(); it != shots.end();){
    if(SDL_HasIntersection(&it->shape, &temp)){
      it = shots.erase(it);
      if(shots.size() == 0){
        shotFired = false;
      }
      return true;
    } else {
      ++it;
    }
  }
  return false;
}

SDL_Rect Alien::getShape(){
  return shape;
}

bool Alien::getShotFired(){
  return shotFired;
}

void Alien::setIsDestroyed(bool isDestroyed){
  this->isDestroyed = isDestroyed;
}

bool Alien::getIsDestroyed(){
  return isDestroyed;
}

std::vector<Shot> Alien::getShots(){
  return shots;
}
