#include "CArea.hpp"
 
#include <iostream>
using namespace std;

CArea CArea::areaControl;
 
CArea::CArea() {
  areaSize = 0;
}
 
bool CArea::onLoad(std::string filename) {
  mapList.clear();
 
  FILE* fp = fopen(filename.c_str(), "r");
 
  if(fp == NULL) {
    return false;
  }
 
  char tileBuf[255];
 
  fscanf(fp, "%s\n", tileBuf);
 
  tileSet = CSurface::onLoad(string(CALIBRATE_DIR)
			     +string(tileBuf));

  if(tileSet == NULL) {
    fclose(fp);
    cout << tileBuf << " not valid!" << endl;
    return false;
  }
 
  fscanf(fp, "%d\n", &areaSize);
 
  for(int x=0; x<areaSize; x++) {
    for(int y=0; y<areaSize; y++) {
      char mapBuf[255];
 
      fscanf(fp, "%s ", mapBuf);
 
      CMap tempMap;
      if(tempMap.onLoad(string(CALIBRATE_DIR)
			+string(mapBuf)) == false) {
	fclose(fp);
 
	return false;
      }
 
      tempMap.tileSet = tileSet;
 
      mapList.push_back(tempMap);
    }
    fscanf(fp, "\n");
  }
 
  fclose(fp);
 
  return true;
}
 
void CArea::onRender(SDL_Surface* dispSurf, int x, int y) {
  int width  = MAP_WIDTH * TILE_SIZE;
  int height = MAP_HEIGHT * TILE_SIZE;
 
  int firstID = -x / width;
  firstID = firstID + ((-y / height) * areaSize);
 
  for(int i=0; i<4; i++) {
    int ID = firstID + ((i/2) * areaSize) + (i%2);
 
    if(ID < 0 || ID >= mapList.size()) {
      continue;
    }
 
    int rendX = ((ID % areaSize) * width) + x;
    int rendY = ((ID / areaSize) * height) + y;
 
    mapList[ID].onRender(dispSurf, rendX, rendY);
  }
}

CMap* CArea::getMap(int x, int y) {
  int id = x / (MAP_WIDTH * TILE_SIZE) 
    + (y / (MAP_HEIGHT * TILE_SIZE) * areaSize);

  if (id < 0 || id >= mapList.size()) {
    return NULL;
  }

  return &mapList[id];
}

CTile* CArea::getTile(int x, int y) {
  CMap* theMap = getMap(x, y);

  if (theMap == NULL) {
    return NULL;
  } else {
    x = x % (MAP_WIDTH * TILE_SIZE);
    y = y % (MAP_HEIGHT * TILE_SIZE);

    return theMap->getTile(x, y);
  }
}
 
void CArea::onCleanup() {
  if(tileSet != NULL) {
    SDL_FreeSurface(tileSet);
  }
 
  mapList.clear();
}
