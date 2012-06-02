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
  bool clearScreen;

  SDL_Surface* dispSurf;
  SDL_Surface* bgSurf;

  map< int, GnmsCursor*> gnmsCursors;
  map< int, CEntity*> brushes;
  vector< string> colorChoices;
  unsigned short color;

  Mix_Music *song;
  unsigned short songNo;
  vector< string> musicChoices;
  bool musicPaused;

  TuioClient* tuioClient;

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
  void onLButtonDown(int x, int y);
  void onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
  void onSingleTouch(float x, float y);

  bool getColors();
  bool getMusic();
  void nextSong();

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

public:
  static bool songEnded;
};

#endif
