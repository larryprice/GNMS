#ifndef _CAPP_HPP_
#define _CAPP_HPP_
 
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "CSurface.hpp"
#include "CEvent.hpp"
#include "CAnimate.hpp"
#include "CEntity.hpp"
#include "Define.hpp"
#include "CArea.hpp"
#include "CCamera.hpp"
#include "CPlayer.hpp"
#include "GnmsCursor.hpp"
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <TUIO/TuioListener.h>
#include <TUIO/TuioClient.h>

using namespace std;
using namespace TUIO;

class CApp : CEvent, TuioListener {
  bool isRunning;

  SDL_Surface* dispSurf;
  SDL_Surface* bgSurf;
  SDL_Surface* msgSurf;

  vector< SDL_Surface*> calibraters;
  bool cal0;
  bool cal1;
  bool cal2;
  bool cal3;
  bool cal4;

  Mix_Chunk* bell0;
  Mix_Chunk* bell1;
  Mix_Chunk* bell2;
  Mix_Chunk* bell3;
  Mix_Chunk* bell4;
 
  TuioClient* tuioClient;

  map< int, GnmsCursor*> gnmsCursors;

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

private:
  void onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
  void onTouch(float x, float y);
  void onSingleTouch(float x, float y);

  // below is the TuioListener stuff we inherited and need
  // to define.
public:
  void addTuioObject(TuioObject *tobj);
  void updateTuioObject(TuioObject *tobj);
  void removeTuioObject(TuioObject *tobj);

  void addTuioCursor(TuioCursor *tcur);
  void updateTuioCursor(TuioCursor *tcur);
  void removeTuioCursor(TuioCursor *tcur);

  void refresh(TuioTime frameTime);

};

#endif
