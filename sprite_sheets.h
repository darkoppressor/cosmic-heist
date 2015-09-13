/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef sprite_sheets_h
#define sprite_sheets_h

#include "ship_data.h"

#include <vector>

#include <SDL.h>

extern std::vector<SDL_Rect> sprites_tiles;

struct ship_sprite_data{
    std::vector<SDL_Rect> walk;
    std::vector<SDL_Rect> idle;
};
extern std::vector<ship_sprite_data> SPRITES_SHIPS;

extern std::vector<SDL_Rect> sprites_exit;

extern std::vector<SDL_Rect> sprites_explosion;

void generate_sprite_sheets();

void create_sprite_sheets_tiles();
void create_sprite_sheets_ships();
void create_sprite_sheets_exit();
void create_sprite_sheets_explosion();

#endif
