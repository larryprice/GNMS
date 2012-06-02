#ifndef _CTILE_HPP_
#define _CTILE_HPP_
 
#include "Define.hpp"
 
enum TileType_e {
    TILE_TYPE_NONE = 0,
 
    TILE_TYPE_NORMAL,
    TILE_TYPE_BLOCK,
    TILE_TYPE_GOAL
};

class CTile {
    public:
        int     tileID;
        int     typeID;
 
    public:
        CTile();
};
 
#endif
