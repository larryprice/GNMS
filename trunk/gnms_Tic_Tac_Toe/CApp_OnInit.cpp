#include "CApp.hpp"

bool CApp::OnInit() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return false;
  }

  #if GNMS_TTT_NO_FULLSCREEN == 1
  Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, 32,
				  SDL_SWSURFACE | SDL_DOUBLEBUF);
  #else
  Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, 32, SDL_FULLSCREEN |
				  SDL_SWSURFACE | SDL_DOUBLEBUF);
  #endif
  if (Surf_Display == NULL) {
    cerr << "Failed to set video mode" << endl;
    return false;
  }

  #if GNMS_TTT_NO_MOUSE == 1
  SDL_ShowCursor(SDL_DISABLE);
  #endif

  tuioClient = new TuioClient(TUIO_PORT);
  tuioClient->addTuioListener(this);
  tuioClient->connect();
  if (tuioClient->isConnected() == false) {
    cerr << "Failed to load TUIO" << endl;
    return false;
  }

  Surf_GridX = CSurface::OnLoad(string(TTT_DIR)+
			       string("/images/ttt_game_board_x.png"));
  if (Surf_GridX == NULL) {
    cerr << "Failed to load alt board (X turn)" << endl;
    return false;
  }

  Surf_GridO = CSurface::OnLoad(string(TTT_DIR)+
			       string("/images/ttt_game_board_o.png"));
  if (Surf_GridO == NULL) {
    cerr << "Failed to load alt board (O turn)" << endl;
    return false;
  }

  Surf_GridXW = CSurface::OnLoad(string(TTT_DIR)+
			       string("/images/ttt_game_board_x_wins.png"));
  if (Surf_GridXW == NULL) {
    cerr << "Failed to load alt board (X Wins)" << endl;
    return false;
  }

  Surf_GridOW = CSurface::OnLoad(string(TTT_DIR)+
			       string("/images/ttt_game_board_o_wins.png"));
  if (Surf_GridOW == NULL) {
    cerr << "Failed to load alt board (O Wins)" << endl;
    return false;
  }

  Surf_X = CSurface::OnLoad(string(TTT_DIR)+
			    string("/images/x.png"));
  if (Surf_X == NULL) {
    cerr << "Failed to load X" << endl;
    return false;
  }

  Surf_O = CSurface::OnLoad(string(TTT_DIR)+string("/images/o.png"));
  if (Surf_O == NULL) {
    cerr << "Failed to load O" << endl;
    return false;
  }

  p1ScoreSurf = CSurface::OnLoad(string(TTT_DIR)+string("/images/score_0.png"));
  p2ScoreSurf = CSurface::OnLoad(string(TTT_DIR)+string("/images/score_0.png"));

  if (p1ScoreSurf == NULL || p2ScoreSurf == NULL) {
    cerr << "Failed to load score images" << endl;
    return false;
  }

  Reset();

  if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096)) {
    cerr << "Unable to open audio" << endl;
    return false;
  }

  string temp = string(TTT_DIR)+string("/sounds/Artifact.wav");
  song = Mix_LoadMUS(temp.c_str());
  if (song == NULL) {
    cerr << "Failed to load audio file" << endl;
  }
  Mix_PlayMusic(song, -1);

  return true;
}
