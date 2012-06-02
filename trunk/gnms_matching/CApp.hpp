#ifndef _CAPP_HPP_
#define _CAPP_HPP_
 
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "CSurface.hpp"
#include "CEvent.hpp"
#include "CAnimate.hpp"
#include "CEntity.hpp"
#include "Define.hpp"
#include "GnmsCursor.hpp"
#include <ctime>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <algorithm>
#include <TUIO/TuioListener.h>
#include <TUIO/TuioClient.h>

using namespace std;
using namespace TUIO;

class CApp : CEvent, TuioListener {
  bool isRunning;

  SDL_Surface* dispSurf;
  SDL_Surface* bgSurf;

  map< int, GnmsCursor*> gnmsCursors;
  map< unsigned short, CEntity*> matchTiles;

  vector< unsigned short> tilesVisible;
  short guess1;
  short guess2;

  bool reset;

  Mix_Music *song;

  TuioClient* tuioClient;

public:
  CApp();

  int onExecute();
  bool onInit();
  void onEvent(SDL_Event* Event);
  void onLoop();
  void onRender();
  void onCleanup();
  void onExit();

private:
  void onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
  void onSingleTouch(float x, float y);

  bool getTiles();

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
