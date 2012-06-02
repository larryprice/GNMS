#include "CApp.hpp"


void CApp::OnLoop(){
  // check for winner
  if (win == false && waitForReset == false) {
    if (Grid[0] != GRID_TYPE_NONE && 
	Grid[0] == Grid[1] && Grid[1] == Grid[2]) {
      win = true;
      waitForReset = true;
    } else if (Grid[3] != GRID_TYPE_NONE && 
	       Grid[3] == Grid[4] && Grid[4] == Grid[5]) {
      win = true;
      waitForReset = true;
    } else if (Grid[6] != GRID_TYPE_NONE && 
	       Grid[6] == Grid[7] && Grid[7] == Grid[8]) {
      win = true;
      waitForReset = true;
    } else if (Grid[0] != GRID_TYPE_NONE && 
	       Grid[0] == Grid[3] && Grid[3] == Grid[6]) {
      win = true;
      waitForReset = true;
    } else if (Grid[1] != GRID_TYPE_NONE && 
	       Grid[1] == Grid[4] && Grid[4] == Grid[7]) {
      win = true;
      waitForReset = true;
    } else if (Grid[2] != GRID_TYPE_NONE && 
	       Grid[2] == Grid[5] && Grid[5] == Grid[8]) {
      win = true;
      waitForReset = true;
    } else if (Grid[0] != GRID_TYPE_NONE && 
	       Grid[0] == Grid[4] && Grid[4] == Grid[8]) {
      win = true;
      waitForReset = true;
    } else if (Grid[2] != GRID_TYPE_NONE && 
	       Grid[2] == Grid[4] && Grid[4] == Grid[6]) {
      win = true;
      waitForReset = true;
    } else if (Grid[0] != GRID_TYPE_NONE && Grid[1] != GRID_TYPE_NONE &&
	       Grid[2] != GRID_TYPE_NONE && Grid[3] != GRID_TYPE_NONE &&
	       Grid[4] != GRID_TYPE_NONE && Grid[5] != GRID_TYPE_NONE &&
	       Grid[6] != GRID_TYPE_NONE && Grid[7] != GRID_TYPE_NONE &&
	       Grid[2] != GRID_TYPE_NONE) {
      // tie game, no one wins
      Reset();
      CurrentPlayer = 0;
    }
  }

  if (waitForReset == true) {
    if (CurrentPlayer == 0) {
      p2Score++;
    } else if (CurrentPlayer == 1) {
      p1Score++;
    }
    waitForReset = false;
  }

  // touch stuff
  tuioClient->lockCursorList();
  map< int, GnmsCursor*>::iterator it;
  for (it=gnmsCursors.begin(); it!=gnmsCursors.end(); it++) {
    GnmsCursor* cur = it->second;
    if (cur->isLive() == false && cur->isDead() == false) {
      float x, y;
      bool dir;
      if (cur->singleTouch(x, y) == true) {
	onSingleTouch(x, y);
      }

      cur->killCursor();
    }
  }
  tuioClient->unlockCursorList();
}
