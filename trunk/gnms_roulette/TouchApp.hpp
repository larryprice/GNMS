#ifndef _TOUCH_APP_HPP_
#define _TOUCH_APP_HPP_

#include <TUIO/TuioListener.h>
#include <TUIO/TuioClient.h>
#include <map>
#include "GnmsCursor.hpp"
#include "roulette.hpp"

using namespace std;
using namespace TUIO;

class TouchApp : TuioListener {
public:
  TouchApp();
  ~TouchApp();
  GnmsCursor* getMostRecentCursor();
  static TouchApp* sApp;
  void ageMostRecentCursor();
  bool getAge();
private:
  int mru;
  bool aged;
  map< int, GnmsCursor*> gnmsCursors;
  TuioClient* tuioClient;

// stuff inherited from TuioListener
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
