#ifndef _CPUCK_HPP_
#define _CPUCK_HPP_

#include "CPlayer.hpp"

class CPuck : public CPlayer {
public:
  CPuck();
  CPuck(float x, float y, int cX=0, int cY=0, int cW=0, int cH=0,
	int t=ENTITY_TYPE_PLAYER, int f=ENTITY_FLAG_FRICTION,
	float maxSX=20, float maxSY=20, Uint8 mF=1, bool anim=false);
  bool onCollision(CEntity* ent);

  void transferSpeed(float , float);

};

#endif
