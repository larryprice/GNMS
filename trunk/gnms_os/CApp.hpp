#ifndef _CAPP_HPP_
#define _CAPP_HPP_
 
#include <SDL/SDL.h>
#include "CSurface.hpp"
#include "CEvent.hpp"
#include "CAnimate.hpp"
#include "CEntity.hpp"
#include "Define.hpp"
#include "CArea.hpp"
#include "CCamera.hpp"
#include "CPlayer.hpp"
#include "GnmsApp.hpp"
#include "GnmsCursor.hpp"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <dirent.h>
#include <TUIO/TuioListener.h>
#include <TUIO/TuioClient.h>

using namespace std;
using namespace TUIO;

class CApp : CEvent, TuioListener {
  bool isRunning;

  SDL_Surface* dispSurf;
  SDL_Surface* menuSurf;
  CEntity* USBsymbol;

  map< unsigned short, vector< GnmsApp*> > gnmsApps;
  map< int, GnmsCursor*> gnmsCursors;

  unsigned short curPage;

  TuioClient* tuioClient;

  bool rotated;
  
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
  bool getApps();
  void onAppSelect(GnmsApp* app);
  void modifyInitFile(std::string appName);
  void onRefresh();

  void onLButtonDown(int x, int y);
  void onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
  bool onSingleTouch(float x, float y);

  void onPrevPage();
  void onNextPage();
  void onScreenChange();
  
  void rotate180();

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
