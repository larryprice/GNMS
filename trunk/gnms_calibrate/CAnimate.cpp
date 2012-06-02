#include "CAnimate.hpp"

CAnimate::CAnimate() {
  currentFrame = 0;
  maxFrames = 0;
  frameInc = 1;

  frameRate = 100; // ms
  oldTime = 0;

  oscillate = false;
}

CAnimate::~CAnimate() {
}

void CAnimate::onAnimate() {
  if (oldTime + frameRate <= SDL_GetTicks()) {

    oldTime = SDL_GetTicks();

    currentFrame += frameInc;

    if (oscillate) {
      if (frameInc > 0) {
	if (currentFrame >= maxFrames-1) {
	  frameInc = -frameInc;
	}
      } else if (currentFrame <= 0) {
	  frameInc = -frameInc;
      }
    } else if (currentFrame >= maxFrames-1) {
	currentFrame = 0;
    }
  }

  return;
}

void CAnimate::setFrameRate(int rate) {
  frameRate = rate;

  return;
}

void CAnimate::setCurrentFrame(int frame) {
  if ((frame < 0) || (frame >= maxFrames)) {
    return;
  } else {
    currentFrame = frame;
  }

  return;
}

int CAnimate::getCurrentFrame() {
  return currentFrame;
}
