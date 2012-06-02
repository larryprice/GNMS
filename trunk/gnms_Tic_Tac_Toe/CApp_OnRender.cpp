#include "CApp.hpp"

void CApp::OnRender() {
  int X,Y,i;

  if (CurrentPlayer == 0) {
    if (win == true) {
      CSurface::OnDraw(Surf_Display, Surf_GridOW, 0, 0);
    } else {
      CSurface::OnDraw(Surf_Display, Surf_GridX, 0, 0);
    }
  } else {
    if (win == true) {
      CSurface::OnDraw(Surf_Display, Surf_GridXW, 0, 0);
    } else {
      CSurface::OnDraw(Surf_Display, Surf_GridO, 0, 0);
    }
  }

  for (i = 0; i < 9; i++) {
    X = (i % 3) * MARK_WIDTH;
    Y = (i / 3) * MARK_HEIGHT;

    if (Grid[i] == GRID_TYPE_X) {
      CSurface::OnDraw(Surf_Display, Surf_X, X, Y);
    } else if (Grid[i] == GRID_TYPE_O) {
      CSurface::OnDraw(Surf_Display, Surf_O, X, Y);
    }
  }

  stringstream scoreStr;
  if (p1Score < 10 && p1Score != oldP1Score) {
    oldP1Score = p1Score;
    SDL_FreeSurface(p1ScoreSurf);
    p1ScoreSurf = NULL;
    scoreStr << TTT_DIR << "/images/score_" << p1Score << ".png";
    p1ScoreSurf = CSurface::OnLoad(scoreStr.str());
  }
  scoreStr.str("");
  if (p2Score < 10 && p2Score != oldP2Score) {
    SDL_FreeSurface(p2ScoreSurf);
    oldP2Score = p2Score;
    p2ScoreSurf = NULL;
    scoreStr << TTT_DIR << "/images/score_" << p2Score << ".png";
    p2ScoreSurf = CSurface::OnLoad(scoreStr.str());
  }

  CSurface::OnDraw(Surf_Display, p1ScoreSurf, 985, 100);
  CSurface::OnDraw(Surf_Display, p2ScoreSurf, 1188, 100);

  SDL_Flip(Surf_Display);

  return;
}
