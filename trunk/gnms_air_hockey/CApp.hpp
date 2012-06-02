#ifndef _CAPP_HPP_
#define _CAPP_HPP_
 
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <TUIO/TuioListener.h>
#include <TUIO/TuioClient.h>
#include "CSurface.hpp"
#include "CEvent.hpp"
#include "CAnimate.hpp"
#include "CEntity.hpp"
#include "Define.hpp"
#include "CArea.hpp"
#include "CCamera.hpp"
#include "CPlayer.hpp"
#include "CPuck.hpp"
#include "GnmsCursor.hpp"
#include <string>
#include <map>
#include <sstream>

using namespace std;
using namespace TUIO;
 
class CApp : CEvent, public TuioListener {
  bool isRunning;
  
  SDL_Surface* dispSurf;
  SDL_Surface* tableSurf;
  SDL_Surface* msgSurf;
  SDL_Surface* grabPaddlesSurf;
  SDL_Surface* p1ScoreSurf;
  SDL_Surface* p2ScoreSurf;
  SDL_Surface* helpSurf;

  CPlayer* p1;
  CPlayer* p2;
  CPuck* puck;

  bool paused;
  bool reset;
  bool help;

  int p1CursorId;
  int p2CursorId;

  unsigned short p1Score;
  unsigned short oldP1Score;
  unsigned short p2Score;
  unsigned short oldP2Score;

  TuioClient* tuioClient;
  map< int, GnmsCursor*> gnmsCursors;

  Mix_Chunk *colNoise;
  Mix_Chunk *wallNoise;
  Mix_Chunk *goalNoise;
  Mix_Chunk *winNoise;
	
public:
  CApp();
  int onExecute();

public:
  bool onInit();
  void onEvent(SDL_Event* Event);
  void onLoop();
  void onRender();
  void onCleanup();
  void onExit();
  void Reset();
  void onSingleTouch(float x, float y);


private:
  void onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
  void onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

public: // TuioListener stuff
  void addTuioObject(TuioObject *tobj);
  void updateTuioObject(TuioObject *tobj);
  void removeTuioObject(TuioObject *tobj);

  void addTuioCursor(TuioCursor *tcur);
  void updateTuioCursor(TuioCursor *tcur);
  void removeTuioCursor(TuioCursor *tcur);

  void refresh(TuioTime frameTime);
};
 
#endif
