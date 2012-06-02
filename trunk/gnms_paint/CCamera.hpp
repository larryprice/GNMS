#ifndef _CCAMERA_HPP_
#define _CCAMERA_HPP_
 
#include <SDL/SDL.h>
 
#include "Define.hpp"
 
enum {
  TARGET_MODE_NORMAL = 0,
  TARGET_MODE_CENTER
};
 
class CCamera {
public:
  static CCamera cameraControl;
 
private:
  int x;
  int y;
 
  float* targetX;
  float* targetY;
 
public:
  int targetMode;
 
public:
  CCamera();
 
public:
  void onMove(int mX, int mY);
 
public:
  int getX();
  int getY();
 
public:
  void setPos(int nX, int nY);
 
  void setTarget(float* nX, float* nY);
};
 
#endif
