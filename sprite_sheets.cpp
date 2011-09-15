#include "sprite_sheets.h"
#include "world.h"
#include "render.h"

using namespace std;

vector<SDL_Rect> sprites_tiles;

vector<ship_sprite_data> SPRITES_SHIPS;

vector<SDL_Rect> sprites_exit;

vector<SDL_Rect> sprites_explosion;

void create_sprite_sheets_tiles(){
    sprites_tiles.clear();
    sprites_tiles.push_back(SDL_Rect());

    for(int i=TILE_SPRITE_BEGIN+1;i<TILE_SPRITE_END;i++){
        sprites_tiles.push_back(SDL_Rect());
        sprites_tiles[i].x=0;
        sprites_tiles[i].y=(i-1)*TILE_H;
        sprites_tiles[i].w=TILE_W;
        sprites_tiles[i].h=TILE_H;
    }
}

void create_sprite_sheets_ships(){
    SPRITES_SHIPS.clear();

    for(int n=SHIP_TYPE_BEGIN;n<SHIP_TYPE_END;n++){
        SPRITES_SHIPS.push_back(ship_sprite_data());
        int idle_sprites=0;
        for(short i=0;i<SHIP_SPRITE_COUNTS[n].idle_sprites;i++){
            SPRITES_SHIPS[n].idle.push_back(SDL_Rect());
            idle_sprites++;
            SPRITES_SHIPS[n].idle[i].x=i*ship_sizes[SHIP_TYPE_PLAYER].w;
            SPRITES_SHIPS[n].idle[i].y=n*ship_sizes[SHIP_TYPE_PLAYER].h;
            SPRITES_SHIPS[n].idle[i].w=ship_sizes[n].w;
            SPRITES_SHIPS[n].idle[i].h=ship_sizes[n].h;
        }
        for(short i=0;i<SHIP_SPRITE_COUNTS[n].walk_sprites;i++){
            SPRITES_SHIPS[n].walk.push_back(SDL_Rect());
            SPRITES_SHIPS[n].walk[i].x=idle_sprites*ship_sizes[SHIP_TYPE_PLAYER].w+i*ship_sizes[SHIP_TYPE_PLAYER].w;
            SPRITES_SHIPS[n].walk[i].y=n*ship_sizes[SHIP_TYPE_PLAYER].h;
            SPRITES_SHIPS[n].walk[i].w=ship_sizes[n].w;
            SPRITES_SHIPS[n].walk[i].h=ship_sizes[n].h;
        }
    }
}

void create_sprite_sheets_exit(){
    sprites_exit.clear();

    for(int i=0;i<4;i++){
        sprites_exit.push_back(SDL_Rect());
        sprites_exit[i].x=i*EXIT_W;
        sprites_exit[i].y=0;
        sprites_exit[i].w=EXIT_W;
        sprites_exit[i].h=EXIT_H;
    }
}

void create_sprite_sheets_explosion(){
    sprites_explosion.clear();

    for(int i=0;i<3;i++){
        sprites_explosion.push_back(SDL_Rect());
        sprites_explosion[i].x=0;
        sprites_explosion[i].y=i*EXPLOSION_H;
        sprites_explosion[i].w=EXPLOSION_W;
        sprites_explosion[i].h=EXPLOSION_H;
    }
}

void generate_sprite_sheets(){
    create_sprite_sheets_tiles();
    create_sprite_sheets_ships();
    create_sprite_sheets_exit();
    create_sprite_sheets_explosion();
}
