/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "exit.h"
#include "render.h"
#include "world.h"

using namespace std;

Exit::Exit(double get_x,double get_y){
    x=get_x;
    y=get_y;

    frame=0;
    frame_counter=0;
}

void Exit::animate(){
    //Increment the frame counter.
    frame_counter++;

    if(frame_counter>=4){
        frame_counter=0;

        //Now increment the frame.
        frame++;

        if(frame>4-1){
            frame=0;
        }
    }
}

void Exit::render(){
    if(x>=player.camera_x-EXIT_W && x<=player.camera_x+player.camera_w && y>=player.camera_y-EXIT_H && y<=player.camera_y+player.camera_h){
        render_sprite((int)(x-player.camera_x),(int)(y-player.camera_y),image.exits,&sprites_exit[frame]);

        if(player.option_dev){
            ///render_rectangle_empty((int)(x-player.camera_x),(int)(y-player.camera_y),EXIT_W,EXIT_H,1.0,COLOR_GREEN);
            render_rectangle_empty((int)(x+EXIT_COLLISION_SIZE-player.camera_x),(int)(y+EXIT_COLLISION_SIZE-player.camera_y),EXIT_W-EXIT_COLLISION_SIZE*2,EXIT_H-EXIT_COLLISION_SIZE*2,1.0,COLOR_RED);
        }
    }
}
