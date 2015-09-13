/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "tile.h"
#include "render.h"
#include "world.h"

using namespace std;

Tile::Tile(){
    x=0.0;
    y=0.0;
    solid=false;
    sprite=TILE_SPRITE_EMPTY;

    angle=0.0;
    rotate_speed=0.1;
    rotate_direction=false;
}

void Tile::prepare(){
    angle=0.0;
    rotate_speed=random_range(1,10)/10.0;
    if(random_range(0,99)<50){
        rotate_direction=true;
    }
    else{
        rotate_direction=false;
    }
}

void Tile::animate(){
    if(sprite!=TILE_SPRITE_EMPTY){
        if(rotate_direction){
            angle+=rotate_speed;
        }
        else{
            angle-=rotate_speed;
        }

        if(angle>359.9){
            angle=0.0;
        }
        if(angle<0.0){
            angle=359.9;
        }
    }
}

void Tile::render(){
    //If the tile is in camera bounds, render the tile.
    if(x>=player.camera_x-TILE_W && x<=player.camera_x+player.camera_w && y>=player.camera_y-TILE_H && y<=player.camera_y+player.camera_h){
        if(sprite!=TILE_SPRITE_EMPTY){
            render_sprite((int)(x-player.camera_x),(int)(y-player.camera_y),image.tiles,&sprites_tiles[sprite],1.0,1.0,1.0,angle);

            if(player.option_dev){
                ///render_rectangle_empty((int)(x-player.camera_x),(int)(y-player.camera_y),TILE_W,TILE_H,1.0,COLOR_GREEN);
                render_rectangle_empty((int)(x+TILE_COLLISION_SIZE-player.camera_x),(int)(y+TILE_COLLISION_SIZE-player.camera_y),TILE_W-TILE_COLLISION_SIZE*2,TILE_H-TILE_COLLISION_SIZE*2,1.0,COLOR_RED);
            }
        }
    }
}
