#include "CMap.hpp"

#include <iostream>
using namespace std;
 
CMap::CMap() {
  tileSet = NULL;
}
 
bool CMap::onLoad(std::string filename) {
  tileList.clear();
 
  FILE* fp = fopen(filename.c_str(), "r");
 
  if(fp == NULL) {
    cerr << filename << " not valid!" << endl;
    return false;
  }
 
  for(int y = 0; y < MAP_HEIGHT; y++) {
    for(int x = 0; x < MAP_WIDTH; x++) {
      CTile tempTile;
 
      fscanf(fp, "%d:%d ", &tempTile.tileID, &tempTile.typeID);
 
      tileList.push_back(tempTile);
    }
    fscanf(fp, "\n");
  }
 
  fclose(fp);
 
  return true;
}
 
void CMap::onRender(SDL_Surface* dispSurf, int x, int y) {
  if(tileSet == NULL) {
    return;
  }

  int width  = tileSet->w / TILE_SIZE;
  int height = tileSet->h / TILE_SIZE;
 
  int ID = 0;
 
  for(int i=0; i<MAP_HEIGHT; i++) {
    for(int j=0; j<MAP_WIDTH; j++) {
      if(tileList[ID].typeID == TILE_TYPE_NONE) {
	ID++;
	continue; // do nothing
      }
 
      int tX = x + (j * TILE_SIZE);
      int tY = y + (i * TILE_SIZE);
 
      int setX = (tileList[ID].tileID % width) * TILE_SIZE;
      int setY = (tileList[ID].tileID / width) * TILE_SIZE;
 
      CSurface::onDraw(dispSurf, tileSet, tX, tY, setX, setY, TILE_SIZE, TILE_SIZE);
 
      ID++;
    }
  }
}

CTile* CMap::getTile(int x, int y) {
  int id = x / TILE_SIZE + (MAP_WIDTH * (y / TILE_SIZE));

  if (id < 0 || id >= tileList.size()) {
    return NULL;
  } else {
    return &tileList[id];
  }
}
