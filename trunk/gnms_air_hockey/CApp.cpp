#include "CApp.hpp"
#include <iostream>

CApp::CApp() {
  isRunning = true;
  dispSurf  = NULL;
  tableSurf = NULL;
  msgSurf = NULL;
  grabPaddlesSurf = NULL;
  p1ScoreSurf = NULL;
  p2ScoreSurf = NULL;
  helpSurf = false;

  p1 = new CPlayer(200, 450, 0, 0, 0, 0, ENTITY_TYPE_PLAYER, ENTITY_FLAG_FRICTION, 40, 40, 1, false);
  p2 = new CPlayer(1000, 450, 0, 0, 0, 0, ENTITY_TYPE_PLAYER, ENTITY_FLAG_FRICTION, 40, 40, 1, false);

  puck = new CPuck(WWIDTH/2-PUCK_SIZE/2, WHEIGHT/2-PUCK_SIZE/2,
		   0, 0, 0, 0, ENTITY_TYPE_PUCK);

  tuioClient = NULL;

  paused = false;
  reset = true;
  help = false;
  p1CursorId = -1;
  p2CursorId = -1;
  p1Score = 0;
  oldP1Score = -1;
  p2Score = 0;
  oldP2Score = -1;

  colNoise = NULL;
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
    cerr << "Failed to init SDL" << endl;
    return false;
  }

  #if GNMS_AIR_HOCKEY_NO_FULLSCREEN == 1
  dispSurf =  SDL_SetVideoMode(WWIDTH, WHEIGHT, 32, 
			       SDL_SWSURFACE | SDL_DOUBLEBUF);
  #else
  dispSurf =  SDL_SetVideoMode(WWIDTH, WHEIGHT, 32, 
			       SDL_SWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  #endif

  SDL_ShowCursor(SDL_DISABLE);

  if(dispSurf == NULL) {
    cerr << "Failed to set video mode" << endl;
    return false;
  }

  tableSurf = CSurface::onLoad(string(HOCKEY_DIR)+string("/images/air_hockey_table_w_p.png"));
  
  if (tableSurf == NULL) {
    cerr << "Failed to load table image" << endl;
    return false;
  } 
    
  if (p1->onLoad(string(HOCKEY_DIR)+string("/images/gnms_air_hockey_paddle.png"), PADDLE_SIZE, PADDLE_SIZE, 1) == false) {
    cerr << "Failed to load paddle 1" << endl;
    return false;
  }

  if (p2->onLoad(string(HOCKEY_DIR)+string("/images/gnms_air_hockey_paddle.png"), PADDLE_SIZE, PADDLE_SIZE, 1) == false) {
    cerr << "Failed to load paddle 2" << endl;
    return false;
  }

  if (puck->onLoad(string(HOCKEY_DIR)+string("/images/gnms_air_hockey_puck.png"), PUCK_SIZE, PUCK_SIZE, 1) == false) {
    cerr << "Failed to load puck" << endl;
    return false;
  }

  CEntity::entityList.push_back(p1);
  CEntity::entityList.push_back(p2);
  CEntity::entityList.push_back(puck);

  if(CArea::areaControl.onLoad(string(HOCKEY_DIR)+string("/map/1.area")) == false) {
    cerr << "area not loaded properly!" << endl;
    return false;
  }
 
  msgSurf = CSurface::onLoad(string(HOCKEY_DIR)+
			     string("/images/air_hockey_msg_begin.png"));
  if (msgSurf == NULL) {
    cerr << "Failed to load message" << endl;
  }

  p1ScoreSurf = CSurface::onLoad(string(HOCKEY_DIR)+
				 string("/images/score0.png"));
  if (p1ScoreSurf == NULL) {
    cerr << "Failed to load score" << endl;
  }

  p2ScoreSurf = CSurface::onLoad(string(HOCKEY_DIR)+
				 string("/images/score0.png"));
  if (p2ScoreSurf == NULL) {
    cerr << "Failed to load score" << endl;
  }

  helpSurf = CSurface::onLoad(string(HOCKEY_DIR)+
				 string("/images/air_hockey_msg_help.png"));
  if (helpSurf == NULL) {
    cerr << "Failed to load help" << endl;
  }

  SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);

  tuioClient = new TuioClient(TUIO_PORT);
  tuioClient->addTuioListener(this);
  tuioClient->connect();
  if (tuioClient->isConnected() == false) {
    cerr << "Failed to start TUIO" << endl;
    return false;
  }

  if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024)) {
    cerr <<"Unable to open audio!" << endl;
    return false;
  }

  string temp = string(HOCKEY_DIR)+
    string("/sounds/air_hockey_puck_paddle.wav");
  colNoise  = Mix_LoadWAV(temp.c_str());
  temp = string(HOCKEY_DIR)+
    string("/sounds/air_hockey_puck_wall.wav");
  wallNoise = Mix_LoadWAV(temp.c_str());
  temp = string(HOCKEY_DIR)+
    string("/sounds/air_hockey_goal.wav");
  goalNoise = Mix_LoadWAV(temp.c_str());
temp = string(HOCKEY_DIR)+
    string("/sounds/air_hockey_win.wav");
  winNoise = Mix_LoadWAV(temp.c_str());
  if (colNoise == NULL || wallNoise == NULL ||
      goalNoise == NULL || winNoise == NULL) {
    cerr << Mix_GetError() << endl;
    return false;
  }

  return true;
}

void CApp::onEvent(SDL_Event* Event) {
  CEvent::onEvent(Event);
}

void CApp::onLoop() {
  CFPS::FPSControl.onLoop();

  for (int i=0; i<CEntity::entityList.size(); i++) {
    if (CEntity::entityList[i] == NULL) {
      continue;
    } else {
      CEntity::entityList[i]->onLoop();
      // check for score
      if (CEntity::entityList[i]->entityScored() == true) {
	// increment appropriate score
	Mix_PlayChannel(-1, goalNoise, 0);
	if (CEntity::entityList[i]->getX() < WWIDTH/2) {
	  p2Score++;
	  if (p2Score >= WINNING_SCORE) {
	    SDL_FreeSurface(msgSurf);
	    msgSurf = NULL;
	    msgSurf = CSurface::onLoad(string(HOCKEY_DIR)+
				       string("/images/air_hockey_msg_p2_win.png"));
	    Mix_PlayChannel(-1, winNoise, 0);
	    reset = true;
	    p2CursorId = -1;
	    p1CursorId = -1;
	  } else {
	    SDL_FreeSurface(msgSurf);
	    msgSurf = NULL;
	    msgSurf = CSurface::onLoad(string(HOCKEY_DIR)+
				       string("/images/air_hockey_msg_p2_score.png"));
	  }
	} else {
	  p1Score++;
	  if (p1Score >= WINNING_SCORE) {
	    SDL_FreeSurface(msgSurf);
	    msgSurf = NULL;
	    msgSurf = CSurface::onLoad(string(HOCKEY_DIR)+
				       string("/images/air_hockey_msg_p1_win.png"));
	    Mix_PlayChannel(-1, winNoise, 0);
	    reset = true;
	    p1CursorId = -1;
	    p2CursorId = -1;
	  } else {
	    SDL_FreeSurface(msgSurf);
	    msgSurf = NULL;
	    msgSurf = CSurface::onLoad(string(HOCKEY_DIR)+
				       string("/images/air_hockey_msg_p1_score.png"));
	  }
	}
	reset = true;
	if (gnmsCursors.count(p1CursorId) == 1) {
	  gnmsCursors[p1CursorId]->killCursor();
	}
	if (gnmsCursors.count(p2CursorId) == 1) {
	  gnmsCursors[p2CursorId]->killCursor();
	}
	p1CursorId = -1;
	p2CursorId = -1;
	// move p1, p2, paddle back to original locations
	p1->setX(200);
	p1->setY(450);
	p2->setX(1000);
	p2->setY(450);
	puck->setX(WWIDTH/2-PUCK_SIZE/2);
	puck->setY(WHEIGHT/2-PUCK_SIZE/2);
	puck->setSpeedX(0);
	puck->setSpeedY(0);
      }
    }
  }

  // check for puck having hit wall, play sound
  if (puck->getWallHit() == true) {
    Mix_PlayChannel(-1, wallNoise, 0);
  }

  // check collisions
  for (int i=0; i<CEntityCol::entColList.size(); i++) {
    CEntity* a = CEntityCol::entColList[i].entA;
    CEntity* b = CEntityCol::entColList[i].entB;
    
    if ((a == NULL) || (b == NULL)) {
      continue;
    } else if (a->onCollision(b) == true) {
      b->onCollision(a);
      // play collision noise
      Mix_PlayChannel(-1, colNoise, 0);
    }
  }

  stringstream scoreStr;
  if (p1Score != oldP1Score) {
    scoreStr << HOCKEY_DIR << "/images/score" << p1Score << ".png";
    SDL_FreeSurface(p1ScoreSurf);
    p1ScoreSurf = NULL;
    p1ScoreSurf = CSurface::onLoad(scoreStr.str());
    scoreStr.str("");
    oldP1Score = p1Score;
  }
  if (p2Score != oldP2Score) {
    scoreStr << HOCKEY_DIR << "/images/score" << p2Score << ".png";
    SDL_FreeSurface(p2ScoreSurf);
    p2ScoreSurf = NULL;
    p2ScoreSurf = CSurface::onLoad(scoreStr.str());
    oldP2Score = p2Score;
  }

  CEntityCol::entColList.clear();

  // lock cursor list
  tuioClient->lockCursorList();
  // move p1
  if (p1CursorId != -1) {
    float x, y;
    gnmsCursors[p1CursorId]->getCurrentCoordinates(x, y);
    if (x < p1->getX() || x > p1->getX()+PADDLE_SIZE) {
      p1->setSpeedX((x-p1->getX())*CFPS::FPSControl.getSpeedFactor());
    } else {
      p1->setStopX();
    }
    if (y < p1->getY() || y > p1->getY()+PADDLE_SIZE) { 
      p1->setSpeedY((y-p1->getY())*CFPS::FPSControl.getSpeedFactor());
    }  else {
      p1->setStopY();
    }
  } else { // make sure we're not moving
    p1->setSpeedX(0);
    p1->setSpeedY(0);
  } 
  // move p2
  if (p2CursorId != -1) {
    float x, y;
    gnmsCursors[p2CursorId]->getCurrentCoordinates(x, y);
    if (x < p2->getX() || x > p2->getX()+PADDLE_SIZE) {
      p2->setSpeedX((x-p2->getX())*CFPS::FPSControl.getSpeedFactor());
    } else {
      p2->setStopX();
    }
    if (y < p2->getY() || y > p2->getY()+PADDLE_SIZE) { 
      p2->setSpeedY((y-p2->getY())*CFPS::FPSControl.getSpeedFactor());
    }  else {
      p2->setStopY();
    }
  } else { // make sure we're not moving
    p2->setSpeedX(0);
    p2->setSpeedY(0);
  } 

  // for all cursors
  map< int, GnmsCursor*>::iterator it;
  for (it=gnmsCursors.begin(); it!=gnmsCursors.end(); it++) {
    GnmsCursor* cur = it->second;
    if (paused == false) { // not paused
      if ((cur->isLive() == true && cur->isDead() == false) &&
	  (p1CursorId != it->first || p2CursorId != it->first)) { 
	float x, y;
	cur->getInitialCoordinates(x, y);
	if (p1CursorId == -1) {
	  if ((x > p1->getX() && x < p1->getX()+PADDLE_SIZE) &&
	      (y > p1->getY() && y < p1->getY()+PADDLE_SIZE)) {
	    p1CursorId = it->first;
	    if (reset == true) {
	      reset = false;
	      if (p1Score >= WINNING_SCORE || p2Score >= WINNING_SCORE) {
		p1Score = 0;
		p2Score = 0;
	      }
	    }
	  }
	}
	if (p2CursorId == -1) {
	  if ((x > p2->getX() && x < p2->getX()+PADDLE_SIZE) &&
	      (y > p2->getY() && y < p2->getY()+PADDLE_SIZE)) {
	    p2CursorId = it->first;
	    if (reset == true) {
	      reset = false;
	      if (p1Score >= WINNING_SCORE || p2Score >= WINNING_SCORE) {
		p1Score = 0;
		p2Score = 0;
	      }
	    }
	  } // else ignore touch
	}
      }
      else if (cur->isLive() == false && cur->isDead() == false) {
	float x, y;
	if (it->first == p1CursorId) {
	  p1CursorId = -1;
	} else if (it->first == p2CursorId) {
	  p2CursorId = -1;
	} else if (cur->singleTouch(x, y) == true) {
	  onSingleTouch(x, y);
	} else if (cur->drewZ(WWIDTH/3, WHEIGHT/3, WWIDTH/5, WHEIGHT/5) == true) {
	  paused = true;
	  reset = false;
	  SDL_FreeSurface(msgSurf);
	  msgSurf = NULL;
	  msgSurf = CSurface::onLoad(string(HOCKEY_DIR)+
				     string("/images/air_hockey_paused.png"));
	}
	cur->killCursor();
      } // else ignore cursor
    } else { // paused
      if (cur->isLive() == false && cur->isDead() == false) {
	// check for pause menu touches
	float x, y;
	if (cur->singleTouch(x, y) == true) {
	  onSingleTouch(x, y);
	} else if (cur->drewZ(WWIDTH/3, WHEIGHT/3, WWIDTH/5, WHEIGHT/5) == true) {
	  paused = false;
	  help = false;
	  reset = false;
	}
	cur->killCursor();
      }
    }
  }
  tuioClient->unlockCursorList();

    return;
}

void CApp::onSingleTouch(float x, float y) {
  if (paused == false) {
    // check for pause at bottom of screen
    if (x >= WWIDTH/2-40 && x <= WWIDTH/2+40) {
      if (y >= WHEIGHT-30) {
	paused = true;
	reset = false;
	msgSurf = CSurface::onLoad(string(HOCKEY_DIR)+
				     string("/images/air_hockey_paused.png"));
      }
    }
  } else {
    // check for pause locations
    if (x >= WWIDTH/2-40 && x <= WWIDTH/2+40) { // unpause
      if (y >= WHEIGHT-30) {
	paused = false;
	reset = false;
	help = false;
      }
    } else if (x >= WWIDTH/2-115 && x <= WWIDTH/2+123) { // pause menu
      if (y >= WHEIGHT/6+56 && y <= WHEIGHT/6+100) { // help
	help = !help;
	cout << "help" << endl;
      } else if (y >= WHEIGHT/6+106 && y <= WHEIGHT/6+150) { // reset
	cout << "reset" << endl;
	reset = true;
	paused = false;
	help = false;
	msgSurf = CSurface::onLoad(string(HOCKEY_DIR)+string("/images/air_hockey_msg_begin.png"));
      } else if (y >= WHEIGHT/6+160 && y<= WHEIGHT/6+214) { // main menu
	onExit();
      }
    }
  }
}

void CApp::onRender() {
  SDL_FillRect(dispSurf, NULL, 0x000000);

  CArea::areaControl.onRender(dispSurf, -CCamera::cameraControl.getX(),
			      -CCamera::cameraControl.getY());

  CSurface::onDraw(dispSurf, tableSurf, 0, 0);

  for (int i=0; i<CEntity::entityList.size(); i++) {
    if (CEntity::entityList[i] == NULL) {
      continue;
    } else {
      CEntity::entityList[i]->onRender(dispSurf);
    }
  }

  if (p1ScoreSurf != NULL) {
    CSurface::onDraw(dispSurf, p1ScoreSurf, WWIDTH/2-NUM_SIZE, 4);
  }
  if (p2ScoreSurf != NULL) {
    CSurface::onDraw(dispSurf, p2ScoreSurf, WWIDTH/2, 4);
  }

  if (reset == true && msgSurf != NULL) {
    CSurface::onDraw(dispSurf, msgSurf, WWIDTH/2-140, WHEIGHT/3);
  }

  if (paused == true ) {
    CSurface::onDraw(dispSurf, msgSurf, WWIDTH/2-115, WHEIGHT/6);
  }

  if (help == true) {
    CSurface::onDraw(dispSurf, helpSurf, WWIDTH/2-215, 9*WHEIGHT/17);
  }
 
  SDL_Flip(dispSurf);
  return;
}

void CApp::onKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) {
  switch(sym) {
  case SDLK_LEFT: {
    p1->accelerateX(-0.8);
    break;
  }
  case SDLK_RIGHT: {
    p1->accelerateX(0.8);
    break;
  }
  case SDLK_UP: {
    p1->accelerateY(-0.8);
    break;
  }
  case SDLK_DOWN: {
    p1->accelerateY(0.8);
    break;
  }
  case SDLK_f: { 
    // fullscreen
    SDL_WM_ToggleFullScreen(dispSurf);
    break;
  }
  case SDLK_ESCAPE: {
    onExit();
    break;
  }
  default: { // do nothing
  }
  }
  return;
}

void CApp::onKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
  switch(sym) {
  case SDLK_LEFT: {
    p1->setStopX();
    break;
  }
  case SDLK_RIGHT: {
    p1->setStopX();
    break;
  }
  case SDLK_UP: {
    p1->setStopY();
    break;
  }
  case SDLK_DOWN: {
    p1->setStopY();
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
  
  Mix_FreeChunk(colNoise);
  Mix_FreeChunk(wallNoise);
  Mix_FreeChunk(goalNoise);
  Mix_FreeChunk(winNoise);
  Mix_CloseAudio();

  SDL_FreeSurface(helpSurf);
  SDL_FreeSurface(p2ScoreSurf);
  SDL_FreeSurface(p1ScoreSurf);
  SDL_FreeSurface(grabPaddlesSurf);
  SDL_FreeSurface(msgSurf);
  SDL_FreeSurface(tableSurf);
  SDL_FreeSurface(dispSurf);

  tuioClient->disconnect();
  delete tuioClient;
  
  SDL_Quit();
  return;
}

void CApp::onExit() {
  isRunning = false;
}

void CApp::Reset() {
  return;
}

// TuioListener interface
void CApp::addTuioObject(TuioObject *tobj) {
  // do nothing
  return;
}

void CApp::updateTuioObject(TuioObject *tobj) {
  // do nothing
  return;
}

void CApp::removeTuioObject(TuioObject *tobj) {
  // do nothing
  return;
}

void CApp::addTuioCursor(TuioCursor *tcur) {
  // add to map of possible cursors
  //cout << "starting " << tcur->getCursorID() << endl;
  gnmsCursors[tcur->getCursorID()] = 
    new GnmsCursor(tcur->getX()*WWIDTH, tcur->getY()*WHEIGHT);
}

void CApp::updateTuioCursor(TuioCursor *tcur) {
  // update map
  //cout << "updating " << tcur->getCursorID() << endl;
  gnmsCursors[tcur->getCursorID()]->
    addCoordinate(tcur->getX()*WWIDTH, tcur->getY()*WHEIGHT);
}

void CApp::removeTuioCursor(TuioCursor *tcur) {
  // tell the cursor it's been stopped
  //cout << "stopping " << tcur->getCursorID() << endl;
  gnmsCursors[tcur->getCursorID()]->
    stopCursor(tcur->getX()*WWIDTH, tcur->getY()*WHEIGHT);
}

void CApp::refresh(TuioTime frameTime) {
  // do nothing
  return;
}

