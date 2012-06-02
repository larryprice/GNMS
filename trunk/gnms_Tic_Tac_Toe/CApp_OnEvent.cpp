#include "CApp.hpp"

void CApp::OnEvent(SDL_Event* Event) {
  CEvent::OnEvent(Event);
}

void CApp::OnExit() {
  Running = false;
}

void CApp::OnLButtonDown(int mX, int mY) {
  int ID;

  ID = mX / MARK_WIDTH;
  ID = ID + ((mY / MARK_HEIGHT) * 3);

  if(Grid[ID] != GRID_TYPE_NONE) {
    return;
  } else if (win == true) {
    return;
  }

  /*if(CurrentPlayer == 0) {
    SetCell(ID, GRID_TYPE_X);
    CurrentPlayer = 1;
  } else {
    SetCell(ID, GRID_TYPE_O);
    CurrentPlayer = 0;
    }*/

  return;
}

void CApp::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) {
  switch(sym) {
  case SDLK_ESCAPE: {
    OnExit();
    break;
  }
  case SDLK_f: {
    // toggle fullscreen
    SDL_WM_ToggleFullScreen(Surf_Display);
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
