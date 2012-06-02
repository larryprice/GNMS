#include "CApp.hpp"

CApp::CApp() {
  isRunning = true;
  dispSurf  = NULL;
  bgSurf = NULL;

  calibraters.clear();
  cal0 = false;
  cal1 = false;
  cal2 = false;
  cal3 = false;
  cal4 = false;

  bell0 = NULL;
  bell1 = NULL;
  bell2 = NULL;
  bell3 = NULL;
  bell4 = NULL;

  msgSurf = NULL;
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

  unsigned short touchTimeout = 0;

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
  SDL_WM_SetCaption("GNMS Calibrate", NULL);

  #if GNMS_OS_NO_FULLSCREEN == 1
  dispSurf =  SDL_SetVideoMode(WWIDTH, WHEIGHT, 32,
			       SDL_SWSURFACE | SDL_DOUBLEBUF);
  #else
  dispSurf =  SDL_SetVideoMode(WWIDTH, WHEIGHT, 32,
			       SDL_SWSURFACE | SDL_DOUBLEBUF | 
			       SDL_FULLSCREEN);
  #endif

  if(dispSurf == NULL) {
    return false;
  }

  bgSurf = CSurface::onLoad(string(CALIBRATE_DIR)+
			    string("/images/gnms_calibrate.png"));

  if (bgSurf == NULL) {
    return false;
  }

  msgSurf = CSurface::onLoad(string(CALIBRATE_DIR)+
			     string("/images/calibrate_msg.png"));
  if (msgSurf == NULL) {
    cerr << "Could not open help message" << endl;
    return false;
  }

  if(CArea::areaControl.onLoad(string(CALIBRATE_DIR)+
				 string("/map/1.area")) == false) {
    cout << "area not loaded properly!" << endl;
    return false;
  }

  for (int i=0; i<5; i++) {
    calibraters.push_back(CSurface::onLoad(string(CALIBRATE_DIR)+
					   string("/images/calibrator_red.png")));
    if (calibraters[i] == NULL) {
      cerr << "Unable to load calibrater image" << endl;
      return false;
    }
  }

  if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024)) {
    cerr << "Unable to open audio!" << endl;
    return false;
  } 

  string temp = string(CALIBRATE_DIR)+string("/sounds/boxing_bell.wav");
  bell0 = Mix_LoadWAV(temp.c_str());
  temp = string(CALIBRATE_DIR)+string("/sounds/ding_bell.wav");
  bell1 = Mix_LoadWAV(temp.c_str());
  temp = string(CALIBRATE_DIR)+string("/sounds/goblet_bell.wav");
  bell2 = Mix_LoadWAV(temp.c_str());
  temp = string(CALIBRATE_DIR)+string("/sounds/indian_bell.wav");
  bell3 = Mix_LoadWAV(temp.c_str());
  temp = string(CALIBRATE_DIR)+string("/sounds/ship_bell.wav");
  bell4 = Mix_LoadWAV(temp.c_str());

  SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

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
    if (cur->isLive() == true && cur->isDead() == false) {
      float x, y;
      cur->getCurrentCoordinates(x, y);
      onTouch(x, y);
    } else if (cur->isLive() == false && cur->isDead() == false) {
      float x, y;
      if (cur->singleTouch(x, y) == true) {
	onSingleTouch(x, y);
	/*} else if (cur->swipeDown(3*WHEIGHT/5, WWIDTH/10) == true) {
	cout << "detected downward swipe" << endl;
      } else if (cur->swipeUp(3*WHEIGHT/5, WWIDTH/10) == true) {
	cout << "detected upward swipe" << endl;
      } else if (cur->drewCircle(WWIDTH/5, WHEIGHT/4, WWIDTH/8, WHEIGHT/12)
		 == true) {
		 cout << "detected circle" << endl;*/
      }
      cur->killCursor();     // cursor has been serviced
    }
  }
  tuioClient->unlockCursorList(); // unlock cursor list

  if (cal0 == true) {
    SDL_FreeSurface(calibraters[0]);
    calibraters[0] = NULL;
    calibraters[0] = CSurface::onLoad(string(CALIBRATE_DIR)+
				      string("/images/calibrator_green.png"));
  } else {
    SDL_FreeSurface(calibraters[0]);
    calibraters[0] = NULL;
    calibraters[0] = CSurface::onLoad(string(CALIBRATE_DIR)+
				      string("/images/calibrator_red.png"));
  }

  if (cal1 == true) {
    SDL_FreeSurface(calibraters[1]);
    calibraters[1] = NULL;
    calibraters[1] = CSurface::onLoad(string(CALIBRATE_DIR)+
				      string("/images/calibrator_green.png"));
  } else {
    SDL_FreeSurface(calibraters[1]);
    calibraters[1] = NULL;
    calibraters[1] = CSurface::onLoad(string(CALIBRATE_DIR)+
				      string("/images/calibrator_red.png"));
  }

  if (cal2 == true) {
    SDL_FreeSurface(calibraters[2]);
    calibraters[2] = NULL;
    calibraters[2] = CSurface::onLoad(string(CALIBRATE_DIR)+
				      string("/images/calibrator_green.png"));
  } else {
    SDL_FreeSurface(calibraters[2]);
    calibraters[2] = NULL;
    calibraters[2] = CSurface::onLoad(string(CALIBRATE_DIR)+
				      string("/images/calibrator_red.png"));
  }

  if (cal3 == true) {
    SDL_FreeSurface(calibraters[3]);
    calibraters[3] = NULL;
    calibraters[3] = CSurface::onLoad(string(CALIBRATE_DIR)+
				      string("/images/calibrator_green.png"));
  } else {
    SDL_FreeSurface(calibraters[3]);
    calibraters[3] = NULL;
    calibraters[3] = CSurface::onLoad(string(CALIBRATE_DIR)+
				      string("/images/calibrator_red.png"));
  }
  if (cal4 == true) {
    SDL_FreeSurface(calibraters[4]);
    calibraters[4] = NULL;
    calibraters[4] = CSurface::onLoad(string(CALIBRATE_DIR)+
				      string("/images/calibrator_green.png"));
  } else {
    SDL_FreeSurface(calibraters[4]);
    calibraters[4] = NULL;
    calibraters[4] = CSurface::onLoad(string(CALIBRATE_DIR)+
				      string("/images/calibrator_red.png"));
  }

  cal0 = false;
  cal1 = false;
  cal2 = false;
  cal3 = false;
  cal4 = false;

  return;
}

void CApp::onTouch(float x, float y) {
  if (x >= 0 && x <= 130) {
    if (y >= 0 && y <= 130) {
      // touched upper left
      cal0 = true;
      // also play sound
    } else if (y >= WHEIGHT-130 && y <= WHEIGHT) {
      // touched bottom left
      cal1 = true;
    }
  } else if (x >= WWIDTH-130 && x <= WWIDTH) {
    if (y >= 0 && y <= 130) {
      // touched upper left
      cal2 = true;
      // also play sound
    } else if (y >= WHEIGHT-130 && y <= WHEIGHT) {
      cal3 = true;
      // also play sound
    }
  } else if (x >= WWIDTH/2-65 && x <= WWIDTH/2+65) {
    if (y >= WHEIGHT/2-65 && y <= WHEIGHT/2+65) {
      cal4 = true;
      // also play sound
    }
  }

  return;
}

void CApp::onSingleTouch(float x, float y) {
  if (x >= 0 && x <= 130) {
    if (y >= 0 && y <= 130) {
      // touched upper left
      cal0 = true;
      // also play sound
      Mix_PlayChannel(-1, bell0, 0);
    } else if (y >= WHEIGHT-130 && y <= WHEIGHT) {
      // touched bottom left
      cal1 = true;
      Mix_PlayChannel(-1, bell1, 0);
    }
  } else if (x >= WWIDTH-130 && x <= WWIDTH) {
    if (y >= 0 && y <= 130) {
      // touched upper left
      cal2 = true;
      // also play sound
      Mix_PlayChannel(-1, bell2, 0);
    } else if (y >= WHEIGHT-130 && y <= WHEIGHT) {
      cal3 = true;
      // also play sound
      Mix_PlayChannel(-1, bell3, 0);
    }
  } else if (x >= WWIDTH/2-65 && x <= WWIDTH/2+65) {
    if (y >= WHEIGHT/2-65 && y <= WHEIGHT/2+65) {
      cal4 = true;
      // also play sound
      Mix_PlayChannel(-1, bell4, 0);
    }
  }

  if (x >= WWIDTH/2-200 && x <= WWIDTH/2+200) {
    if (y >= WHEIGHT/6-100 && y <= WHEIGHT/6+100) {
      onExit();
    }
  }

  return;
}


void CApp::onRender() {
  SDL_FillRect(dispSurf, NULL, 0x000000);

  CArea::areaControl.onRender(dispSurf, -CCamera::cameraControl.getX(),
			      -CCamera::cameraControl.getY());

  CSurface::onDraw(dispSurf, bgSurf, 0, 0);
  CSurface::onDraw(dispSurf, msgSurf, WWIDTH/2-178, WHEIGHT/6);

  CSurface::onDraw(dispSurf, calibraters[0], 0, 0);
  CSurface::onDraw(dispSurf, calibraters[1], 0, WHEIGHT-130);
  CSurface::onDraw(dispSurf, calibraters[2], WWIDTH-130, 0);
  CSurface::onDraw(dispSurf, calibraters[3], WWIDTH-130, WHEIGHT-130);
  CSurface::onDraw(dispSurf, calibraters[4], WWIDTH/2-65, WHEIGHT/2-65);

  SDL_Flip(dispSurf);
  return;
}

void CApp::onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
  switch(sym) {
  case SDLK_ESCAPE: {
    onExit();
    break;
  }
  case SDLK_n: {
    break;
  }
  case SDLK_p: {
    break;
  }
  case SDLK_f: {
    // toggle fullscreen
    SDL_WM_ToggleFullScreen(dispSurf);
    break;
  }
  case SDLK_r: {
    // rotate
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

void CApp::onCleanup() {
  CArea::areaControl.onCleanup();

  for (int i=0; i<CEntity::entityList.size(); i++) {
    if (CEntity::entityList[i] == NULL) {
      continue;
    } else {
      CEntity::entityList[i]->onCleanup();
    }
  }

  CEntity::entityList.clear();

  for (int i=0; i<calibraters.size(); i++) {
    if (calibraters[i] != NULL) {
      calibraters[i] = NULL;
    }
  }

  calibraters.clear();

  SDL_FreeSurface(dispSurf);
  SDL_FreeSurface(msgSurf);

  Mix_FreeChunk(bell0);
  Mix_FreeChunk(bell1);
  Mix_FreeChunk(bell2);
  Mix_FreeChunk(bell3);
  Mix_FreeChunk(bell4);
  Mix_CloseAudio();

  tuioClient->disconnect();
  delete tuioClient;

  SDL_Quit();
  return;
}

void CApp::onExit() {
  isRunning = false;
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

