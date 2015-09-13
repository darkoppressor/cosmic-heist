/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "update.h"
#include "world.h"
#include "quit.h"
#include "render.h"
#include "version.h"
#include "hud_buttons.h"

#include <SDL.h>

using namespace std;

//Gather input.
void input(){
    //Start each frame of updates with the tooltip off.
    tooltip.on=false;

    player.prepare_for_input();

    while(SDL_PollEvent(&event)){
        hud_buttons_handle_input_events();

        window_highscores[0].handle_input_events();

        for(int i=0;i<vector_windows.size();i++){
            vector_windows[i].handle_input_events();
        }

        player.handle_input_events();

        switch(event.type){
            //Check to see if the player has X'ed out of the game. If so, run quit_game().
            case SDL_QUIT:
            quit_game();
            break;
        }
    }

    hud_buttons_handle_input_states();

    window_highscores[0].handle_input_states();

    for(int i=0;i<vector_windows.size();i++){
        vector_windows[i].handle_input_states();
    }

    //As long as the game is not paused and is in progress.
    if(!player.pause && player.game_in_progress){
        player.handle_input_states();

        for(int i=0;i<ships.size();i++){
            ships[i].handle_ai();
            ships[i].handle_input_ai();
        }
    }
    //If the game is paused and is in progress.
    else if(player.pause && player.game_in_progress){
        player.handle_input_states_always();
        player.handle_input_states_during_play();
    }
    //If the game is not in progress.
    else if(!player.game_in_progress){
        player.handle_input_states_always();
    }
}

//Move things that need to be moved.
void movement(){
    if(!player.pause && player.game_in_progress){
        player_ship[0].move();

        for(int i=0;i<ships.size();i++){
            ships[i].move();
        }
    }
}

//Handle events, such as collisions.
void events(){
    music.fadein_tracks();
    music.fadeout_tracks();

    if(!player.pause && player.game_in_progress){
        //If no explosions exist, then we clear the respective vector.
        if(player.existing_explosions==0){
            explosions.clear();
        }
    }
}

//Handle animations.
void animation(){
    //As long as the game is not paused and is in progress.
    if(!player.pause && player.game_in_progress){
        //Set the player's current tile position.
        int camera_current_x=(int)((int)player.camera_x/TILE_W);
        int camera_current_y=(int)((int)player.camera_y/TILE_H);

        //Check all tiles in a rectangle around the player.
        int check_x_start=camera_current_x;
        int check_x_end=camera_current_x+(int)((int)player.camera_w/TILE_W)+2;
        int check_y_start=camera_current_y;
        int check_y_end=camera_current_y+(int)((int)player.camera_h/TILE_H)+2;

        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(levels[0].level_x)-1 && int_y>=0 && int_y<=(levels[0].level_y)-1){
                    levels[0].tile_array[int_x][int_y].animate();
                }
            }
        }

        for(int i=0;i<exits.size();i++){
            exits[i].animate();
        }

        player_ship[0].animate();

        for(int i=0;i<ships.size();i++){
            ships[i].animate();
        }

        for(int i=0;i<explosions.size();i++){
            explosions[i].animate();
        }
    }

    if(!player.game_in_progress){
        player.animate_menu_title();
    }
}

//Handle camera movement.
void camera(int frame_rate,double ms_per_frame,int logic_frame_rate){
    //As long as the game is in progress.
    if(player.game_in_progress){
        //Handle camera movements for the player.
        player.set_camera();
    }

    player.update_background();

    player.update_window_caption(frame_rate,ms_per_frame,logic_frame_rate);
}

//Render everything to the screen.
void render(int frame_rate,double ms_per_frame,int logic_frame_rate){
    //Clear the screen, filling it with the clear color set in Window.init().
    glClear(GL_COLOR_BUFFER_BIT);

    player.render_background();

    //As long as the game is in progress.
    if(player.game_in_progress){
        //Set the player's current tile position.
        int camera_current_x=(int)((int)player.camera_x/TILE_W);
        int camera_current_y=(int)((int)player.camera_y/TILE_H);

        //Check all tiles in a rectangle around the player.
        int check_x_start=camera_current_x;
        int check_x_end=camera_current_x+(int)((int)player.camera_w/TILE_W)+2;
        int check_y_start=camera_current_y;
        int check_y_end=camera_current_y+(int)((int)player.camera_h/TILE_H)+2;

        //Render each onscreen terrain tile.
        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){
                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(levels[0].level_x)-1 && int_y>=0 && int_y<=(levels[0].level_y)-1){
                    levels[0].tile_array[int_x][int_y].render();
                }
            }
        }

        for(int i=0;i<exits.size();i++){
            exits[i].render();
        }

        player_ship[0].render();

        for(int i=0;i<ships.size();i++){
            ships[i].render();
        }

        for(int i=0;i<explosions.size();i++){
            explosions[i].render();
        }

        player.render_foreground();

        ss.clear();ss.str("");ss<<"Health: ";ss<<player_ship[0].health;msg=ss.str();
        font.show(25+2,15+2,msg,COLOR_BLACK);
        font.show(25,15,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Score: ";ss<<player.score;msg=ss.str();
        font.show((player.option_resolution_x-msg.length()*font.spacing_x)/2.0+2,15+2,msg,COLOR_BLACK);
        font.show((player.option_resolution_x-msg.length()*font.spacing_x)/2.0,15,msg,COLOR_WHITE);

        ss.clear();ss.str("");ss<<"Level: ";ss<<player.difficulty+1;msg=ss.str();
        font.show(player.option_resolution_x-150+2,15+2,msg,COLOR_BLACK);
        font.show(player.option_resolution_x-150,15,msg,COLOR_WHITE);

        player.render_menu_background();

        //If the game is paused.
        if(player.pause){
            render_rectangle((player.option_resolution_x-160)/2,(player.option_resolution_y-85)/2,160,85,1.0,COLOR_PAYNES_GRAY);
            render_rectangle((player.option_resolution_x-160)/2+2,(player.option_resolution_y-85)/2+2,160-4,85-4,1.0,COLOR_DARK_GRAY);
            font.show((player.option_resolution_x-71)/2+2,(player.option_resolution_y-16)/2+2,"Paused",COLOR_BLACK);
            font.show((player.option_resolution_x-71)/2,(player.option_resolution_y-16)/2,"Paused",COLOR_WHITE);
        }
    }
    //If a game is not in progress.
    else{
        player.render_menu_background();

        //Display the game title.
        ss.clear();ss.str("");ss<<"Cosmic Heist";msg=ss.str();
        font.show((player.option_resolution_x-msg.length()*font.spacing_x*2)/2.0+4,30+4,msg,COLOR_BLACK,player.menu_title_opacity,2.0,2.0);
        font.show((player.option_resolution_x-msg.length()*font.spacing_x*2)/2.0+2,30+2,msg,COLOR_BLUE,player.menu_title_opacity,2.0,2.0);
        font.show((player.option_resolution_x-msg.length()*font.spacing_x*2)/2.0,30,msg,COLOR_WHITE,player.menu_title_opacity,2.0,2.0);

        //Display the game version number.
        ss.clear();ss.str("");ss<<"Version: ";ss<<AutoVersion::MAJOR;ss<<".";ss<<AutoVersion::MINOR;ss<<" ";ss<<AutoVersion::STATUS;ss<<"\xA";msg=ss.str();
        font.show(0+2,player.option_resolution_y-25+2,msg,COLOR_BLACK);
        font.show(0,player.option_resolution_y-25,msg,COLOR_WHITE);
    }

    for(int i=0;i<hud_buttons.size();i++){
        hud_buttons[i].render(0,0);
    }

    window_highscores[0].render();

    for(int i=0;i<vector_windows.size();i++){
        vector_windows[i].render();
    }

    tooltip.render();

    //Display development stuff.
    if(player.option_dev){
        ss.clear();ss.str("");ss<<"Ships: ";ss<<ships.size();ss<<"\xA";msg=ss.str();
        ss.clear();ss.str("");ss<<"Level Size: ";ss<<levels[0].level_x;ss<<"/";ss<<levels[0].level_y;ss<<"\xA";msg+=ss.str();
        if(player_ship.size()>0){
            ss.clear();ss.str("");ss<<"Position: ";ss<<player_ship[0].x/32;ss<<",";ss<<player_ship[0].y/32;ss<<"\xA";msg+=ss.str();
        }
        font.show(550,60,msg,COLOR_RED);
    }

    //Swap the buffers, updating the screen.
    SDL_GL_SwapBuffers();
}
