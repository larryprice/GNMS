#include "CApp.hpp"

void helperNextSong();

bool CApp::songEnded = false;

CApp::CApp() {
  isRunning = true;
  clearScreen = true;
  dispSurf  = NULL;
  bgSurf  = NULL;

  gnmsCursors.clear();
  brushes.clear();
  colorChoices.clear();
  color = 0;

  song = NULL;
  songNo = 0;
  musicPaused = false;
  musicChoices.clear();

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
    cerr << "Failed to initialize SDL" << endl;
    return false;
  }

  #if GNMS_PAINT_NO_MOUSE == 1
  SDL_ShowCursor(SDL_DISABLE);
  #endif

  tuioClient = new TuioClient(TUIO_PORT);
  tuioClient->addTuioListener(this);
  tuioClient->connect();
  if (tuioClient->isConnected() == false) {
    cerr << "Failed to start TUIO" << endl;
    return false;
  }

  SDL_WM_SetCaption("GNMS Paint", NULL);

  #if GNMS_PAINT_NO_FULLSCREEN == 1
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

  bgSurf = CSurface::onLoad(string(PAINT_DIR)+
  			      string("/images/gnms_paint.png"));

  if (bgSurf == NULL) {
    cerr << "Failed to load background" << endl;
    return false;
  }

  if (getColors() == false) {
    cerr << "Didn't find any brushes" << endl;
    return false;
  }

  if (getMusic() == false) {
    cerr << "Didn't find any music" << endl;
    return false;
  }

  if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096)) {
    cerr << "Unable to open audio" << endl;
    return false;
  }

  song = Mix_LoadMUS(musicChoices[songNo].c_str());
  if (song == NULL) {
    cerr << musicChoices[songNo].c_str() << endl;
    return false;
  }
  Mix_HookMusicFinished(helperNextSong);
  songNo++;
  Mix_PlayMusic(song, 0);

  if(CArea::areaControl.onLoad(string(PAINT_DIR)+
			       string("/map/1.area")) == false) {
    cout << "area not loaded properly!" << endl;
    return false;
  }

  SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

  return true;
}

void CApp::onEvent(SDL_Event* Event) {
  CEvent::onEvent(Event);
}

void CApp::onLoop() {
  CFPS::FPSControl.onLoop();

  CEntityCol::entColList.clear();

  if (songEnded == true) {
    this->nextSong();
    songEnded = false;
  }

  map< int, GnmsCursor*>::iterator it;
  tuioClient->lockCursorList(); // lock the cursor list  

  // check for single touches
  for (it=gnmsCursors.begin(); it!=gnmsCursors.end(); it++) {
    GnmsCursor* cur = it->second;
    if (cur->isLive() == true && cur->isDead() == false) {
      float x, y;
      cur->getCurrentCoordinates(x, y);
      if (brushes.count(it->first) == 1 &&
	  brushes[it->first] != NULL) {
	brushes[it->first]->setX(x);
	brushes[it->first]->setY(y);
      } else {
	brushes[it->first] = new CEntity(x, y);
	brushes[it->first]->
	  onLoad(colorChoices[color], BRUSH_WIDTH, BRUSH_HEIGHT, 1);
	// if we haven't exhausted all colors
	if (color < colorChoices.size()-1) {
	  color++; // go to next color
	} else { // else start over
	  color = 0;
	}
      }
    } else if (cur->isLive() == false && cur->isDead() == false) {
      float x, y;
      if (cur->singleTouch(x, y) == true) {
	onSingleTouch(x, y);
      }
      if (brushes.count(it->first) == 1) {
	brushes[it->first] = NULL;
      }
      cur->killCursor();     // cursor has been serviced
    }
  }
  tuioClient->unlockCursorList(); // unlock cursor list

  return;
}

void CApp::onSingleTouch(float x, float y) {
  if ((x > 860 && y > 850) && (x < 1010 && y < 920)) {
    onExit(); // EXIT
  } else if ((x > 550 && x < 770) && (y > 840 && y < 910)) {
    // music stuff
    if (x < 612) { // prev
      // decrement song number, change track
      Mix_HaltMusic();
      if (songNo > 1) {
	Mix_FreeMusic(song);
	song = NULL;
	song = Mix_LoadMUS(musicChoices[songNo-2].c_str());
	songNo--;
      } else if (songNo == 1) {
	Mix_FreeMusic(song);
	song = NULL;
	song = Mix_LoadMUS(musicChoices.back().c_str());
	songNo = 0;
      } else {
	if (musicChoices.size() > 1) {
	  Mix_FreeMusic(song);
	  song = NULL;
	  song = Mix_LoadMUS(musicChoices[musicChoices.size()-2].c_str());
	  songNo = musicChoices.size()-1;
	}
      }
      Mix_PlayMusic(song, 0);
    } else if (x < 660) {
      // stop playback
      musicPaused = true;
      Mix_HaltMusic();
    } else if (x < 880 && musicPaused == true) {
      // restart playback
      musicPaused = false;
      Mix_PlayMusic(song, 0);
    } else { // next song
      // increment song number, change track
      Mix_HaltMusic();
      song = Mix_LoadMUS(musicChoices[songNo].c_str());
      Mix_PlayMusic(song, 0);
      if (songNo < musicChoices.size()-1) {
	songNo++;
      } else {
	songNo = 0;
      }
    }
    }else if ((x > 370 && x < 450) && (y > 820 && y < 940)) {
	clearScreen = true; // X CLEAR
  }

  return;
}

void CApp::onRender() {
  if (clearScreen == true) {
    SDL_FillRect(dispSurf, NULL, 0xFFFFFF);
    CArea::areaControl.onRender(dispSurf, -CCamera::cameraControl.getX(),
			      -CCamera::cameraControl.getY());
    CSurface::onDraw(dispSurf, bgSurf, 0, 0);
    clearScreen = false;
  }

  map< int, CEntity*>::iterator it;
  for (it=brushes.begin(); it!=brushes.end(); it++) {
    if (it->second != NULL) {
      it->second->onRender(dispSurf);
    }
  }

  SDL_Flip(dispSurf);
  return;
}

/* @TODO: Delete */
void CApp::onLButtonDown(int x, int y) {


  return;
}

void CApp::onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
  switch(sym) {
  case SDLK_ESCAPE: {
    onExit();
    break;
  }
  case SDLK_f: {
    // toggle fullscreen
    SDL_WM_ToggleFullScreen(dispSurf);
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

  if (song != NULL) {
    Mix_FreeMusic(song);
    song = NULL;
  }

  tuioClient->disconnect();
  delete tuioClient;

  map< int, CEntity*>::iterator brushIt;
  for (brushIt=brushes.begin(); brushIt!=brushes.end(); brushIt++) {
    if (brushIt->second != NULL) {
      delete brushIt->second;
    }
  }

  SDL_FreeSurface(bgSurf);
  SDL_FreeSurface(dispSurf);

  Mix_CloseAudio();
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

bool CApp::getColors() {
  DIR* dp;
  struct dirent* dirp;
  string dirStr = string(PAINT_DIR)+string("/images/");
  if ((dp = opendir(dirStr.c_str())) == NULL) {
    cerr << "Error opening " << dirStr << endl;
    return false;
  }

  while ((dirp = readdir(dp)) != NULL) {
    string temp = dirp->d_name;
    if (temp.find("brush") != string::npos) {
      colorChoices.push_back(string(PAINT_DIR)+
			     string("/images/")+string(dirp->d_name));
    }
  }
  closedir(dp);

  if (colorChoices.empty() == true) {
    return false;
  }

  return true;
}

bool CApp::getMusic() {
  DIR* dp;
  struct dirent* dirp;
  string dirStr = string(PAINT_DIR)+string("/sounds/");
  if ((dp = opendir(dirStr.c_str())) == NULL) {
    cerr << "Error opening " << dirStr << endl;
    return false;
  }

  while ((dirp = readdir(dp)) != NULL) {
    string temp = dirp->d_name;
    if (temp.find("music") != string::npos) {
      musicChoices.push_back(string(PAINT_DIR)+
			     string("/sounds/")+string(dirp->d_name));
    }
  }
  closedir(dp);

  if (musicChoices.empty() == true) {
    return false;
  }

  return true;
}

void CApp::nextSong() {
  Mix_HaltMusic();
  song = Mix_LoadMUS(musicChoices[songNo].c_str());
  if (songNo < musicChoices.size()-1) {
    songNo++;
  } else {
    songNo = 0;
  }
  Mix_PlayMusic(song, 0);
}

void helperNextSong() {
  CApp::songEnded = true;
}
