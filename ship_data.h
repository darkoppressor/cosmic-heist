/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef ship_data_h
#define ship_data_h

#include <vector>

struct ship_sprite_count_data{
    short walk_sprites;
    short idle_sprites;
};

extern std::vector<ship_sprite_count_data> SHIP_SPRITE_COUNTS;

void prepare_ship_sprite_counts();

#endif
