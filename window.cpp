/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "window.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "quit.h"
#include "button_events.h"
#include "options.h"

using namespace std;

Window::Window(short get_x,short get_y,short get_w,short get_h,string get_title,short get_special_input){
    x=get_x;
    y=get_y;
    w=get_w;
    h=get_h;
    mouse_offset_x=0;
    mouse_offset_y=0;
    title=get_title;
    on=false;
    moving=false;
    special_input=get_special_input;

    //Create the close button.
    create_button(w-23,5,"","X",&button_event_close_window,&sound_button_mouse_over[12-1],&sound_button_event_fire[12-1],BUTTON_VISIBLE,-1);
}

Window::Window(){
}

Window::~Window(){
}

short Window::return_w(){
    return w;
}
short Window::return_h(){
    return h;
}

bool Window::return_on(){
    return on;
}

void Window::toggle_on(){
    on=!on;

    //If the window was just turned off, stop its movement.
    if(!on){
        moving=false;

        //Whenever a window is closed, we unpause the game.
        player.toggle_pause(false);

        //Whenever a window is closed, we reset the key config setting state.
        player.begin_setting_command(-1);
    }
    //If the window was just turned on.
    else{
        //Whenever a window is opened, we pause the game.
        player.toggle_pause(true);
    }
}

void Window::turn_off(){
    on=false;

    //The window was just turned off, so stop its movement.
    moving=false;

    //Whenever a window is closed, we unpause the game.
    player.toggle_pause(false);

    //Whenever a window is closed, we reset the key config setting state.
    //Note that this call is only here and in toggle_on() in the Window class, but not any of the subclasses of Window.
    //The reason is that this command is needed to ensure that whenever the user closes the key config window, the game stops looking for a
    //new key to bind to a command, if it was doing so.
    player.begin_setting_command(-1);
}

void Window::turn_on(){
    on=true;

    //Whenever a window is opened, we pause the game.
    player.toggle_pause(true);
}

void Window::special_input_create_profile(){
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
    SDL_EnableUNICODE(SDL_ENABLE);

    if(event.type==SDL_QUIT){
        quit_game();
    }

    if(event.type==SDL_KEYDOWN && profile.profile_list.size()<7){
        //If the string is less than the maximum size.
        if(creating_profile.length()<16){
            if((event.key.keysym.unicode>=(Uint16)'0' && event.key.keysym.unicode<=(Uint16)'9') || (event.key.keysym.unicode>=(Uint16)'A' && event.key.keysym.unicode<=(Uint16)'Z') || (event.key.keysym.unicode>=(Uint16)'a' && event.key.keysym.unicode<=(Uint16)'z')){
                creating_profile+=(char)event.key.keysym.unicode;
            }
        }

        //If the backspace key is pressed and the string is not empty.
        if(event.key.keysym.sym==SDLK_BACKSPACE && creating_profile.length()>0){
            //Remove one character from the end of the string.
            creating_profile.erase(creating_profile.length()-1);
        }

        //If the player hits enter, we create the new profile.
        if(event.key.keysym.sym==SDLK_RETURN || event.key.keysym.sym==SDLK_KP_ENTER){
            profile.create_profile();
        }
    }

    SDL_EnableKeyRepeat(0,0);
}

void Window::clear_buttons(){
    buttons.clear();
}

void Window::clear_information(){
    informations.clear();
}

void Window::create_button(short b_x,short b_y,string b_tooltip_text,string b_text,void (*b_event_function)(Window*,int),Mix_Chunk** get_mouse_over_sound,Mix_Chunk** get_event_fire_sound,bool get_invisible,int get_special_data,bool get_enabled){
    buttons.push_back(Button(b_x,b_y,b_tooltip_text,b_text,b_event_function,get_mouse_over_sound,get_event_fire_sound,get_invisible,get_special_data,get_enabled));
}

void Window::create_information(short get_x,short get_y,string get_tooltip_text,string get_text,image_data* get_info_image,short get_special_info){
    informations.push_back(Information(get_x,get_y,get_tooltip_text,get_text,get_info_image,get_special_info));
}

void Window::handle_input_states(){
    if(on){
        int mouse_x,mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);

        //If the window is moving, center it on the mouse's current position - the offsets.
        if(moving){
            x=mouse_x-mouse_offset_x;
            y=mouse_y-mouse_offset_y;

            //Don't allow the window to leave the screen.
            if(x<0){
                x=0;
            }
            if(y<0){
                y=0;
            }
            if(x+w>player.option_resolution_x){
                x=player.option_resolution_x-w;
            }
            if(y+h>player.option_resolution_y){
                y=player.option_resolution_y-h;
            }
        }

        //If the window is not moving.
        else if(!moving){
            //Check to see if the mouse is hovering over any information pieces.
            for(int i=0;i<informations.size();i++){
                informations[i].handle_input(mouse_x,mouse_y,x,y);
            }

            //Check to see if the mouse is hovering over any of this window's buttons.
            for(int i=0;i<buttons.size();i++){
                //For each button, reset its moused over state before anything else.
                //Remember whether or not the button was moused over before this reset.
                bool already_moused_over=buttons[i].reset_moused_over();

                //If the mouse is hovering over this button.
                if(buttons[i].is_moused_over(mouse_x,mouse_y,x,y)){
                    //The button is now being moused over.
                    buttons[i].mouse_over(already_moused_over);

                    //Setup the button's tooltip.
                    tooltip.setup(buttons[i].return_tooltip_text(),mouse_x,mouse_y);
                }
            }
        }
    }
}

void Window::handle_input_events(){
    if(on){
        int mouse_x,mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);

        if(special_input==SPECIAL_INPUT_CREATE_PROFILE){
            special_input_create_profile();
        }

        switch(event.type){
            case SDL_QUIT:
                quit_game();
                break;

            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button==SDL_BUTTON_LEFT){
                    bool button_clicked=false;

                    //Look through all of the buttons.
                    for(int i=0;i<buttons.size();i++){
                        //If this button is moused over,
                        //it has been clicked down on.
                        if(buttons[i].is_moused_over(mouse_x,mouse_y,x,y)){
                            buttons[i].mouse_button_down();
                            //A button has just been clicked, so we keep that in mind.
                            button_clicked=true;
                        }
                    }

                    //If no buttons were just clicked and the title bar of the window is clicked.
                    if(!button_clicked && collision_check(mouse_x,mouse_y,2,2,x,y,w,30)){
                        //Begin moving the window.
                        moving=true;
                        mouse_offset_x=mouse_x-x;
                        mouse_offset_y=mouse_y-y;
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button==SDL_BUTTON_LEFT){
                    //Stop moving the window.
                    moving=false;

                    //Look through all of the buttons.
                    for(int i=0;i<buttons.size();i++){
                        //If this button is moused over,
                        //the mouse button has been released over it.
                        if(buttons[i].is_moused_over(mouse_x,mouse_y,x,y)){
                            buttons[i].mouse_button_up(this);
                            buttons[i].reset_clicked();
                        }
                        //Otherwise, the mouse was not released over this button.
                        else{
                            buttons[i].reset_clicked();
                        }
                    }
                }
                break;
        }
    }
}

void Window::render(){
    if(on){
        //Render the border.
        render_rectangle(x,y,w,h,1.0,COLOR_PAYNES_GRAY);

        //Render the background.
        render_rectangle(x+5,y+5,w-10,h-10,1.0,COLOR_DARK_GRAY);

        //Render the title bar.
        render_rectangle(x+10,y+10,w-20,20,1.0,COLOR_GRAY);

        //Display the window's title.
        font.show(x+(w-(title.length()*12))/2+2,y+12+2,title,COLOR_BLACK,1.0);
        font.show(x+(w-(title.length()*12))/2,y+12,title,COLOR_WHITE,1.0);

        //Render the information pieces.
        for(int i=0;i<informations.size();i++){
            informations[i].render(x,y);
        }

        //Render the buttons.
        for(int i=0;i<buttons.size();i++){
            buttons[i].render(x,y);
        }
    }
}
