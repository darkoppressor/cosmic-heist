/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "information.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "version.h"
#include "special_info_strings.h"

using namespace std;

Information::Information(short get_x,short get_y,string get_tooltip_text,string get_text,image_data* get_info_image,short get_special_info){
    x=get_x;
    y=get_y;

    text=get_text;
    tooltip_text=get_tooltip_text;

    info_image=get_info_image;

    special_info=get_special_info;

    //If there is special info.
    if(special_info!=NO_SPECIAL_INFO){
        //Setup the special string, if any.
        set_special_string();
        //Setup the special image, if any.
        set_special_image();
    }

    set_dimensions();
}

void Information::set_dimensions(){
    //If the information has text.
    if(text.length()>0){
        short current_width=11;
        w=11;
        h=24;

        for(short i=0;i<text.length();i++){
            //
            if(text[i]!='\xA'){
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

            if(i==text.length()-1){
                if(current_width>w){
                    w=current_width;
                }
            }
        }
    }
    //If the information has an image.
    else if(info_image!=0){
        w=info_image->w;
        h=info_image->h;
    }
}

void Information::set_special_string(){
    text.clear();

    special_info_return_data special_info_data;

    special_info_data=set_special_info_string(special_info);

    text=special_info_data.text;
    short command=special_info_data.command;

    short options_keys_width=600;

    //If the special info is a key bind.
    if(special_info>=SPECIAL_INFO_KEY_TOGGLE_MAIN_MENU && special_info<=SPECIAL_INFO_KEY_PAUSE){
        //If this key is currently being set.
        if(player.current_command_to_set()==command){
            //If no input already bound message needs to be relayed.
            if(player.return_input_already_bound()==-1){
                ss.clear();ss.str("");ss<<"Press any key";text=ss.str();
            }
            //If we need to relay an input already bound message.
            else{
                text=player.command_bound_input(player.return_input_already_bound());
                text+=" is already bound.";
            }
        }
        //If this key is not being set.
        else{
            text=player.command_bound_input(command);
        }
        set_dimensions();
        x=options_keys_width-w-15;
    }
}

void Information::set_special_image(){
    info_image=0;

    switch(special_info){

    case SPECIAL_INFO_CHECKBOX_FULLSCREEN:
        if(player.option_fullscreen){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_FPS:
        if(player.option_fps){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_MUTE_GLOBAL:
        if(player.option_mute_global){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_MUTE_SOUND:
        if(player.option_mute_sound){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    case SPECIAL_INFO_CHECKBOX_MUTE_MUSIC:
        if(player.option_mute_music){
            info_image=&image.checkbox_checked;
        }
        else{
            info_image=&image.checkbox_unchecked;
        }
        break;

    }
}

void Information::handle_input(int mouse_x,int mouse_y,short x_offset,short y_offset){
    if(collision_check(mouse_x,mouse_y,2,2,x_offset+x,y_offset+y,w,h)){
        //Setup the information's tooltip.
        tooltip.setup(tooltip_text,mouse_x,mouse_y);
    }
}

void Information::render(short x_offset,short y_offset){
    //If there is special info, it needs to be processed before being displayed.
    if(special_info!=NO_SPECIAL_INFO){
        //Setup the special string, if any.
        set_special_string();
        //Setup the special image, if any.
        set_special_image();
        set_dimensions();
    }

    //If the information has text.
    if(text.length()>0){
        //Display the information's text.
        font.show(x_offset+x+2,y_offset+y+6+2,text,COLOR_BLACK,1.0);
        font.show(x_offset+x,y_offset+y+6,text,COLOR_WHITE,1.0);
    }
    //If the information has an image.
    else if(info_image!=0){
        render_texture(x_offset+x,y_offset+y,*info_image,1.0);
    }
    //If the information has special non-text and non-image info.
    else{
        double volume_percentage=0.0;
        double volume_bar_width=0.0;

        switch(special_info){

        case SPECIAL_INFO_VOLUME_GLOBAL:
            volume_percentage=(double)player.option_volume_global/1.0;
            volume_bar_width=200.0*volume_percentage;

            //Render the background.
            render_rectangle(x_offset+x-5,y_offset+y-5,210,20,1.0,COLOR_GRAY);

            //Render the bar.
            render_rectangle(x_offset+x+2,y_offset+y+2,volume_bar_width,10,1.0,COLOR_BLACK);
            render_rectangle(x_offset+x,y_offset+y,volume_bar_width,10,1.0,COLOR_WHITE);
            break;

        case SPECIAL_INFO_VOLUME_SOUND:
            volume_percentage=(double)player.option_volume_sound/1.0;
            volume_bar_width=200.0*volume_percentage;

            //Render the background.
            render_rectangle(x_offset+x-5,y_offset+y-5,210,20,1.0,COLOR_GRAY);

            //Render the bar.
            render_rectangle(x_offset+x+2,y_offset+y+2,volume_bar_width,10,1.0,COLOR_BLACK);
            render_rectangle(x_offset+x,y_offset+y,volume_bar_width,10,1.0,COLOR_WHITE);
            break;

        case SPECIAL_INFO_VOLUME_MUSIC:
            volume_percentage=(double)player.option_volume_music/1.0;
            volume_bar_width=200.0*volume_percentage;

            //Render the background.
            render_rectangle(x_offset+x-5,y_offset+y-5,210,20,1.0,COLOR_GRAY);

            //Render the bar.
            render_rectangle(x_offset+x+2,y_offset+y+2,volume_bar_width,10,1.0,COLOR_BLACK);
            render_rectangle(x_offset+x,y_offset+y,volume_bar_width,10,1.0,COLOR_WHITE);
            break;

        }
    }
}
