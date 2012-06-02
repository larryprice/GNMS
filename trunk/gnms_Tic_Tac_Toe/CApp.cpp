#include "CApp.hpp"

CApp::CApp(){
  Surf_GridX = NULL;
  Surf_GridO = NULL;
  Surf_GridXW = NULL;
  Surf_GridOW = NULL;
  Surf_X = NULL;
  Surf_O = NULL;
  Surf_Display = NULL;
  p1ScoreSurf = NULL;
  p2ScoreSurf = NULL;
  p1Score = 0;
  p2Score = 0;
  oldP1Score = -1;
  oldP2Score = -1;

  CurrentPlayer = 0;
  Running = true;
  swipeI = false;
  swipeD = false;
  prevSquare = -1;
  waitForReset = false;

  tuioClient = NULL;
  gnmsCursors.clear();

  win = false;

  song = NULL;
}

int CApp::OnExecute() {
  if(OnInit() == false) {
    return -1;
  }

  SDL_Event Event;

  Uint32 waitTime = 1000.0f/FPS;
  Uint32 frameStartTime = 0;
  Sint32 delayTime;
  Uint32 idleTime = 0;

  // kill any cursors that may be waiting in the beginning
  tuioClient->lockCursorList();
  map< int, GnmsCursor*>::iterator it;
  for (it=gnmsCursors.begin(); it!=gnmsCursors.end(); it++) {
    if (it->second != NULL) {
      it->second->killCursor();
    }
  }
  tuioClient->unlockCursorList();

  while(Running == true) {
    while(SDL_PollEvent(&Event)) {
      OnEvent(&Event);
    }

    delayTime = waitTime - (SDL_GetTicks() - frameStartTime);

    OnLoop();

    if(delayTime > 0) {
      SDL_Delay((Uint32)delayTime);
    }
    frameStartTime = SDL_GetTicks();

    OnRender();
  }

  OnCleanup();
  return 0;
}

void CApp::Reset() {
  for(int i = 0; i < 9; i++) {
    Grid[i] = GRID_TYPE_NONE;
  }
}

void CApp::SetCell(int ID, int Type) {
  if (ID < 0 || ID >= 9) {
    return;
  }
  if (Type < 0 || Type > GRID_TYPE_O) {
    return;
  }

  Grid[ID] = Type;
}

void CApp::addTuioObject(TuioObject *tobj) {
  // do nothing
}

void CApp::updateTuioObject(TuioObject *tobj) {
  // do nothing
}

void CApp::removeTuioObject(TuioObject *tobj) {
  // do nothing
}

void CApp::addTuioCursor(TuioCursor *tcur) {
  // add cursor to map
  gnmsCursors[tcur->getCursorID()] = 
    new GnmsCursor(tcur->getX()*WWIDTH, tcur->getY()*WHEIGHT);
}

void CApp::updateTuioCursor(TuioCursor *tcur) {
  // update map
  if (gnmsCursors.count(tcur->getCursorID()) == 1) {
    gnmsCursors[tcur->getCursorID()]->
      addCoordinate(tcur->getX()*WWIDTH, tcur->getY()*WHEIGHT);
  }
}

void CApp::removeTuioCursor(TuioCursor *tcur) {
  // stop updating cursor
  if (gnmsCursors.count(tcur->getCursorID()) == 1) {
    gnmsCursors[tcur->getCursorID()]->
      stopCursor(tcur->getX()*WWIDTH, tcur->getY()*WHEIGHT);
  }
}

void CApp::refresh(TuioTime frameTime) {
  // do nothing
}
 
void CApp::onSingleTouch(float x, float y) {
  if (x > 620 && x < 630 && y > 465 && y < 470) { 
    // ignore touch
    // presumably this is the garbage touch in
    // the middle of the screen causd by the projector
  } else if(x > 1030 && x < 1200 && y > 600 && y < 680) {
    // clear board, reset scores (RESET)
    Reset();
    win = false;
    p1Score = 0;
    p2Score = 0;
    CurrentPlayer = 0;
  } else if (x > 1050 && x < 1175 && y > 700 && y < 780) {
    // quit (EXIT)
    OnExit();
  } else if (win == true && x > 1000 && x < 1250 && y > 350 && y < 460) {
    // clear board (someone won and tapped the screen)
    Reset();
    win = false;
    CurrentPlayer = 0;
  } else if (x < MARK_WIDTH*3) { // just give them the mark out of laziness
    int ID = x;

    ID /= MARK_WIDTH;
    if (y > 2*WHEIGHT/3) {
      ID += 6;
    } else if (y > WHEIGHT/3) {
      ID += 3;
    }
    
    if(Grid[ID] != GRID_TYPE_NONE) {
      ID = -1;
    } else if (win == true) {
      ID = -1;
    }
    
    if (ID != -1) {
      if (CurrentPlayer == 0) {
	SetCell(ID, GRID_TYPE_X);
	CurrentPlayer = 1;
      } else {
	SetCell(ID, GRID_TYPE_O);
	CurrentPlayer = 0;
      }
    }

  }
}

void CApp::setMark(int sq) {
  if (sq < 0 || sq >= 9) {
    return;
  }

  Grid[sq] = CurrentPlayer+1;

  return;
}

int CApp::checkSameSquare(float x, float y) {
  int ID;

  ID = x / MARK_WIDTH;
  ID = ID + ((y / MARK_HEIGHT) * 3);

  if (ID == prevSquare) {
    return prevSquare;
  } else {
    return -1;
  }
}

int CApp::checkValidSquare(float x, float y) {
  int ID;

  ID = x / MARK_WIDTH;
  ID = ID + ((y / MARK_HEIGHT) * 3);

  if(Grid[ID] == GRID_TYPE_NONE) {
    return ID;
  } else {
    return -1;
  }
}
