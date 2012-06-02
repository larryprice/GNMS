#ifndef _CPLAYER_HPP_
#define _CPLAYER_HPP_

#include "CEntity.hpp"
#include <string>

class CPlayer : public CEntity {
public:
  Uint8 maxFrames;
public:
  CPlayer();
  CPlayer(float x, float y, int cX=0, int cY=0, int cW=0, int cH=0,
	  int t=ENTITY_TYPE_PLAYER, int f=ENTITY_FLAG_GRAVITY,
	  float maxSX=15, float maxSY=15, Uint8 mF=1, bool anim=true);
  bool onLoad(std::string filename, int w, int h, int maxFrames);
  void onLoop();
  void onRender(SDL_Surface* dispSurf);
  void onCleanup();
  void onAnimate();
  void setSpeedX(float x);
  void setSpeedY(float y);
  virtual bool onCollision(CEntity* ent);
};

#endif
