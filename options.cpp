/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "options.h"
#include "world.h"

using namespace std;

bool load_current_profile(){
    ifstream load;
    string file_to_load="profiles/current_profile.cfg";
    load.open(file_to_load.c_str(),ifstream::in);

    if(load!=NULL){
        load>>player.name;

        if(load.fail()){
            fprintf(stderr,"Failed to load 'current_profile.cfg.'\n");
            load.close();
            load.clear();
            return false;
        }
        load.close();
        load.clear();
    }
    else{
        //Save options to options.cfg.
        if(!save_current_profile()){
            fprintf(stderr,"Failed to load 'current_profile.cfg.'\n");
            return false;
        }
    }

    return true;
}

bool save_current_profile(){
    //If the player's name is set to something, we know at least one profile exists,
    //and we can save safely.
    if(player.name!="\x1F"){
        profile.make_directories();
        ofstream save;
        string save_name="profiles/current_profile.cfg";
        save.open(save_name.c_str());

        save<<player.name;

        if(save.fail()){
            fprintf(stderr,"Failed to save 'current_profile.cfg.'\n");
            save.close();
            save.clear();
            return false;
        }

        save.close();
        save.clear();
    }
    //If the player's name is not set, no profile exists.
    else{
        fprintf(stdout,"No profile exists.\nPrompting the player to create one...\n");
        return false;
    }

    return true;
}

bool options_load(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        ifstream load;
        string file_to_load="profiles/";
        file_to_load+=player.name;
        file_to_load+="/options.cfg";
        load.open(file_to_load.c_str(),ifstream::in);

        if(load!=NULL){
            int number_of_commands=0;

            //Temporary variables for loading command data.
            short type;
            int key;
            unsigned int which_joystick;
            unsigned int joy_button;
            unsigned int joy_axis;
            bool joy_axis_direction;
            unsigned int joy_hat;
            short joy_hat_direction;
            unsigned int joy_ball;
            short joy_ball_direction;

            load>>player.option_fullscreen;
            load>>player.option_fps;
            load>>player.option_dev;
            load>>player.option_effect_limit;
            load>>player.option_volume_global;
            load>>player.option_mute_global;
            load>>player.option_volume_sound;
            load>>player.option_mute_sound;
            load>>player.option_volume_music;
            load>>player.option_mute_music;
            load>>player.option_resolution_x;
            load>>player.option_resolution_y;
            load>>number_of_commands;

            for(int i=0;i<number_of_commands;i++){
                load>>type;
                load>>key;
                load>>which_joystick;
                load>>joy_button;
                load>>joy_axis;
                load>>joy_axis_direction;
                load>>joy_hat;
                load>>joy_hat_direction;
                load>>joy_ball;
                load>>joy_ball_direction;

                player.keys[i].type=type;
                player.keys[i].key=(SDLKey)key;
                player.keys[i].which_joystick=(Uint8)which_joystick;
                player.keys[i].joy_button=(Uint8)joy_button;
                player.keys[i].joy_axis=(Uint8)joy_axis;
                player.keys[i].joy_axis_direction=joy_axis_direction;
                player.keys[i].joy_hat=(Uint8)joy_hat;
                player.keys[i].joy_hat_direction=joy_hat_direction;
                player.keys[i].joy_ball=(Uint8)joy_ball;
                player.keys[i].joy_ball_direction=joy_ball_direction;
            }

            if(load.fail()){
                fprintf(stderr,"Failed to load 'options.cfg.'\n");
                load.close();
                load.clear();
                return false;
            }
            load.close();
            load.clear();
        }
        else{
            //Save options to options.cfg.
            if(!options_save()){
                fprintf(stderr,"Failed to load 'options.cfg.'\n");
                return false;
            }
        }

        player.camera_w=player.option_resolution_x;
        player.camera_h=player.option_resolution_y;

        return true;
    }
}

bool options_save(){
    //If there is no profile.
    if(player.name=="\x1F"){
        return false;
    }
    //If there is a profile.
    else{
        profile.make_directories();
        ofstream save;

        string save_name="profiles/";
        save_name+=player.name;
        save_name+="/options.cfg";
        save.open(save_name.c_str());

        save<<player.option_fullscreen;
        save<<"\n";
        save<<player.option_fps;
        save<<"\n";
        save<<player.option_dev;
        save<<"\n";
        save<<player.option_effect_limit;
        save<<"\n";
        save<<player.option_volume_global;
        save<<"\n";
        save<<player.option_mute_global;
        save<<"\n";
        save<<player.option_volume_sound;
        save<<"\n";
        save<<player.option_mute_sound;
        save<<"\n";
        save<<player.option_volume_music;
        save<<"\n";
        save<<player.option_mute_music;
        save<<"\n";
        save<<player.option_resolution_x;
        save<<"\n";
        save<<player.option_resolution_y;
        save<<"\n";
        save<<player.keys.size();
        save<<"\n";

        for(int i=0;i<player.keys.size();i++){
            save<<player.keys[i].type;
            save<<"\n";
            save<<(int)player.keys[i].key;
            save<<"\n";
            save<<(unsigned int)player.keys[i].which_joystick;
            save<<"\n";
            save<<(unsigned int)player.keys[i].joy_button;
            save<<"\n";
            save<<(unsigned int)player.keys[i].joy_axis;
            save<<"\n";
            save<<player.keys[i].joy_axis_direction;
            save<<"\n";
            save<<(unsigned int)player.keys[i].joy_hat;
            save<<"\n";
            save<<player.keys[i].joy_hat_direction;
            save<<"\n";
            save<<(unsigned int)player.keys[i].joy_ball;
            save<<"\n";
            save<<player.keys[i].joy_ball_direction;
            save<<"\n";
        }

        if(save.fail()){
            fprintf(stderr,"Failed to save 'options.cfg.'\n");
            save.close();
            save.clear();
            return false;
        }

        save.close();
        save.clear();

        return true;
    }
}
