#include "CCamera.hpp"
 
CCamera CCamera::cameraControl;
 
CCamera::CCamera() {
  x = 0;
  y = 0;
 
  targetX = NULL;
  targetY = NULL;
 
  targetMode = TARGET_MODE_NORMAL;
}
 
void CCamera::onMove(int mX, int mY) {
  x += mX;
  y += mY;
}
 
int CCamera::getX() {
  if(targetX != NULL) {
    if(targetMode == TARGET_MODE_CENTER) {
      return *targetX - (WWIDTH / 2);
    }
 
    return *targetX;
  }
 
  return x;
}
 
int CCamera::getY() {
  if (targetY != NULL) {
    if(targetMode == TARGET_MODE_CENTER) {
      return *targetY - (WHEIGHT / 2);
    }
 
    return *targetY;
  }
 
  return y;
}
 
void CCamera::setPos(int nX, int nY) {
  this->x = nX;
  this->y = nY;
}
 
void CCamera::setTarget(float* nX, float* nY) {
   targetX = nX;
   targetY = nY;
 }
