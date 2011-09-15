/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "explosion.h"
#include "render.h"
#include "world.h"
#include "positional_audio.h"

using namespace std;

Explosion::Explosion(double get_x,double get_y,double get_scale,short get_type){
    x=get_x;
    y=get_y;
    scale=get_scale;

    frame=0;
    frame_counter=0;

    exists=true;
    player.existing_explosions++;

    if(get_type==EXPLOSION_1){
        play_positional_sound(sound_explosion_1,x,y,128*scale);
    }
    else if(get_type==EXPLOSION_2){
        play_positional_sound(sound_explosion_2,x,y,128*scale);
    }
    else if(get_type==EXPLOSION_3){
        play_positional_sound(sound_explosion_3,x,y,128*scale);
    }
}

void Explosion::animate(){
    if(exists){
        //Increment the frame counter.
        frame_counter++;

        if(frame_counter>=5){
            frame_counter=0;

            //Now increment the frame.
            frame++;

            if(frame>3-1){
                frame=0;
                exists=false;
                player.existing_explosions--;
            }
        }
    }
}

void Explosion::render(){
    if(exists){
        if(x>=player.camera_x-EXPLOSION_W && x<=player.camera_x+player.camera_w && y>=player.camera_y-EXPLOSION_H && y<=player.camera_y+player.camera_h){
            render_sprite((int)(x-player.camera_x),(int)(y-player.camera_y),image.explosion,&sprites_explosion[frame],1.0,scale,scale);
        }
    }
}
