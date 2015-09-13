/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef tile_h
#define tile_h

#include "sizes.h"
#include "sprite_sheets.h"

class Tile{
    private:
    public:
    Tile();

    void prepare();

    void animate();

    void render();

    //The coordinates of the tile.
    double x,y;

    double angle;

    double rotate_speed;

    bool rotate_direction;

    //If true, the tile is solid.
    //If false, the tile is not solid.
    bool solid;

    short sprite;
};

#endif
