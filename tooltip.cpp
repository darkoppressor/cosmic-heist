/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "tooltip.h"
#include "world.h"
#include "render.h"

using namespace std;

Tooltip::Tooltip(){
    on=false;

    message="";

    //Starting coordinates.
    x=0;
    y=0;

    //Dimensions.
    w=0;
    h=0;
}

void Tooltip::setup(string get_message,int mouse_x,int mouse_y){
    message=get_message;
    on=true;
    x=mouse_x;
    y=mouse_y+24;
    set_dimensions();

    //If the tooltip would be displayed off the screen, move it.

    if(x+w>player.option_resolution_x){
        x=mouse_x-w;
    }

    if(y+h>player.option_resolution_y){
        y=mouse_y-24-h;
    }
}

void Tooltip::set_dimensions(){
    short current_width=11;
    w=11;
    h=24;

    for(short i=0;i<message.length();i++){
        //
        if(message[i]!='\xA'){
            current_width+=11;
        }
        //If we hit a new line.
        else{
            h+=24;
            if(current_width>w){
                w=current_width;
            }
            current_width=11;
        }

        if(i==message.length()-1){
            if(current_width>w){
                w=current_width;
            }
        }
    }
}

void Tooltip::render(){
    //As long as the tooltip is on and has content.
    if(on && message.length()>0){
        render_rectangle(x-4,y-4,w,h,1.0,COLOR_BROWN);

        font.show(x,y,message,COLOR_WHEAT);
    }
}
