#include "CApp.hpp"

CApp::CApp() {
  isRunning = true;
  dispSurf  = NULL;
  menuSurf = NULL;
  USBsymbol = NULL;
  curPage = 0;

  gnmsApps.clear();
  gnmsCursors.clear();

  tuioClient = NULL;
}

int CApp::onExecute() {
  if(onInit() == false) {
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

  // polling loop... i know it's old fashioned,
  // but it's so easy to implement
  while(isRunning == true) {
    // get events, react accordingly
    while(SDL_PollEvent(&Event)) {
      onEvent(&Event);
    }

    delayTime = waitTime - (SDL_GetTicks() - frameStartTime);

    onLoop();

    if(delayTime > 0) {
      SDL_Delay((Uint32)delayTime);
    }
    frameStartTime = SDL_GetTicks();

    onRender();

  }

  onCleanup();

  return 0;
}

bool CApp::onInit() {
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return false;
  }

  #if GNMS_OS_NO_MOUSE == 1
  SDL_ShowCursor(SDL_DISABLE);
  #endif

  tuioClient = new TuioClient(TUIO_PORT);
  tuioClient->addTuioListener(this);
  tuioClient->connect();
  if (tuioClient->isConnected() == false) {
    cerr << "Failed to start TUIO" << endl;
    return false;
  }
  SDL_WM_SetCaption("GNMS OS", NULL);

  #if GNMS_OS_NO_FULLSCREEN == 1
  dispSurf =  SDL_SetVideoMode(WWIDTH, WHEIGHT, 32,
			       SDL_SWSURFACE | SDL_DOUBLEBUF);
  #else
  dispSurf =  SDL_SetVideoMode(WWIDTH, WHEIGHT, 32,
			       SDL_SWSURFACE | SDL_DOUBLEBUF | 
			       SDL_FULLSCREEN);
  #endif

  if(dispSurf == NULL) {
    cerr << "Failed to set video mode" << endl;
    return false;
  }

  menuSurf = CSurface::onLoad(string(OS_DIR)+"/images/gnms_menu.png");

  if (menuSurf == NULL) {
    cerr << "Failed to load menu image" << endl;
    return false;
  }

  if(CArea::areaControl.onLoad(string(OS_DIR)+"/map/1.area") == false) {
    cerr << "area not loaded properly!" << endl;
    return false;
  }

  SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

  if (getApps() == false) {
    cerr << "Failed to load apps" << endl;
    return false;
  }
  curPage = 0;
  onScreenChange();

  USBsymbol = new CEntity(WWIDTH-USB_WIDTH-10, WHEIGHT-USB_HEIGHT-10);
  if (USBsymbol->onLoad(string(OS_DIR)+"/images/usb-logo.png", 
			USB_WIDTH, USB_HEIGHT, 1) == false) {
    cerr << "Failed to load USB symbol" << endl;
  }

  return true;
}

void CApp::onEvent(SDL_Event* Event) {
  CEvent::onEvent(Event);
}

void CApp::onLoop() {
  CFPS::FPSControl.onLoop();

  CEntityCol::entColList.clear();

  map< int, GnmsCursor*>::iterator it;
  tuioClient->lockCursorList(); // lock the cursor list
  for (it=gnmsCursors.begin(); it!=gnmsCursors.end(); it++) {
    GnmsCursor* cur = it->second;
    if (it->second != NULL) {
      if (cur->isLive() == false && cur->isDead() == false) {
	float x, y;
	if (cur->singleTouch(x, y) == true) {
	  bool refreshed = onSingleTouch(x, y);
	  if (refreshed == true) {
	    break;
	  }
	} else if (cur->swipeDown(WHEIGHT/5, WWIDTH/8) == true) {
	  onPrevPage();
	} else if (cur->swipeUp(WHEIGHT/5, WWIDTH/8) == true) {
	  onNextPage();
	} else if (cur->drewCircle(WWIDTH/3, WHEIGHT/4, WWIDTH/8, WHEIGHT/12)
		   == true) {
	  rotate180();
	}
	cur->killCursor();     // cursor has been serviced
      }
    }
  }
  tuioClient->unlockCursorList(); // unlock cursor list

  return;
}

bool CApp::onSingleTouch(float x, float y) {
  bool retVal = false;

  vector< GnmsApp*> appList = gnmsApps[curPage];
  vector< GnmsApp*>::iterator it;
  for (it=appList.begin(); it!=appList.end(); it++) {
    if (*it == NULL) {
      // this should be cause for alarm
      // but we're going to ignore it for now
      continue;
    } else {
      float appX = (*it)->getX();
      if (x > appX && x < appX+TILE_WIDTH) {
	float appY = (*it)->getY();
	if (y > appY && y < appY+TILE_HEIGHT) {
	  onAppSelect(*it);
	  retVal = true;
	}
      }
    }
  }

  if (x > USBsymbol->getX() && x < USBsymbol->getX()+USB_WIDTH) {
    if (y > USBsymbol->getY() && y < USBsymbol->getY()+USB_HEIGHT) {
      string temp = GNMS_LOAD_USB;
      system(temp.c_str());
      onRefresh();
      retVal = true;
    }
  }

  if (gnmsApps.size() > (curPage+1)*4 &&
	     (x > 475 && x < 810 && y > 900)) {
    onNextPage();
  }
  if (curPage > 0 &&
      (x > 475 && x < 810 && y > 630 && y < 650)) {
    onPrevPage();
  }

  return retVal;
}

void CApp::onRender() {
  SDL_FillRect(dispSurf, NULL, 0x000000);

  CArea::areaControl.onRender(dispSurf, -CCamera::cameraControl.getX(),
			      -CCamera::cameraControl.getY());

  CSurface::onDraw(dispSurf, menuSurf, 0, 0);

  USBsymbol->onRender(dispSurf);

  vector< GnmsApp*> appList = gnmsApps[curPage];
  vector< GnmsApp*>::iterator it;
  for (it=appList.begin(); it!=appList.end(); it++) {
    if (*it == NULL) {
      continue;
    } else {
      (*it)->onRender(dispSurf);
    }
  }

  SDL_Flip(dispSurf);
  return;
}

/* @TODO: Delete */
void CApp::onLButtonDown(int x, int y) {
  vector< GnmsApp*> appList = gnmsApps[curPage];
  vector< GnmsApp*>::iterator it;
  for (it=appList.begin(); it!=appList.end(); it++) {
    if (*it == NULL) {
      continue;
    } else {
      float appX = (*it)->getX();
      if (x > appX && x < appX+TILE_WIDTH) {
	float appY = (*it)->getY();
	if (y > appY && y < appY+TILE_HEIGHT) {
	  onAppSelect(*it);
	}
      }
    }
  }

  if (x > USBsymbol->getX() && x < USBsymbol->getX()+50) {
    if (y > USBsymbol->getY() && y < USBsymbol->getY()+23) {
      system("../gnms_scripts/gnms_load_app_from_usb.py");
      onRefresh();
    }
  }

  return;
}

void CApp::onRefresh() {
  getApps();
  curPage = 0;
  onScreenChange();
  
  // kill any cursors that may be waiting in the beginning
  map< int, GnmsCursor*>::iterator it;
  for (it=gnmsCursors.begin(); it!=gnmsCursors.end(); it++) {
    if (it->second != NULL) {
      delete it->second;
    }
  }

  gnmsCursors.clear();

  return;
}

void CApp::onAppSelect(GnmsApp* app) {
  // pull us into non-fullscreen mode so the
  // other app can have the full screen buffer
  SDL_SetVideoMode(WWIDTH, WHEIGHT, 32,
  		   SDL_SWSURFACE | SDL_DOUBLEBUF);

  tuioClient->disconnect();
  system(app->getExe().c_str());
  tuioClient->connect();

  // go back into fullscreen mode
  #if GNMS_OS_NO_FULLSCREEN == 0
  SDL_SetVideoMode(WWIDTH, WHEIGHT, 32,
  		   SDL_SWSURFACE | SDL_DOUBLEBUF |
  		   SDL_FULLSCREEN);
  #endif

  // move app to top of init file
  modifyInitFile(app->getName());

  // reinitialize the menu
  onRefresh();

  return;
}

void CApp::modifyInitFile(string appName) {
  ifstream initFileIn;
  string temp = GNMS_INIT_FILE;
  initFileIn.open(temp.c_str());

  stringstream tempStream;
  tempStream << appName << endl;
  if (initFileIn.is_open()) {
    while (initFileIn.good()) {
      string tempStr;
      getline(initFileIn, tempStr);
      if (tempStr != "" && tempStr != appName) {
	tempStream << tempStr << endl;
      }
    }
    initFileIn.close();
  } else {
    cerr << "Failed to open init file for input" << endl;
  }

  ofstream initFileOut;
  initFileOut.open(temp.c_str());
  if (initFileOut.is_open()) {
    initFileOut << tempStream.str();
    initFileOut.close();
  } else {
    cerr << "Failed to open init file for output" << endl;
  }

  return;
}

void CApp::onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
  switch(sym) {
  case SDLK_ESCAPE: {
    onExit();
    break;
  }
  case SDLK_n: {
    onNextPage();
    break;
  }
  case SDLK_p: {
    onPrevPage();
    break;
  }
  case SDLK_f: {
    // toggle fullscreen
    SDL_WM_ToggleFullScreen(dispSurf);
    break;
  }
  case SDLK_r: {
    // rotate
    rotate180();
    break;
  }
  case SDLK_c: {
    // toggle cursor
    if (SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE) {
      SDL_ShowCursor(SDL_ENABLE);
    } else {
      SDL_ShowCursor(SDL_DISABLE);
    }
    break;
  }
  default: { // do nothing
  }
  }
  return;
}

void CApp::onNextPage() {
  if (gnmsApps.size() > 1 && curPage < gnmsApps.size()-1) {
    curPage++;

    // change screen
    onScreenChange();
  }

  return;
}

void CApp::onPrevPage() {
  if (curPage > 0) {
    curPage--;

    // change screen
    onScreenChange();
  }

  return;
}

void CApp::onScreenChange() {
  if (gnmsApps.size() <= 1) {
    // normal screen
    SDL_FreeSurface(menuSurf);
    menuSurf = NULL;
    menuSurf = CSurface::onLoad(string(OS_DIR)+"/images/gnms_menu.png");
  } else if (curPage == 0) {
    // ellipses on bottom only
    SDL_FreeSurface(menuSurf);
    menuSurf = NULL;
    menuSurf = CSurface::onLoad(string(OS_DIR)+"/images/gnms_menu_b.png");
  } else if (curPage == gnmsApps.size()-1) {
    // ellipses on top only
    SDL_FreeSurface(menuSurf);
    menuSurf = NULL;
    menuSurf = CSurface::onLoad(string(OS_DIR)+"/images/gnms_menu_t.png");
  } else {
    // ellipses on both
    SDL_FreeSurface(menuSurf);
    menuSurf = NULL;
    menuSurf = CSurface::onLoad(string(OS_DIR)+"/images/gnms_menu_tb.png");
  }

  return;
}

void CApp::onCleanup() {
  CArea::areaControl.onCleanup();

  for (int i=0; i<CEntity::entityList.size(); i++) {
    if (CEntity::entityList[i] == NULL) {
      continue;
    } else {
      CEntity::entityList[i]->onCleanup();
    }
  }

  map< unsigned short, vector< GnmsApp*> >::iterator it;
  for (it=gnmsApps.begin(); it!=gnmsApps.end(); it++) {
    for (int i=0; i<it->second.size(); i++) {
      if (it->second[i] != NULL) {
	delete it->second[i];
      }
    }
  }
  gnmsApps.clear();

  CEntity::entityList.clear();

  tuioClient->disconnect();
  delete tuioClient;

  gnmsCursors.clear();

  SDL_Quit();
  return;
}

void CApp::onExit() {
  isRunning = false;
}

bool CApp::getApps() {
  DIR *dp;
  struct dirent *dirp;
  vector< string> dir_files;
  string temp = string(OS_DIR)+"/apps/bin/";
  if((dp = opendir(temp.c_str())) == NULL) {
    cerr << "Error opening apps/bin/" << endl;
    return false;
  }
  while ((dirp = readdir(dp)) != NULL) {
    if (string(dirp->d_name) != ".."
	&& string(dirp->d_name) != "."
	&& string(dirp->d_name) != ".svn") {
      dir_files.push_back(string(dirp->d_name));
    }
  }
  closedir(dp);

  if (dir_files.empty()) { // just return if dir empty
    cerr << "no apps found" << endl;
    return false;
  }

  map< unsigned short, vector< GnmsApp*> >::iterator appIt;
  for (appIt=gnmsApps.begin(); appIt!=gnmsApps.end(); appIt++) {
    for (int i=0; i<appIt->second.size(); i++) {
      if (appIt->second[i] != NULL) {
	delete appIt->second[i];
      }
    }
  }
  gnmsApps.clear();

  unsigned short pageNo = 0;
  int x = 171;
  int y = 360;
  
  temp = GNMS_INIT_FILE;
  FILE* fp = fopen(temp.c_str(), "r");

  if(fp != NULL) {
    char tileBuf[255];
    // for all lines in init file
    while (fscanf(fp, "%s\n", tileBuf) == 1) {
      vector< string>::iterator it;
      bool foundIt = false;
      // for file found in dir
      for (it=dir_files.begin(); it!=dir_files.end(); it++) {
	if (*it == string(tileBuf)) { // if buf exists in dir
	  foundIt = true;
          // post tile to next location
	  string tileImage = string(OS_DIR)+"/apps/tiles/" + *it + ".png";
	  GnmsApp* nextApp = new GnmsApp(x, y, *it);
	  if (nextApp->onLoad(tileImage, TILE_WIDTH, TILE_HEIGHT, 1) == false) {
	    // load default image
	    if (nextApp->onLoad(GNMS_DEFAULT_TILE, TILE_WIDTH, TILE_HEIGHT, 1) == false) {
	      cerr << "Failed to load default tile!" << endl;
	      return false;
	    }
	  }
	  string appStr = string(OS_DIR)+"/apps/bin/" + *it;
	  nextApp->setExe(appStr);
	  gnmsApps[pageNo].push_back(nextApp);
          // increment location
          x += 631;
	  y-=2;
          if (x > WWIDTH) {
            x = 171;
            if (y > 400) { // next page
              pageNo++;
              y = 360;
            } else {       // next row
              y += 292;
            }
          }
          it = dir_files.erase(it);
          break;
	}
      }
    }
  }

  fclose(fp);

  // for files in dir_files
  vector< string>::iterator it;
  // for file found in dir
  for (it=dir_files.begin(); it!=dir_files.end(); it++) {
    // post tile to next location
    string tileImage = string(OS_DIR)+"/apps/tiles/" + *it + ".png";
    GnmsApp* nextApp = new GnmsApp(x, y, *it);
    if (nextApp->onLoad(tileImage, TILE_WIDTH, TILE_HEIGHT, 1) == false) {
      // try loading default image
      if (nextApp->onLoad(GNMS_DEFAULT_TILE, TILE_WIDTH, TILE_HEIGHT, 1) == false) {
	cerr << "Failed to load default tile!" << endl;
	return false;
      }
    }
    string appStr = string(OS_DIR)+"/apps/bin/" + *it;
    nextApp->setExe(appStr);
    gnmsApps[pageNo].push_back(nextApp);
    // increment location
    x += 631;
    y -= 2;
    if (x > WWIDTH) {
      x = 170;
      if (y > 400) { // next page
	pageNo++;
	y = 360;
      } else {       // next row
	y += 292;
      }
    }
  }

  return true;
}

void CApp::addTuioObject(TuioObject *tobj) {
  // empty
}

void CApp::updateTuioObject(TuioObject *tobj) {
  // empty
}

void CApp::removeTuioObject(TuioObject *tobj) {
  // empty
}

void CApp::addTuioCursor(TuioCursor *tcur) {
  // only track the first cursor on the screen
  gnmsCursors[tcur->getCursorID()] = 
    new GnmsCursor(tcur->getX()*WWIDTH, tcur->getY()*WHEIGHT);
}

void CApp::updateTuioCursor(TuioCursor *tcur) {
  // watch the update to see if it's up/down
  if (gnmsCursors.count(tcur->getCursorID()) == 1) {
  gnmsCursors[tcur->getCursorID()]->
    addCoordinate(tcur->getX()*WWIDTH, tcur->getY()*WHEIGHT);
  }
}

void CApp::removeTuioCursor(TuioCursor *tcur) {
  // Tell the cursor it's been stopped
  // and add it's final coordinates
  if (gnmsCursors.count(tcur->getCursorID()) == 1) {
  gnmsCursors[tcur->getCursorID()]->
    stopCursor(tcur->getX()*WWIDTH, tcur->getY()*WHEIGHT);
  }
}

void CApp::refresh(TuioTime frameTime) {
  // empty
}

void CApp::rotate180() {
  if (rotated == false) {
    system("xrandr -o inverted");
    rotated = true;
  } else {
    system("xrandr -o normal");
    rotated = false;
  }
}
