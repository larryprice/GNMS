#include "CPlayer.hpp"

#include <iostream>
using namespace std;

CPlayer::CPlayer() : CEntity() {
  type = ENTITY_TYPE_PLAYER;
  flags = ENTITY_FLAG_GRAVITY;
}

CPlayer::CPlayer(float x, float y, int cX, int cY, int cW, int cH,
		 int t, int f, float maxSX, float maxSY, Uint8 mF,
		 bool anim) :
  CEntity(x, y, cX, cY, cW, cH, t, f, maxSX, maxSY, mF, anim) {
}

bool CPlayer::onLoad(std::string filename, int w, int h, int maxFrames) {
  if (CEntity::onLoad(filename, w, h, maxFrames) == false) {
    return false;
  } else {
    return true;
  }
}

void CPlayer::onLoop() {
  CEntity::onLoop();
  return;
}

void CPlayer::onRender(SDL_Surface* dispSurf) {
  CEntity::onRender(dispSurf);  
  return;
}

void CPlayer::onCleanup() {
  CEntity::onCleanup();
  return;
}

void CPlayer::onAnimate() {
  if (speedX != 0) {
    animControl.maxFrames = maxFrames;
  } else {
    animControl.maxFrames = 0;
  }

  CEntity::onAnimate();

  return;
}

bool CPlayer::onCollision(CEntity* ent) {
  //cout << speedX << " " << speedY << endl;
  colWait = false;
  return true;
}

void CPlayer::setSpeedX(float x) {
  this->speedX = x;
}

void CPlayer::setSpeedY(float y) {
  this->speedY = y;
}
