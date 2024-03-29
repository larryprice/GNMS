/*
 * CSurface.cpp
 *
 *  Created on: Mar 25, 2012
 *      Author: grp6
 */

#include "CSurface.hpp"

CSurface::CSurface() {
  // TODO Auto-generated constructor stub
}

SDL_Surface* CSurface::OnLoad(std::string File) {
  SDL_Surface* Surf_Temp = NULL;
  SDL_Surface* Surf_Return = NULL;

  if((Surf_Temp = IMG_Load(File.c_str())) == NULL){
    return NULL;
  }

  Surf_Return = SDL_DisplayFormatAlpha(Surf_Temp);
  SDL_FreeSurface(Surf_Temp);

  return Surf_Return;
}

bool CSurface::OnDraw(SDL_Surface* Surf_Dest,
		      SDL_Surface* Surf_Src, int X, int Y) {
  SDL_Rect DestR;

  if(Surf_Dest == NULL || Surf_Src == NULL){
    return false;
  }

  DestR.x = X;
  DestR.y = Y;

  SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);

  return true;
}

bool CSurface::OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H) {
  SDL_Rect DestR, SrcR;

  if(Surf_Dest == NULL || Surf_Src == NULL){
    return false;
  }

  DestR.x = X;
  DestR.y = Y;

  SrcR.x = X2;
  SrcR.y = Y2;
  SrcR.w = W;
  SrcR.h = H;

  SDL_BlitSurface(Surf_Src, &SrcR, Surf_Dest, &DestR);

  return true;
}

bool CSurface::Transparent(SDL_Surface* Surf_Dest, int R, int G, int B) {
  if(Surf_Dest == NULL){
    return false;
  }

  SDL_SetColorKey(Surf_Dest, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(Surf_Dest->format,R,G,B));

  return true;
}
