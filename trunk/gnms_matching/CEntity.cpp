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
		 float maxSpeedX, float maxSpeedY, bool canJump,
		 Uint8 mf, bool anim) {
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
    cerr << "Entity surface not loaded! Filename:" << file << endl;
    return false;
  }

  CSurface::Transparent(entSurf, 255, 0, 255);

  this->width = width;
  this->height = height;

  animControl.maxFrames = maxFrames;

  filename = file;

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

  speedX += accelX * FPS; //CFPS::FPSControl.getSpeedFactor();
  speedY += accelY * FPS; //CFPS::FPSControl.getSpeedFactor();

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
    CSurface::onDraw(dispSurf, entSurf, x, y, 
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

    mX *= FPS; //CFPS::FPSControl.getSpeedFactor();
    mY *= FPS; //CFPS::FPSControl.getSpeedFactor();

    if (mX > 0) {
      nX = FPS; //CFPS::FPSControl.getSpeedFactor();
    } else if (mX < 0) {
      nX = -FPS; //CFPS::FPSControl.getSpeedFactor();
    } // else nX = 0

    if (mY > 0) {
      nY = FPS; //CFPS::FPSControl.getSpeedFactor();
    } else if (mY < 0) {
      nY = -FPS; //CFPS::FPSControl.getSpeedFactor();
    } // else nY = 0

    while (true) {
      if (flags & ENTITY_FLAG_GHOST) { // ghost == no collisions
	x += nX;
	y += nY;
      } else { // otherwise == collisions possible
	x += nX;
	speedX = -speedX;
      }
      
      y += nY;
      if (mY > 0) {
	canJump = true;
      }
      if (this->type == ENTITY_TYPE_PUCK) {
	speedY = -speedY;
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
  if (stopX && (speedX < .2f && speedX > -0.2f)) {
    accelX = 0;
    speedX = 0;
    stopX = false;
  }

  if (stopY && (speedY < 0.2f && speedY > -0.2f)) {
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

void CEntity::onCleanup() {
  if (entSurf != NULL) {
    SDL_FreeSurface(entSurf);
    entSurf = NULL;
  }

  return;
}

float CEntity::getX() {
  return x;
}

float CEntity::getY() {
  return y;
}

void CEntity::setX(float nx) {
  x = nx;
}

void CEntity::setY(float ny) {
  y = ny;
}
