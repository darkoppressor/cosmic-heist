/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef sizes_h
#define sizes_h

#include <vector>

struct dimensions{
    double w;
    double h;
};

const int TILE_W=32;
const int TILE_H=32;

const int EXIT_W=128;
const int EXIT_H=128;

const int EXPLOSION_W=23;
const int EXPLOSION_H=23;

const short TILE_COLLISION_SIZE=2;
const short EXIT_COLLISION_SIZE=16;

extern std::vector<dimensions> ship_sizes;

void prepare_ship_sizes();

#endif
