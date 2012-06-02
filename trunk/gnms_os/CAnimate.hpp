#ifndef _CANIMATE_HPP_
#define _CANIMATE_HPP_

#include <SDL/SDL.h>

class CAnimate {
private:
  int currentFrame;
  int frameInc;
  int frameRate;
  long oldTime;
  
public:
  int maxFrames;
  bool oscillate;

  CAnimate();
  ~CAnimate();

  void onAnimate();
  void setFrameRate(int rate);
  void setCurrentFrame(int frame);
  int getCurrentFrame();
};

#endif
