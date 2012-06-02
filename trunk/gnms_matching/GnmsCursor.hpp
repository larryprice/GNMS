#ifndef _GNMS_CURSOR_HPP_
#define _GNMS_CURSOR_HPP_

#include <TUIO/TuioCursor.h>
#include <utility>
#include <vector>

using namespace TUIO;
using namespace std;

class GnmsCursor {
public:
  GnmsCursor(float x, float y);

  void getCurrentCoordinates(float &x, float &y);
  void getInitialCoordinates(float &x, float &y);
  int getNumCoordinates();
  bool isLive(); // implies still being updated
  bool isDead(); // implies already been analyzed
  void stopCursor(float final_x, float final_y);
  void killCursor();
  void addCoordinate(float x, float y);

  bool singleTouch(float &x, float &y);
  bool drewCircle(unsigned int reqWidth, unsigned int reqHeight, 
		  float xFudge=80, float yFudge=40);
  bool drewZ(unsigned int reqWidth, unsigned int reqHeight, 
		  float xFudge=80, float yFudge=40);
  bool swipeUp(unsigned int reqLength, unsigned int w_fudge=0);
  bool swipeDown(unsigned int reqLength, unsigned int w_fudge=0);

  // not implemented
  bool swipeLeft();
  bool swipeRight();

private:
  bool live;
  bool dead;
  vector< pair<float, float> > coordinates;
  unsigned short numHorDirChanges;
  bool xDir;

protected:
  float xMax;
  float xMin;
  float yMax;
  float yMin;

};

#endif
