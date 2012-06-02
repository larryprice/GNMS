#ifndef _CENTITY_HPP_
#define _CENTITY_HPP_

#include <vector>
#include <string>

#include "CAnimate.hpp"
#include "CSurface.hpp"
#include "CArea.hpp"
#include "CCamera.hpp"
#include "CFPS.hpp"

using namespace std;

enum EntityType {
  ENTITY_TYPE_GENERIC = 0,
 
  ENTITY_TYPE_PLAYER,
  ENTITY_TYPE_PUCK
};
 
enum EntityFlag {
  ENTITY_FLAG_NONE     = 0,             // nothing special
 
  ENTITY_FLAG_GRAVITY    = 0x00000001,  // affected by gravity
  ENTITY_FLAG_FRICTION   = 0x00000002,  // affected by friction
  ENTITY_FLAG_GHOST      = 0x00000004,  // go through walls
  ENTITY_FLAG_MAPONLY    = 0x00000008   // only collide with map
};

class CEntity {
public:
  static vector<CEntity*> entityList;

protected:
  CAnimate animControl;
  SDL_Surface* entSurf;

  float speedX;
  float speedY;
  float accelX;
  float accelY;

  int currentFrameCol;
  int currentFrameRow;

  int colX;
  int colY;
  int colW;
  int colH;

  float colSpeedX;
  float colSpeedY;

  bool colWait;

  bool canJump;

public:
  float x;
  float y;
  int width;
  int height;
  
  bool moveLeft;
  bool moveRight;

  int type;
  bool dead;
  int flags;

  float maxSpeedX;
  float maxSpeedY;

  bool animated;
  Uint8 maxFrames;

  bool stopX;
  bool stopY;

public:
  CEntity();
  CEntity(float x, float y, int colX=0,
	  int colY=0, int colW=0, int colH=0, int type=ENTITY_TYPE_GENERIC,
	  int flags = ENTITY_FLAG_MAPONLY, float maxSpeedX=5,
	  float maxSpeedY=5, bool canJump = false, Uint8 mF=1,
	  bool anim=true);

  virtual ~CEntity();

  virtual bool onLoad(string file, int width, int height, int maxFrames);
  virtual void onLoop();
  virtual void onRender(SDL_Surface* dispSurf);
  virtual void onCleanup();

  virtual void onAnimate();
  virtual bool onCollision(CEntity* ent);

  void onMove(float mX, float mY);
  void stopMove();
  bool onJump();
  bool collides(int oX, int oY, int oW, int oH);

  float getColSpeedX();
  float getColSpeedY();

  void accelerateX(float );
  void accelerateY(float );

  void setStopX();
  void setStopY();

  float getX();
  float getY();
  void setX(float nx);
  void setY(float ny);
private:
  bool posValid(int nX, int nY);
  bool posValidTile(CTile* tile);
  bool posValidEntity(CEntity* ent, int nX, int nY);

};

class CEntityCol {
public:
  static std::vector< CEntityCol> entColList;

  CEntity* entA;
  CEntity* entB;

public:
  CEntityCol();
};

#endif
