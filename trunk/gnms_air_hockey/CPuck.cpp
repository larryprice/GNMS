#include "CPuck.hpp"

#include <iostream>
using namespace std;

CPuck::CPuck() : CPlayer() {
}

CPuck::CPuck(float x, float y, int cX, int cY, int cW, int cH,
	     int t, int f, float maxSX, float maxSY, Uint8 mF,
	     bool anim)
  : CPlayer(x, y, cX, cY, cW, cH, t, f, maxSX, maxSY, mF, anim) {
}

bool CPuck::onCollision(CEntity* ent) {
  speedX += 3 * ent->getColSpeedX() / 5;
  speedY += 3 * ent->getColSpeedY() / 5;

  stopX = true;
  stopY = true;

  return true;
}
