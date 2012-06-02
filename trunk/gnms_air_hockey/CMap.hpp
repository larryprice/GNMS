#ifndef _CMAP_HPP_
#define _CMAP_HPP_
 
#include <SDL/SDL.h>
#include <vector>
#include <string>
 
#include "CTile.hpp"
#include "CSurface.hpp"
 
class CMap {
public:
  SDL_Surface* tileSet;
 
private:
  std::vector<CTile> tileList;
 
public:
  CMap();
 
public:
  bool onLoad(std::string File); 
  void onRender(SDL_Surface* dispSurf, int X, int Y);
  CTile* getTile(int x, int y);
};
 
#endif
