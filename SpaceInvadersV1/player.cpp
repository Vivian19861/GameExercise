#include "player.hpp"
#include "spaceInvader.hpp"

Player::Player(int x, int y)
:velocity{8.0},
laserVelocity{4},
laserFired{false},
isDestroyed{false}
{
  shape.x = x;
  shape.y = y;
  shape.w = playerWidth;
  shape.h = playerHeight;
}

void Player::moveLeft(){
  shape.x -= velocity;
  if( shape.x < 0 ){
    shape.x = 0;
  }
}

void Player::moveRight(){
  shape.x += velocity;
  if( shape.x+shape.w > windowWidth){
    shape.x = windowWidth - shape.w;
  }
}

void Player::fireLaser(){
  laserFired = true;
  LaserShot tempLaser{shape.x+25, shape.y-10};
  tempLaser.velocity = this->laserVelocity;
  lasers.push_back(tempLaser);
}

void Player::updateShots(){
  for(auto& laser:lasers){
    laser.shape.y -= laser.velocity;
  }
}

bool Player::didLaserHit(SDL_Rect temp){
  for(auto it = lasers.begin(); it != lasers.end();){
    if(SDL_HasIntersection(&it->shape, &temp)){
      it = lasers.erase(it);
      if(lasers.size() == 0){
        laserFired = false;
      }
      return true;
    } else {
      ++it;
    }
  }
  return false;
}

SDL_Rect Player::getShape(){
  return shape;
}

void Player::setIsDestroyed(bool isDestroyed){
  this->isDestroyed = isDestroyed;
}

std::vector<LaserShot> Player::getLasers(){
  return lasers;
}
