/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef level_h
#define level_h

#include "tile.h"
#include "background_layer.h"
#include "menu_background.h"

#include <vector>

#include <SDL.h>

class Level{
    private:
    public:
    Level();

    void complete_level();

    void new_level(int difficulty);

    //The dimensions of the level map, in tiles.
    unsigned int level_x,level_y;

    std::vector< std::vector<Tile> > tile_array;

    std::vector<Background_Layer> background_layers;

    std::vector<Menu_Background> menu_backgrounds;
};

#endif
