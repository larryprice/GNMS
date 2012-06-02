#ifndef _CAREA_HPP_
#define _CAREA_HPP_
 
#include "CMap.hpp"
#include <string>
#include <vector>
 
class CArea {
public:
  static CArea areaControl;
 
public:
  std::vector< CMap> mapList;
 
private:
  int areaSize; // assuming square areas for now
 
  SDL_Surface* tileSet;
 
public:
  CArea();
 
  bool onLoad(std::string filename); 
  void onRender(SDL_Surface* dispSurf, int x, int y);
  void onCleanup();
  CMap* getMap(int x, int y);
  CTile* getTile(int x, int y);
};
 
#endif
