#include "CApp.hpp"

void CApp::OnCleanup() {
  tuioClient->disconnect();
  delete tuioClient;

  if (song != NULL) {
    Mix_FreeMusic(song);
    song = NULL;
  }

  Mix_CloseAudio();

  SDL_FreeSurface(Surf_GridX);
  SDL_FreeSurface(Surf_GridXW);
  SDL_FreeSurface(Surf_GridO);
  SDL_FreeSurface(Surf_GridOW);
  SDL_FreeSurface(Surf_X);
  SDL_FreeSurface(Surf_O);
  SDL_FreeSurface(p1ScoreSurf);
  SDL_FreeSurface(p2ScoreSurf);
  SDL_FreeSurface(Surf_Display);

  map< int, GnmsCursor*>::iterator it;
  for (it=gnmsCursors.begin(); it!=gnmsCursors.end(); it++) {
    delete it->second;
  }

  SDL_Quit();

}
