#ifndef _CSURFACE_HPP_
#define _CSURFACE_HPP_
 
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>
 
class CSurface {
public:
  CSurface();
 
public:
  static SDL_Surface* onLoad(std::string filename);
  static bool onDraw(SDL_Surface* dest, 
		     SDL_Surface* src, int x, int y);
  static bool onDraw(SDL_Surface* dest,
		     SDL_Surface* src, int x, int y,
		     int x2, int y2, int w, int h);
  static bool Transparent(SDL_Surface* dest,
			  int R, int G, int B);
};
 
#endif
