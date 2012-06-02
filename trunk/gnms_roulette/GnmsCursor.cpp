#include "GnmsCursor.hpp"

GnmsCursor::GnmsCursor(float x, float y) {
  live = true;
  dead = false;
  pair< float, float> tempPair;
  tempPair.first  = x;
  tempPair.second = y;
  coordinates.push_back(tempPair);

  xMax = x;
  xMin = x;
  yMax = y;
  yMin = y;

  xDir = true;
  numHorDirChanges = 0;
}

void GnmsCursor::getCurrentCoordinates(float &x, float &y) {
  x = coordinates.back().first;
  y = coordinates.back().second;
}

void GnmsCursor::getInitialCoordinates(float &x, float &y) {
  x = coordinates.front().first;
  y = coordinates.front().second;
}

void GnmsCursor::stopCursor(float final_x, float final_y) {
  live = false;

  addCoordinate(final_x, final_y);
}

void GnmsCursor::killCursor() {
  dead = true;
}

bool GnmsCursor::isLive() {
  return live;
}

bool GnmsCursor::isDead() {
  return dead;
}

void GnmsCursor::addCoordinate(float x, float y) {
  pair< float, float> tempPair;
  tempPair.first  = x;
  tempPair.second = y;

  if (coordinates.size() > 0) {
    if ((x-coordinates.back().first) > 0) {
      if (coordinates.size() > 1 && xDir == false) {
	numHorDirChanges++;
      }
      xDir = true;
    } else if ((x-coordinates.back().first) < 0) {
      if (coordinates.size() > 1 && xDir == true) {
	numHorDirChanges++;
      }
      xDir = false;
    }
  }

  coordinates.push_back(tempPair);

  if (x > xMax) {
    xMax = x;
  } else if (x < xMin) {
    xMin = x;
  }
  if (y > yMax) {
    yMax = y;
  } else if (y < yMin) {
    yMin = y;
  }

  return;
}

bool GnmsCursor::singleTouch(float &x, float &y) {
  bool retCode = false;
  if (coordinates.size() <= 5) {
    if (coordinates[0].first == coordinates[1].first) {
      if (coordinates[0].second == coordinates[1].second) {
	retCode = true;
	x = coordinates[0].first;
	y = coordinates[0].second;
      }
    }
  }

  return retCode;
}

bool GnmsCursor::swipeUp(unsigned int reqLength, unsigned int w_fudge) {
  bool retCode = false;

  // did we recieved more than one coordinate?
  if (coordinates.size() > 1) {
    // is the last touch at the apex of the slide?
    if (coordinates.back().second == yMin) {
      // is y difference greater than required length?
      if ((yMax - yMin) >= reqLength) {
	// do xmin and xmax differ significantly?
	if ((xMax - xMin) < w_fudge) {
	  retCode = true;
	}
      }
    }
  }

  return retCode;
}

bool GnmsCursor::swipeDown(unsigned int reqLength, unsigned int w_fudge) {
  bool retCode = false;

  // did we recieved more than one coordinate?
  if (coordinates.size() > 1) {
    // is the last touch at the lower extreme of the slide?
    if (coordinates.back().second == yMax) {
      // is y difference greater than required length?
      if ((yMax - yMin) >= reqLength) {
	// do xmin and xmax differ significantly?
	if ((xMax - xMin) < w_fudge) {
	  retCode = true;
	}
      }
    }
  }

  return retCode;
}

bool GnmsCursor::drewCircle(unsigned int widthReq, unsigned int heightReq, 
			    float xFudge, float yFudge) {
  bool retCode = false;

  if (coordinates.size() > 4) {
    // do we satisfy the width/height constraints?
    if (((xMax - xMin) >= widthReq) && ((yMax - yMin) >= heightReq)) {
      // is the first pair significantly close to the last pair?
      if ((coordinates.front().first <= 
	   (coordinates.back().first + xFudge)) &&
	  (coordinates.front().first >= 
	   (coordinates.back().first - xFudge))) {
	if ((coordinates.front().second <= 
	     (coordinates.back().second + yFudge)) &&
	    (coordinates.front().second >= 
	     (coordinates.back().second - yFudge))) {
	  retCode = true;
	}
      }
    }
  }

  return retCode;
}

bool GnmsCursor::drewZ(unsigned int widthReq, unsigned int heightReq, 
		  float xFudge, float yFudge) {
  bool retCode = false;

  if (coordinates.size() > 4) {
    // do we satisfy the width/height constraints?
    if (((xMax - xMin) >= widthReq) && ((yMax - yMin) >= heightReq)) {
      // is the first pair opposite the last pair?
      if (((coordinates.front().first + widthReq) <= (coordinates.back().first + xFudge)) && ((coordinates.front().first + widthReq) >= (coordinates.back().first - xFudge))) {
	if (((coordinates.front().second + heightReq) <= (coordinates.back().second + yFudge)) && ((coordinates.front().second + heightReq) >= (coordinates.back().second - yFudge))) {
	  if (numHorDirChanges >= 2) {
	    retCode = true;
	  }
	}
      }
    }
  }

  return retCode;
}
