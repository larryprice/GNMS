#ifndef _CFPS_HPP_
#define _CFPS_HPP_
 
#include <SDL/SDL.h>
 
class CFPS {
public:
  static CFPS FPSControl;
 
private:
  int oldTime;
  int lastTime;
 
  float speedFactor;
 
  int numFrames;
  int frames;
 
public:
  CFPS();
 
  void onLoop();
  int getFPS();
  float getSpeedFactor();
};
 
#endif
