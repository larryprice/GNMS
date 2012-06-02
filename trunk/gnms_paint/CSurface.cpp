#include "CSurface.hpp"
 
CSurface::CSurface() {

}
 
SDL_Surface* CSurface::onLoad(std::string filename) {
    SDL_Surface* tempSurf = NULL;
    SDL_Surface* retSurf = NULL;
 
    if((tempSurf = IMG_Load(filename.c_str())) == NULL) {
        return NULL;
    }
    retSurf = SDL_DisplayFormatAlpha(tempSurf);
    SDL_FreeSurface(tempSurf);
 
    return retSurf;
}

bool CSurface::onDraw(SDL_Surface* dest, 
        				SDL_Surface* src, int x, int y) {
    if(dest == NULL || src == NULL) {
        return false;
    }
 
    SDL_Rect destRect;
 
    destRect.x = x;
    destRect.y = y;
 
    SDL_BlitSurface(src, NULL, dest, &destRect);
 
    return true;
}

bool CSurface::onDraw(SDL_Surface* dest, 
        				SDL_Surface* src, int x, int y,
        				int x2, int y2, int w, int h) {
	if(dest == NULL || src == NULL) {
        return false;
    }
 
    SDL_Rect destRect;
 
    destRect.x = x;
    destRect.y = y;
 
    SDL_Rect srcRect;
 
    srcRect.x = x2;
    srcRect.y = y2;
    srcRect.w = w;
    srcRect.h = h;
 
    SDL_BlitSurface(src, &srcRect, dest, &destRect);
 
    return true;
}

bool CSurface::Transparent(SDL_Surface* dest, int R, int G, int B) {
	if(dest == NULL) {
        return false;
    }

    SDL_SetColorKey(dest, SDL_SRCCOLORKEY | SDL_RLEACCEL,
    					 SDL_MapRGB(dest->format, R, G, B));

    return true;
}




