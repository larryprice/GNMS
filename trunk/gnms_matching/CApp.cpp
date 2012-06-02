#include "CApp.hpp"

CApp::CApp() {
  isRunning = true;
  dispSurf  = NULL;
  bgSurf  = NULL;

  gnmsCursors.clear();
  matchTiles.clear();
  tilesVisible.clear();

  reset = false;

  guess1 = -1;
  guess2 = -1;

  song = NULL;

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

  unsigned int delayGuess = 0;
  bool startDelay = false;

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

    if (guess1 != -1 && guess2 != -1 && startDelay == false) {
      startDelay = true;
      delayGuess = 0;
    } else if (startDelay == true) {
      delayGuess += waitTime;
      if (delayGuess >= 1000) {
	guess1 = -1;
	guess2 = -1;
	startDelay = false;
      }
    }

  }

  onCleanup();

  return 0;
}

bool CApp::onInit() {
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    cerr << "Failed to initialize SDL" << endl;
    return false;
  }

  #if GNMS_MATCH_NO_MOUSE == 1
  SDL_ShowCursor(SDL_DISABLE);
  #endif

  tuioClient = new TuioClient(TUIO_PORT);
  tuioClient->addTuioListener(this);
  tuioClient->connect();
  if (tuioClient->isConnected() == false) {
    cerr << "Failed to start TUIO" << endl;
    return false;
  }

  SDL_WM_SetCaption("GNMS Matching", NULL);

  #if GNMS_MATCH_NO_FULLSCREEN == 1
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

  bgSurf = CSurface::onLoad(GNMS_MATCH_BG);

  if (bgSurf == NULL) {
    cerr << "Failed to load background" << endl;
    return false;
  }

  if (getTiles() == false) {
    cerr << "Didn't find any tiles" << endl;
    return false;
  }

  if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096)) {
    cerr << "Unable to open audio" << endl;
    return false;
  }

  string temp = GNMS_MATCH_JAZZ;
  song = Mix_LoadMUS(temp.c_str());
  if (song == NULL) {
    cerr << Mix_GetError() << endl;
    return false;
  }
  Mix_PlayMusic(song, -1);

  return true;
}

void CApp::onEvent(SDL_Event* Event) {
  CEvent::onEvent(Event);
}

void CApp::onLoop() {
  CEntityCol::entColList.clear();
  
  if (guess1 != -1 && guess2 != -1) {
    if (matchTiles[guess1]->filename == matchTiles[guess2]->filename) {
      tilesVisible.push_back(guess1);
      tilesVisible.push_back(guess2);
    }
  }

  if (guess1 == -1 || guess2 == -1) {
    tuioClient->lockCursorList(); // lock the cursor list  
    map< int, GnmsCursor*>::iterator it;
    // check for single touches
    for (it=gnmsCursors.begin(); it!=gnmsCursors.end(); it++) {
      GnmsCursor* cur = it->second;
      if (cur->isLive() == false && cur->isDead() == false) {
	float x, y;
	if (cur->singleTouch(x, y) == true) {
	  onSingleTouch(x, y);
	}
	cur->killCursor();     // cursor has been serviced
      }
    }
    tuioClient->unlockCursorList(); // unlock cursor list
  }

  return;
}

void CApp::onSingleTouch(float x, float y) {
  map< unsigned short, CEntity*>::iterator it;
  for (it=matchTiles.begin(); it!=matchTiles.end(); it++) {
    if (it->second != NULL) {
      float mx, my;
      mx = it->second->getX();
      my = it->second->getY();
      if ((x > mx && x < mx+TILE_WIDTH) && (y > my && y < my+TILE_HEIGHT)) {
	if (guess1 == -1) {
	  bool validTile = true;
	  for (int i=0; i<tilesVisible.size(); i++) {
	    if (it->first == tilesVisible[i]) {
	      validTile = false;
	    }
	  }
	  if (validTile == true) {
	    guess1 = it->first;
	  }
	} else if (guess2 == -1) {
	  bool validTile = true;
	  for (int i=0; i<tilesVisible.size(); i++) {
	    if (it->first == tilesVisible[i]) {
	      validTile = false;
	    }
	  }
	  if (validTile == true) {
	    guess2 = it->first;
	  }
	}
      }
    }
  }

  if (tilesVisible.size() >= matchTiles.size()) {
    if (y < 200 && x > 250 && x < 900) {
      reset = true;
    }
  }

  if (y > 800 && x > 500 && x < 800) {
    onExit();
  }

  return;
}

void CApp::onRender() {
  SDL_FillRect(dispSurf, NULL, 0xFFFFFF);
  CSurface::onDraw(dispSurf, bgSurf, 0, 0);

  if (tilesVisible.size() >= matchTiles.size()) {
    SDL_FreeSurface(bgSurf);
    bgSurf = NULL;
    bgSurf = CSurface::onLoad(GNMS_MATCH_BG_WIN);
  }

  if (reset == true) {
    tilesVisible.clear();
    map< unsigned short, CEntity*>::iterator it;
    for (it=matchTiles.begin(); it!=matchTiles.end(); it++) {
      if (it->second != NULL) {
	delete it->second;
      }
    }
    matchTiles.clear();
    if (getTiles() == false) {
      cerr << "Could not get new tiles" << endl;
      onExit();
    }
    SDL_FreeSurface(bgSurf);
    bgSurf = NULL;
    bgSurf = CSurface::onLoad(GNMS_MATCH_BG);
    reset = false;
  }

  // display all permanently visible tiles
  for (int i=0; i<tilesVisible.size(); i++) {
    unsigned short tileIndex = tilesVisible[i];
    if (matchTiles.count(tileIndex) == 1 && matchTiles[tileIndex] != NULL) {
      if (tileIndex == guess1) {
	guess1 = -1;
      } else if (tileIndex == guess2) {
	guess2 = -1;
      }
      matchTiles[tileIndex]->onRender(dispSurf);
    }
  }

  // display the new tiles being guessed
  if (guess1 != -1) {
    matchTiles[guess1]->onRender(dispSurf);
  }
  if (guess2 != -1) {
    matchTiles[guess2]->onRender(dispSurf);
  }

  SDL_Flip(dispSurf);
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

bool CApp::getTiles() {
  DIR* dp;
  struct dirent* dirp;
  string dirStr = GNMS_MATCHING_TILES;
  map< unsigned short, string> tileFiles;
  unsigned short tileNo = 0;

  if ((dp = opendir(dirStr.c_str())) == NULL) {
    cerr << "Error opening " << dirStr << endl;
    return false;
  }

  while ((dirp = readdir(dp)) != NULL) {
    string temp = dirp->d_name;
    if (temp.find("tile") != string::npos) {
      tileFiles[tileNo] = GNMS_MATCHING_TILES+string(dirp->d_name);
      tileNo++;
    }
  }
  closedir(dp);

  if (tileFiles.empty() == true) {
    return false;
  }

  float x=100, y=225;
  vector< string> chosenTiles;
  //seed a randomizer
  srand(time(NULL));
  while (chosenTiles.size() < 6) {
    unsigned short nextImage = rand() % tileFiles.size();
    if (tileFiles.count(nextImage) == 1) {
      chosenTiles.push_back(tileFiles[nextImage]);
      tileFiles.erase(nextImage);
    }
  }

  // randomize tiles to use as second half
  vector< string> tempTiles = chosenTiles;
  chosenTiles.insert(chosenTiles.end(), tempTiles.begin(),
		     tempTiles.end());

  random_shuffle(chosenTiles.begin(), chosenTiles.end());

  for (int i=0; i<chosenTiles.size(); i++) {
    matchTiles[i] = new CEntity(x, y);
    matchTiles[i]->onLoad(chosenTiles[i], 
    				    TILE_WIDTH, TILE_HEIGHT, 1);
    x += 275;
    if (x > 1000) {
      x = 100;
      y += 225;
    }
  }
  

  return true;
}
