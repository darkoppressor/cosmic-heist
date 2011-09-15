/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "sizes.h"
#include "enumerations.h"

using namespace std;

vector<dimensions> ship_sizes;

void prepare_ship_sizes(){
    ship_sizes.clear();

    for(int i=SHIP_TYPE_BEGIN;i<SHIP_TYPE_END;i++){
        ship_sizes.push_back(dimensions());
    }

    ship_sizes[SHIP_TYPE_PLAYER].w=23;
    ship_sizes[SHIP_TYPE_PLAYER].h=23;

    ship_sizes[SHIP_TYPE_SPECK].w=17;
    ship_sizes[SHIP_TYPE_SPECK].h=17;

    ship_sizes[SHIP_TYPE_BULLET].w=19;
    ship_sizes[SHIP_TYPE_BULLET].h=19;

    ship_sizes[SHIP_TYPE_TORP].w=23;
    ship_sizes[SHIP_TYPE_TORP].h=19;
}
