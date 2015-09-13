/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "window_highscores.h"
#include "world.h"
#include "render.h"
#include "collision.h"
#include "quit.h"
#include "button_events.h"

#include <fstream>

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost::algorithm;

Window_Highscores::Window_Highscores(short get_x,short get_y,short get_w,short get_h){
    x=get_x;
    y=get_y;
    w=get_w;
    h=get_h;

    title="Highscores";

    on=false;
    moving=false;

    mouse_offset_x=0;
    mouse_offset_y=0;

    //Create the close button.
    create_button(w-23,5,"","X",&button_event_close_window,&sound_button_mouse_over[12-1],&sound_button_event_fire[12-1],BUTTON_VISIBLE);
}

void Window_Highscores::update_highscores(){
    highscores.clear();

    //Load the highscores.

    ifstream load("highscores",ifstream::in);

    if(load!=NULL){
        //As long as we haven't reached the end of the file.
        while(!load.eof()){
            string line="";

            //Grab the next line of the file.
            getline(load,line);

            //Clear initial whitespace from the line.
            trim(line);

            //If the line begins a highscore entry.
            if(icontains(line,"<highscore>")){
                highscores.push_back(Highscore());

                //As long as we haven't reached the end of the file.
                while(!load.eof()){
                    //Grab the next line of the file.
                    getline(load,line);

                    //Clear initial whitespace from the line.
                    trim(line);

                    //Score.
                    if(icontains(line,"<score>")){
                        line.erase(0,7);

                        highscores[highscores.size()-1].score=atoll(line.c_str());
                    }
                    //Score string.
                    else if(icontains(line,"<name>")){
                        line.erase(0,6);

                        highscores[highscores.size()-1].name=line;

                        break;
                    }
                }
            }
        }

        load.close();
        load.clear();
    }
    else{
        fprintf(stderr,"Error loading highscores file.\n");
    }

    while(highscores.size()<10){
        highscores.push_back(Highscore());
    }
}

void Window_Highscores::toggle_on(){
    on=!on;

    //If the window was just turned off, stop its movement and drop any dragged item.
    if(!on){
        moving=false;

        //Whenever a window is closed, we unpause the game.
        player.toggle_pause(false);
    }
    //If the window was just turned on.
    else{
        //Whenever a window is opened, we pause the game.
        player.toggle_pause(true);
    }
}

void Window_Highscores::turn_off(){
    on=false;

    //The window was just turned off, so stop its movement and drop any dragged item.
    moving=false;

    //Whenever a window is closed, we unpause the game.
    player.toggle_pause(false);
}

void Window_Highscores::handle_input_states(){
    if(on){
        int mouse_x,mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);

        //If the window is moving, center it on the mouse's current position - the offsets.
        if(moving){
            x=mouse_x-mouse_offset_x;
            y=mouse_y-mouse_offset_y;

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

        else if(!moving){
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

void Window_Highscores::handle_input_events(){
    if(on){
        int mouse_x,mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);

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
                    //Stop moving the inventory window.
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

void Window_Highscores::render(){
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

        short render_color=COLOR_BLACK;

        for(int i=0;i<highscores.size();i++){
            if(player.current_highscore==i){
                render_rectangle(x+10,y+35+i*font.spacing_y*1.6,w-20,font.spacing_y,1.0,COLOR_PAYNES_GRAY);
                render_rectangle(x+12,y+35+i*font.spacing_y*1.6+2,w-20-4,font.spacing_y-4,1.0,COLOR_LIGHT_GRAY);
            }

            ss.clear();ss.str("");ss<<highscores[i].name;ss<<"\xA";msg=ss.str();
            font.show(x+15,y+40+i*font.spacing_y*1.6,msg,render_color);

            ss.clear();ss.str("");ss<<highscores[i].score;ss<<"\xA";msg=ss.str();
            font.show(x+15+250,y+40+i*font.spacing_y*1.6,msg,render_color);
        }

        //Render the buttons.
        for(int i=0;i<buttons.size();i++){
            buttons[i].render(x,y);
        }
    }
}
