/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "level.h"
#include "world.h"
#include "render.h"
#include "collision.h"
#include "positional_audio.h"

#include <fstream>

using namespace std;

Level::Level(){
    level_x=0;
    level_y=0;
}

void Level::complete_level(){
    player.score+=player.difficulty*100;
    player.difficulty++;
    player.score+=player_ship[0].health/2.0;
    new_level(player.difficulty);
}

void Level::new_level(int difficulty){
    music.play_track(MUSIC_LEVEL_1);

    play_positional_sound(sound_level_start);

    int level_min_size=64+difficulty*2;
    int level_max_size=level_min_size+difficulty*8;

    level_x=random_range(level_min_size,level_max_size);
    level_y=random_range(level_min_size,level_max_size);

    tile_array.clear();
    tile_array.resize(level_x,vector<Tile>(level_y));

    //Set the whole level to empty space.
    for(int x=0;x<level_x;x++){
        for(int y=0;y<level_y;y++){
            tile_array[x][y].x=x*TILE_W;
            tile_array[x][y].y=y*TILE_H;
            tile_array[x][y].solid=false;
            tile_array[x][y].sprite=TILE_SPRITE_EMPTY;
            tile_array[x][y].prepare();
        }
    }

    //Create the player's ship.
    int health=500;
    if(player_ship.size()>0){
        health=player_ship[0].health;
        if(difficulty<3){
            health+=10;
        }
        else if(difficulty>=3 && difficulty<8){
            health+=5;
        }
        else if(difficulty>=8 && difficulty<12){
            health+=2;
        }
    }

    player_ship.clear();
    player_ship.push_back(Ship(random_range(level_x*TILE_W/2.0-500,level_x*TILE_W/2.0+500),random_range(level_y*TILE_H/2.0-500,level_y*TILE_H/2.0+500),SHIP_TYPE_PLAYER,true));
    player_ship[0].health=health;

    ///Create the initial shipyard.

    //Generate the extraction points.
    int number_of_extraction_points=0;
    if(difficulty>=0 && difficulty<8){
        number_of_extraction_points=5;
    }
    else if(difficulty>=8 && difficulty<14){
        number_of_extraction_points=4;
    }
    else if(difficulty>=14 && difficulty<18){
        number_of_extraction_points=3;
    }
    else if(difficulty>=18 && difficulty<20){
        number_of_extraction_points=2;
    }
    else{
        number_of_extraction_points=1;
    }

    exits.clear();
    for(int i=0;i<number_of_extraction_points;i++){
        double x=-1.0;
        double y=-1.0;

        int random=random_range(0,99);

        if(random>=0 && random<25){
            //Left side.
            x=0.0;
            y=random_range(0,level_y*TILE_H-EXIT_H);
        }
        else if(random>=25 && random<50){
            //Top side.
            x=random_range(0,level_x*TILE_W-EXIT_W);
            y=0.0;
        }
        else if(random>=50 && random<75){
            //Right side.
            x=level_x*TILE_W-EXIT_W;
            y=random_range(0,level_y*TILE_H-EXIT_H);
        }
        else if(random>=75){
            //Bottom side.
            x=random_range(0,level_x*TILE_W-EXIT_W);
            y=level_y*TILE_H-EXIT_H;
        }

        bool create_this_exit=true;

        for(int n=0;n<exits.size();n++){
            if(collision_check(x,y,EXIT_W,EXIT_H,exits[n].x,exits[n].y,EXIT_W,EXIT_H)){
                i--;
                create_this_exit=false;
                break;
            }
        }

        if(create_this_exit){
            exits.push_back(Exit(x,y));
        }
    }

    //Create debris.
    int number_of_tiles=level_x*level_y/(250.0);
    number_of_tiles+=difficulty*random_range(5,10);
    for(int i=0;i<number_of_tiles;i++){
        int x=random_range(5,level_x-6);
        int y=random_range(5,level_y-6);

        if(!collision_check(x*TILE_W,y*TILE_H,TILE_W,TILE_H,player_ship[0].x,player_ship[0].y,player_ship[0].return_w(),player_ship[0].return_h())){
            tile_array[x][y].solid=true;
            tile_array[x][y].sprite=random_range(TILE_SPRITE_BEGIN+1,TILE_SPRITE_END-1);
        }
    }

    //Create enemy ships.
    ships.clear();
    int number_of_ships=level_x*level_y/(500.0);
    number_of_ships+=difficulty*random_range(5,10);
    for(int i=0;i<number_of_ships;i++){
        int random=random_range(0,99);
        int type=SHIP_TYPE_SPECK;

        if(random>=0 && random<50){
            type=SHIP_TYPE_SPECK;
        }
        else if(random>=50 && random<85){
            type=SHIP_TYPE_BULLET;
        }
        else if(random>=85){
            type=SHIP_TYPE_TORP;
        }

        ships.push_back(Ship(random_range(0,(level_x-1)*TILE_W),random_range(0,(level_y-1)*TILE_H),type,false));
    }
}
