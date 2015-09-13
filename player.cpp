/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "player.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "options.h"
#include "quit.h"
#include "highscore.h"

#include <cmath>
#include <ctime>

#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost::algorithm;

Player::Player(){
    //Start the keystates variable as NULL.
    keystates=NULL;

    reset();

    current_highscore=-1;
}

void Player::reset(){
    option_fullscreen=false;
    option_fps=false;
    option_dev=false;
    option_effect_limit=10000;
    option_volume_global=1.0;
    option_mute_global=false;
    option_volume_sound=1.0;
    option_mute_sound=false;
    option_volume_music=1.0;
    option_mute_music=false;
    option_resolution_x=800;
    option_resolution_y=600;

    //Note that commands must be setup here in the same order as command identifiers are setup in the enum in enumerations.h.
    keys.clear();
    keys.push_back(Input_Data(SDLK_F10));
    keys.push_back(Input_Data(SDLK_F5));
    keys.push_back(Input_Data(SDLK_LEFT));
    keys.push_back(Input_Data(SDLK_UP));
    keys.push_back(Input_Data(SDLK_RIGHT));
    keys.push_back(Input_Data(SDLK_DOWN));
    keys.push_back(Input_Data(SDLK_PAUSE));

    name="\x1F";

    score=0;

    difficulty=0;

    existing_explosions=0;

    input_already_bound=-1;
    command_to_set=-1;

    game_in_progress=false;

    block_input_events=false;

    //Set the camera's initial location and its dimensions.
    camera_x=0;
    camera_y=0;
    camera_w=option_resolution_x;
    camera_h=option_resolution_y;

    camera_speed=24;

    camera_delta_x=0.0;
    camera_delta_y=0.0;

    menu_background_counter_x=0;
    menu_background_counter_y=0;

    menu_background_counter_max_x=0;
    menu_background_counter_max_y=0;

    menu_background_x=NONE;
    menu_background_y=NONE;

    menu_speed_x=5.0;
    menu_speed_y=5.0;

    menu_title_opacity=1.0;
    menu_title_direction=false;
}

void Player::stop_game(){
    //If a game is in progress.
    if(game_in_progress){
        game_in_progress=false;

        ships.clear();
        exits.clear();
        explosions.clear();
        player_ship.clear();

        keys.clear();
        reset();

        music.play_track(MUSIC_MENU);

        load_data();
    }
}

void Player::start_game(){
    //If a game is not already in progress.
    if(!game_in_progress){
        //A game is now in progress.
        game_in_progress=true;

        //Load and prepare the player data.
        load_data();

        //Start a new game.
        levels[0].new_level(difficulty);
    }
}

void Player::load_data(){
    UPDATE_RATE=60.0;
    SKIP_TICKS=1000.0/UPDATE_RATE;

    pause=false;

    cam_state=CAM_STICKY;
}

void Player::toggle_pause(bool get_pause){
    //If we are pausing the game.
    if(get_pause){
        pause=true;
    }
    //If we are unpausing the game.
    else{
        pause=false;
    }
}

void Player::set_camera(){
    camera_delta_x=camera_x;
    camera_delta_y=camera_y;

    //If the camera is sticky, center the camera on the player.
    if(cam_state==CAM_STICKY){
        //Now center the camera on the player.
        camera_x=(player_ship[0].x+player_ship[0].return_w()/2)-(camera_w/2);
        camera_y=(player_ship[0].y+player_ship[0].return_h()/2)-(camera_h/2);
    }

    //Move the camera if it is unsticky:
    else{
        //Now check for camera input and move the camera accordingly.
        if(cam_state==LEFT){
            camera_x-=camera_speed;
        }
        if(cam_state==UP){
            camera_y-=camera_speed;
        }
        if(cam_state==RIGHT){
            camera_x+=camera_speed;
        }
        if(cam_state==DOWN){
            camera_y+=camera_speed;
        }
        if(cam_state==LEFT_UP){
            camera_x-=camera_speed;
            camera_y-=camera_speed;
        }
        if(cam_state==RIGHT_UP){
            camera_x+=camera_speed;
            camera_y-=camera_speed;
        }
        if(cam_state==RIGHT_DOWN){
            camera_x+=camera_speed;
            camera_y+=camera_speed;
        }
        if(cam_state==LEFT_DOWN){
            camera_x-=camera_speed;
            camera_y+=camera_speed;
        }
    }

    //If the camera goes past the bounds of the level map, set it back in the bounds.
    if(camera_x<0){
        camera_x=0;
    }
    if(camera_x+camera_w>levels[0].level_x*TILE_W){
        camera_x=levels[0].level_x*TILE_W-camera_w;
    }
    if(camera_y<0){
        camera_y=0;
    }
    if(camera_y+camera_h>levels[0].level_y*TILE_H){
        camera_y=levels[0].level_y*TILE_H-camera_h;
    }

    camera_delta_x=camera_x-camera_delta_x;
    camera_delta_y=camera_y-camera_delta_y;
}

void Player::update_window_caption(int frame_rate,double ms_per_frame,int logic_frame_rate){
    string temp="";

    time_t now;
    struct tm *tm_now;
    char buff[BUFSIZ];
    now=time(NULL);
    tm_now=localtime(&now);
    strftime(buff,sizeof buff,"   %A, %B %d, %Y - %H:%M:%S",tm_now);

    //Set the window caption.
    if(option_dev){
        msg="Cosmic Heist (DEV Mode)";
    }
    else{
        msg="Cosmic Heist";
    }

    //If a profile exists.
    if(name!="\x1F"){
        msg+=" - ";
        msg+=name;
    }

    msg+=buff;

    if(option_fps){
        ss.clear();ss.str("");ss<<"   FPS: ";ss<<frame_rate;msg+=ss.str();

        ss.clear();ss.str("");ss<<" / MSPF: ";ss<<ms_per_frame;msg+=ss.str();

        ss.clear();ss.str("");ss<<" / LUPS: ";ss<<logic_frame_rate;msg+=ss.str();
    }

    SDL_WM_SetCaption(msg.c_str(),msg.c_str());
}

void Player::update_background(){
    if(game_in_progress){
        for(int i=0;i<levels[0].background_layers.size();i++){
            levels[0].background_layers[i].update(camera_delta_x,camera_delta_y);
        }
    }

    menu_background_counter_x+=random_range(1,2);
    menu_background_counter_y+=random_range(1,2);

    if(menu_background_counter_x>=menu_background_counter_max_x){
        menu_background_counter_x=0;
        menu_background_counter_max_x=random_range(60,210);
        menu_speed_x=5.0/random_range(1,5);

        int random=random_range(0,2);
        if(menu_background_y==NONE){
            random=random_range(1,2);
        }
        if(random==0){
            menu_background_x=NONE;
        }
        else if(random==1){
            menu_background_x=LEFT;
        }
        else if(random==2){
            menu_background_x=RIGHT;
        }
    }
    if(menu_background_counter_y>=menu_background_counter_max_y){
        menu_background_counter_y=0;
        menu_background_counter_max_y=random_range(60,210);
        menu_speed_y=5.0/random_range(1,5);

        int random=random_range(0,2);
        if(menu_background_x==NONE){
            random=random_range(1,2);
        }
        if(random==0){
            menu_background_y=NONE;
        }
        else if(random==1){
            menu_background_y=UP;
        }
        else if(random==2){
            menu_background_y=DOWN;
        }
    }

    double fake_delta_x=0.0;
    double fake_delta_y=0.0;

    if(menu_background_x==LEFT){
        fake_delta_x=-menu_speed_x;
    }
    else if(menu_background_x==RIGHT){
        fake_delta_x=menu_speed_x;
    }
    if(menu_background_y==UP){
        fake_delta_y=-menu_speed_y;
    }
    else if(menu_background_y==DOWN){
        fake_delta_y=menu_speed_y;
    }

    for(int i=0;i<levels[0].menu_backgrounds.size();i++){
        levels[0].menu_backgrounds[i].update(fake_delta_x,fake_delta_y);
    }
}

void Player::render_background(){
    if(game_in_progress){
        for(int i=0;i<levels[0].background_layers.size();i++){
            levels[0].background_layers[i].render(i);
        }
    }
}

void Player::render_menu_background(){
    if(!game_in_progress){
        for(int i=0;i<levels[0].menu_backgrounds.size();i++){
            levels[0].menu_backgrounds[i].render(i);
        }
    }
    else{
        if(pause || window_manager.which_window_open()!=-1){
            for(int i=0;i<levels[0].menu_backgrounds.size();i++){
                levels[0].menu_backgrounds[i].render(i,0.5);
            }
        }
    }
}

void Player::render_foreground(){
}

void Player::animate_menu_title(){
    if(!menu_title_direction){
        menu_title_opacity-=0.005;
    }
    else{
        menu_title_opacity+=0.005;
    }

    if(menu_title_opacity<=0.2){
        menu_title_opacity=0.2;
        menu_title_direction=!menu_title_direction;
    }
    else if(menu_title_opacity>=1.0){
        menu_title_opacity=1.0;
        menu_title_direction=!menu_title_direction;
    }
}

void Player::save_highscore(){
    //Load the highscores.

    vector<Highscore> highscores;

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

    bool score_inserted=false;

    current_highscore=-1;

    //Look through all of the highscores.
    for(int i=0;i<highscores.size();i++){
        //If the new score is higher than this highscore.
        if(score>highscores[i].score){
            highscores.insert(highscores.begin()+i,Highscore());
            highscores[i].score=score;
            highscores[i].name=name;
            score_inserted=true;

            current_highscore=i;
            break;
        }
    }

    if(!score_inserted){
        highscores.push_back(Highscore());
        highscores[highscores.size()-1].score=score;
        highscores[highscores.size()-1].name=name;

        current_highscore=highscores.size()-1;
    }

    if(current_highscore>=10){
        current_highscore=-1;
    }

    //Trim the highscores list to 10 entries.
    while(highscores.size()>10){
        highscores.erase(highscores.end());
    }

    ofstream save("highscores",ifstream::out);

    if(save!=NULL){
        for(int i=0;i<highscores.size();i++){
            save<<"<highscore>\n";
            save<<"<score>";
            save<<highscores[i].score;
            save<<"\n<name>";
            save<<highscores[i].name;
            save<<"\n</highscore>\n";
        }
    }
    else{
        fprintf(stderr,"Error saving highscores file.\n");
    }
}

void Player::save_initial_highscores(){
    //Load the highscores.

    vector<Highscore> highscores;

    ifstream load("highscores",ifstream::in);

    if(load!=NULL){
        load.close();
        load.clear();
    }
    else{
        fprintf(stdout,"Creating highscores file.\n");

        for(int i=0;i<10;i++){
            highscores.push_back(Highscore());
            highscores[highscores.size()-1].score=100000-i*10000;

            if(i==0){
                ///Hubert from Hubert's Island
                highscores[highscores.size()-1].name="Hubert";
            }
            else if(i==1){
                ///Commander Keen from... Commander Keen
                highscores[highscores.size()-1].name="Billy";
            }
            else if(i==2){
                ///Ben Linus from LOST
                highscores[highscores.size()-1].name="Ben";
            }
            else if(i==3){
                ///Pajama Sam from his eponymous titles
                highscores[highscores.size()-1].name="Sam";
            }
            else if(i==4){
                ///Luke Skywalker from Star Wars
                highscores[highscores.size()-1].name="Luke";
            }
            else if(i==5){
                ///Ender Wiggen from Ender's Game
                highscores[highscores.size()-1].name="Ender";
            }
            else if(i==6){
                ///Freddi Fish is the eponymous fish in several games
                highscores[highscores.size()-1].name="Freddi";
            }
            else if(i==7){
                ///Luther from Freddi's games
                highscores[highscores.size()-1].name="Luther";
            }
            else if(i==8){
                ///Jules Verne
                highscores[highscores.size()-1].name="Jules";
            }
            else if(i==9){
                ///Dopefish lives!
                highscores[highscores.size()-1].name="Dopefish";
            }
        }

        ofstream save("highscores",ifstream::out);

        if(save!=NULL){
            for(int i=0;i<highscores.size();i++){
                save<<"<highscore>\n";
                save<<"<score>";
                save<<highscores[i].score;
                save<<"\n<name>";
                save<<highscores[i].name;
                save<<"\n</highscore>\n";
            }
        }
        else{
            fprintf(stderr,"Error saving highscores file.\n");
        }
    }
}
