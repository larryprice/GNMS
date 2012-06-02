#ifndef _CAPP_HPP_
#define _CAPP_HPP_

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "CSurface.hpp"
#include "CEvent.hpp"
#include "Define.hpp"
#include "GnmsCursor.hpp"
#include <TUIO/TuioListener.h>
#include <TUIO/TuioClient.h>
#include <map>
#include <sstream>

using namespace TUIO;
using namespace std;

class CApp : public CEvent, public TuioListener {
private:
  bool Running;

  SDL_Surface* Surf_Display;
private:
  SDL_Surface* Surf_GridX;
  SDL_Surface* Surf_GridO;
  SDL_Surface* Surf_GridXW;
  SDL_Surface* Surf_GridOW;
  SDL_Surface* Surf_X;
  SDL_Surface* Surf_O;

  SDL_Surface* p1ScoreSurf;
  SDL_Surface* p2ScoreSurf;
  unsigned short p1Score;
  unsigned short oldP1Score;
  unsigned short oldP2Score;
  unsigned short p2Score;
  bool waitForReset;
private:
  TuioClient* tuioClient;
  map< int, GnmsCursor*> gnmsCursors;
  Mix_Music* song;

  bool swipeI;
  bool swipeD;

  int prevSquare;

  int Grid[9];
  enum {
    GRID_TYPE_NONE = 0,
    GRID_TYPE_X,
    GRID_TYPE_O
  };
  int CurrentPlayer;
  bool win;
public:
  CApp();
  int OnExecute();
public:
  bool OnInit();
  void OnEvent(SDL_Event* Event);
  void OnLoop();
  void OnRender();
  void OnCleanup();
  void OnExit();
  void Reset();
  void SetCell(int ID, int Type);
  void OnLButtonDown(int mX, int mY);
  void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

  void onSingleTouch(float x, float y);
  void setMark(int sq);

  int checkSameSquare(float x, float y);
  int checkValidSquare(float x, float y);

  // Stuff we inherited from TuioListener
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
