#include "CEntity.hpp"
#include <iostream>
using namespace std;

vector< CEntity*> CEntity::entityList;

CEntity::CEntity() {
  entSurf = NULL;
  x = 0;
  y = 0;

  width = 0;
  height = 0;
  
  moveLeft = false;
  moveRight = false;

  type = ENTITY_TYPE_GENERIC;
  dead = false;
  flags = ENTITY_FLAG_MAPONLY;

  speedX = 0;
  speedY = 0;
  accelX = 0;
  accelY = 0;
  maxSpeedX = 5;
  maxSpeedY = 5;

  canJump = false;
  colWait = false;

  currentFrameCol = 0;
  currentFrameRow = 0;
  colX = 0;
  colY = 0;
  colW = 0;
  colH = 0;

  maxFrames = 1;
  animated = true;
}

CEntity::CEntity(float x, float y, int colX,
		 int colY, int colW, int colH, int type, int flags,
		 float maxSpeedX, float maxSpeedY,
		 Uint8 mf, bool anim, bool canJump) {
  entSurf = NULL;
  this->x = x;
  this->y = y;
  
  width = 0;
  height = 0;
  
  moveLeft = false;
  moveRight = false;

  this->type = type;
  dead = false;
  this->flags = flags;

  speedX = 0;
  speedY = 0;
  accelX = 0;
  accelY = 0;
  this->maxSpeedX = maxSpeedX;
  this->maxSpeedY = maxSpeedY;

  this->canJump = canJump;

  currentFrameCol = 0;
  currentFrameRow = 0;
  this->colX = colX;
  this->colY = colY;
  this->colW = colW;
  this->colH = colH;

  colWait = false;

  maxFrames = mf;
  animated = anim;
}

CEntity::~CEntity() {
}

bool CEntity::onLoad(string file, int width, int height, int maxFrames) {
  entSurf = CSurface::onLoad(file.c_str());

  if (entSurf == NULL) {
    return false;
  }

  CSurface::Transparent(entSurf, 255, 0, 255);

  this->width = width;
  this->height = height;

  animControl.maxFrames = maxFrames;

  return true;
}

void CEntity::accelerateX(float a_x) {
  this->accelX = a_x;
  this->stopX = false;
  return;
}

void CEntity::accelerateY(float a_y) {
  this->accelY = a_y;
  this->stopY = false;
  return;
}

void CEntity::setStopX() {
  stopX = true;
  this->accelX = 0;
}

void CEntity::setStopY() {
  stopY = true;
  this->accelY = 0;
}

void CEntity::onLoop() {
  if (stopX == true || stopY == true) {
    stopMove();
  }

  if (flags & ENTITY_FLAG_GRAVITY) {
    accelY = GRAVITY;
  } else if (flags & ENTITY_FLAG_FRICTION) {
    speedY *= (1 - FRICTION);
    speedX *= (1 - FRICTION);
  }

  speedX += accelX * CFPS::FPSControl.getSpeedFactor();
  speedY += accelY * CFPS::FPSControl.getSpeedFactor();

  if (speedX > maxSpeedX) {
    speedX = maxSpeedX;
  } else if (speedX < -maxSpeedX) {
    speedX = -maxSpeedX;
  }

  if (speedY > maxSpeedY) {
    speedY = maxSpeedY;
  } else if (speedY < -maxSpeedY) {
    speedY = -maxSpeedY;
  }

  onAnimate();
  onMove(speedX, speedY);

  return;
}

void CEntity::onRender(SDL_Surface* dispSurf) {
  if (entSurf != NULL && dispSurf != NULL) {
    CSurface::onDraw(dispSurf, entSurf, x - CCamera::cameraControl.getX(),
		     y - CCamera::cameraControl.getY(), 
                     currentFrameCol * width,
                     (currentFrameRow + animControl.getCurrentFrame()) * height,
		     width, height);
  }

  return;
}

void CEntity::onAnimate() {
  if (animated == true) {
    if (accelX < 0) {
      currentFrameCol = 0;
    } else if (accelX > 0) {
      currentFrameCol = 1;
    }
  } else {
    currentFrameCol = 0;
  }

  animControl.onAnimate();

  return;
}

bool CEntity::onCollision(CEntity* ent) {
  // do nothing
  return false;
}

void CEntity::onMove(float mX, float mY) {
  if (mX != 0 || mY != 0) {
    double nX = 0;
    double nY = 0;

    mX *= CFPS::FPSControl.getSpeedFactor();
    mY *= CFPS::FPSControl.getSpeedFactor();

    if (mX > 0) {
      nX = CFPS::FPSControl.getSpeedFactor();
    } else if (mX < 0) {
      nX = -CFPS::FPSControl.getSpeedFactor();
    } // else nX = 0

    if (mY > 0) {
      nY = CFPS::FPSControl.getSpeedFactor();
    } else if (mY < 0) {
      nY = -CFPS::FPSControl.getSpeedFactor();
    } // else nY = 0

    while (true) {
      if (flags & ENTITY_FLAG_GHOST) { // ghost == no collisions
	posValid((int)(x + nX), (int)(y+nY));

	x += nX;
	y += nY;
      } else { // otherwise == collisions possible
	if (posValid((int)(x + nX), (int)y) == true) {
	  x += nX;
	} else {
	  if (this->type == ENTITY_TYPE_PUCK) {
	    speedX = -speedX;
	  }
	}

	if (posValid((int)x, (int)(y + nY))) {
	  y += nY;
	} else {
	  if (mY > 0) {
	    canJump = true;
	  }
	  if (this->type == ENTITY_TYPE_PUCK) {
	    speedY = -speedY;
	  }
	}
      }

      mX += -nX;
      mY += -nY;

      if (nX > 0 && mX <= 0) {
	nX = 0;
      } else if (nX < 0 && mX >= 0) {
	nX = 0;
      }

      if (nY > 0 && mY <= 0) {
	nY = 0;
      } else if (nY < 0 && mY >= 0) {
	nY = 0;
      }

      if (mX == 0) {
	nX = 0;
      }

      if (mY == 0) {
	nY = 0;
      }

      if ((mX == 0 && mY == 0) || (nX == 0 && nY == 0)) {
	break;
      }
    }
  }

  return;
}

void CEntity::stopMove() {
  if (stopX && (speedX < .8f && speedX > -0.8f)) {
    accelX = 0;
    speedX = 0;
    stopX = false;
  }

  if (stopY && (speedY < 0.6f && speedY > -0.6f)) {
    accelY = 0;
    speedY = 0;
    stopY = false;
  }

  return;
}

float CEntity::getColSpeedX() {
  return this->colSpeedX;
}

float CEntity::getColSpeedY() {
  return this->colSpeedY;
}

bool CEntity::onJump() {
  if (canJump == true) {
    speedY -= maxSpeedY;
    canJump = false;
    return true;
  } else {
    return false;
  }
}

bool CEntity::collides(int oX, int oY, int oW, int oH) {
  int tX = (int)x + colX;
  int tY = (int)y + colY;

  int top1 = tY;
  int top2 = oY;

  int bottom1 = top1 + height - 1 - colH;
  int bottom2 = oY + oH -1;

  if (bottom1 < top2 || top1 > bottom2) {
    return false;
  }

  int left1 = tX;
  int left2 = oX;

  int right1 = left1 + width - 1 - colW;
  int right2 = oX + oW - 1;

  if (right1 < left2 || left1 > right2) {
    return false;
  }

  return true;
}

bool CEntity::posValid(int nX, int nY) {
  bool ret = true;
  int sX = (nX + colX) / TILE_SIZE;
  int sY = (nY + colY) / TILE_SIZE;
  int eX = ((nX + colX) + width - colW - 1)  / TILE_SIZE;
  int eY = ((nY + colY) + height - colH - 1) / TILE_SIZE;

  for (int i = sY; i <= eY; i++) {
    for (int j = sX; j <= eX; j++) {
      CTile* tile = CArea::areaControl.getTile(j*TILE_SIZE, i*TILE_SIZE);
      
      if (posValidTile(tile) == false) {
	ret = false;
      }
    }
  }

  if (flags & ENTITY_FLAG_MAPONLY) {
    // do nothing
  } else {
    for (int i=0; i<entityList.size(); i++) {
      if (posValidEntity(entityList[i], nX, nY) == false) {
	ret = false;
      }
    }
  }

  return ret;
}

bool CEntity::posValidTile(CTile* tile) {
  if (tile == NULL) {
    return true;
  } else if (tile->typeID == TILE_TYPE_BLOCK) {
    wallHit = true;
    return false;
  } else if (tile->typeID == TILE_TYPE_GOAL) {
    // depends whether we are player or puck
    if (this->type == ENTITY_TYPE_PUCK) {
      scored = true;
    }
    return false;
  } else {
    return true;
  }
}

bool CEntity::posValidEntity(CEntity* ent, int nX, int nY) {
  if (this != ent && ent != NULL && ent->dead == false &&
      ent->flags ^ ENTITY_FLAG_MAPONLY &&
      ent->collides(nX+colX, nY+colY, width-colW-1, height-colH-1) == true) {
    CEntityCol entCol;

    entCol.entA = this;
    entCol.entB = ent;

    CEntityCol::entColList.push_back(entCol);
    if (colWait == false) {
      colSpeedX = this->speedX;
      colSpeedY = this->speedY;
      colWait = true;
    }

    return false;
  } else { // no collision
    return true;
  }
}

void CEntity::onCleanup() {
  if (entSurf != NULL) {
    SDL_FreeSurface(entSurf);
    entSurf = NULL;
  }

  return;
}

int CEntity::getX() {
  return this->x;
}

int CEntity::getY() {
  return this->y;
}

void CEntity::setX(int nx) {
  x = nx;
}

void CEntity::setY(int ny) {
  y = ny;
}

bool CEntity::entityScored() {
  if (scored == true) {
    scored = false;
    return true;
  }
  return false;
}

bool CEntity::getWallHit() {
  bool temp = wallHit;
  wallHit = false;
  return temp;
}

