/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "player.h"
#include "world.h"
#include "options.h"
#include "quit.h"

using namespace std;

void Player::toggle_fullscreen(){
    option_fullscreen=!option_fullscreen;
    options_save();

    main_window.toggle_fullscreen();
}

void Player::toggle_fps(){
    option_fps=!option_fps;
    options_save();
}

void Player::toggle_mute_global(){
    option_mute_global=!option_mute_global;
    options_save();
}

void Player::toggle_mute_sound(){
    option_mute_sound=!option_mute_sound;
    options_save();
}

void Player::toggle_mute_music(){
    option_mute_music=!option_mute_music;
    options_save();
}

void Player::volume_up_global(){
    option_volume_global+=0.1;

    if(option_volume_global>1.0){
        option_volume_global=1.0;
    }

    options_save();
}

void Player::volume_down_global(){
    option_volume_global-=0.1;

    if(option_volume_global<0.0){
        option_volume_global=0.0;
    }

    options_save();
}

void Player::volume_up_sound(){
    option_volume_sound+=0.1;

    if(option_volume_sound>1.0){
        option_volume_sound=1.0;
    }

    options_save();
}

void Player::volume_down_sound(){
    option_volume_sound-=0.1;

    if(option_volume_sound<0.0){
        option_volume_sound=0.0;
    }

    options_save();
}

void Player::volume_up_music(){
    option_volume_music+=0.1;

    if(option_volume_music>1.0){
        option_volume_music=1.0;
    }

    options_save();
}

void Player::volume_down_music(){
    option_volume_music-=0.1;

    if(option_volume_music<0.0){
        option_volume_music=0.0;
    }

    options_save();
}

void Player::begin_setting_command(short command){
    command_to_set=command;
    input_already_bound=-1;
}

bool Player::check_key(Input_Data checked_key){
    //Check through all of the commands.
    for(int i=0;i<keys.size();i++){
        //If the current command has the same input type as the checked command, and if the command is not the one being set.
        //We do the second check so if a command is set to enter, and we are setting that command, enter will not count as taken.
        if(keys[i].type==checked_key.type && i!=command_to_set){
            switch(checked_key.type){

            case INPUT_TYPE_KEYBOARD:
                if(keys[i].key==checked_key.key){
                    input_already_bound=i;
                    return true;
                }
                break;

            case INPUT_TYPE_JOYSTICK_BUTTON:
                if(keys[i].which_joystick==checked_key.which_joystick && keys[i].joy_button==checked_key.joy_button){
                    input_already_bound=i;
                    return true;
                }
                break;

            case INPUT_TYPE_JOYSTICK_AXIS:
                if(keys[i].which_joystick==checked_key.which_joystick && keys[i].joy_axis==checked_key.joy_axis && keys[i].joy_axis_direction==checked_key.joy_axis_direction){
                    input_already_bound=i;
                    return true;
                }
                break;

            case INPUT_TYPE_JOYSTICK_HAT:
                if(keys[i].which_joystick==checked_key.which_joystick && keys[i].joy_hat==checked_key.joy_hat && keys[i].joy_hat_direction==checked_key.joy_hat_direction){
                    input_already_bound=i;
                    return true;
                }
                break;

            case INPUT_TYPE_JOYSTICK_BALL:
                if(keys[i].which_joystick==checked_key.which_joystick && keys[i].joy_ball==checked_key.joy_ball && keys[i].joy_ball_direction==checked_key.joy_ball_direction){
                    input_already_bound=i;
                    return true;
                }
                break;

            }
        }
    }

    return false;
}

void Player::check_for_command_set(){
    //If we are currently waiting for a key to be hit.
    if(command_to_set!=-1){

        Input_Data key_to_check;

        switch(event.type){
            case SDL_QUIT:
                quit_game();
                break;

            case SDL_KEYDOWN:
                //Here I can place any keys that should not be available for binding.
                if(event.key.keysym.sym==SDLK_ESCAPE){
                }
                //Otherwise, assign the new key to the command.
                else{
                    key_to_check.type=INPUT_TYPE_KEYBOARD;
                    key_to_check.key=event.key.keysym.sym;

                    //If the key is not already bound to a command.
                    if(!check_key(key_to_check)){
                        keys[command_to_set].type=key_to_check.type;
                        keys[command_to_set].key=key_to_check.key;
                        command_to_set=-1;
                        options_save();
                    }
                }
                break;

            case SDL_JOYBUTTONDOWN:
                key_to_check.type=INPUT_TYPE_JOYSTICK_BUTTON;
                key_to_check.which_joystick=event.jbutton.which;
                key_to_check.joy_button=event.jbutton.button;

                //If the joystick button is not already bound to a command.
                if(!check_key(key_to_check)){
                    keys[command_to_set].type=key_to_check.type;
                    keys[command_to_set].which_joystick=key_to_check.which_joystick;
                    keys[command_to_set].joy_button=key_to_check.joy_button;
                    command_to_set=-1;
                    options_save();
                }
                break;

            case SDL_JOYAXISMOTION:
                //If the axis is outside the neutral zone and held to the negative direction.
                if(event.jaxis.value<JOYSTICK_NEUTRAL_NEGATIVE){
                    key_to_check.type=INPUT_TYPE_JOYSTICK_AXIS;
                    key_to_check.which_joystick=event.jaxis.which;
                    key_to_check.joy_axis=event.jaxis.axis;
                    key_to_check.joy_axis_direction=false;

                    //If the joystick axis is not already bound to a command.
                    if(!check_key(key_to_check)){
                        keys[command_to_set].type=key_to_check.type;
                        keys[command_to_set].which_joystick=key_to_check.which_joystick;
                        keys[command_to_set].joy_axis=key_to_check.joy_axis;
                        keys[command_to_set].joy_axis_direction=key_to_check.joy_axis_direction;
                        command_to_set=-1;
                        options_save();
                    }
                }
                //If the axis is outside the neutral zone and held to the positive direction.
                else if(event.jaxis.value>JOYSTICK_NEUTRAL_POSITIVE){
                    key_to_check.type=INPUT_TYPE_JOYSTICK_AXIS;
                    key_to_check.which_joystick=event.jaxis.which;
                    key_to_check.joy_axis=event.jaxis.axis;
                    key_to_check.joy_axis_direction=true;

                    //If the joystick axis is not already bound to a command.
                    if(!check_key(key_to_check)){
                        keys[command_to_set].type=key_to_check.type;
                        keys[command_to_set].which_joystick=key_to_check.which_joystick;
                        keys[command_to_set].joy_axis=key_to_check.joy_axis;
                        keys[command_to_set].joy_axis_direction=key_to_check.joy_axis_direction;
                        command_to_set=-1;
                        options_save();
                    }
                }
                break;

            case SDL_JOYHATMOTION:
                //If the hat is not centered.
                if(event.jhat.value!=SDL_HAT_CENTERED){
                    key_to_check.type=INPUT_TYPE_JOYSTICK_HAT;
                    key_to_check.which_joystick=event.jhat.which;
                    key_to_check.joy_hat=event.jhat.hat;
                    key_to_check.joy_hat_direction=event.jhat.value;

                    //If the joystick hat is not already bound to a command.
                    if(!check_key(key_to_check)){
                        keys[command_to_set].type=key_to_check.type;
                        keys[command_to_set].which_joystick=key_to_check.which_joystick;
                        keys[command_to_set].joy_hat=key_to_check.joy_hat;
                        keys[command_to_set].joy_hat_direction=key_to_check.joy_hat_direction;
                        command_to_set=-1;
                        options_save();
                    }
                }
                break;

            case SDL_JOYBALLMOTION:
                key_to_check.type=INPUT_TYPE_JOYSTICK_BALL;
                key_to_check.which_joystick=event.jball.which;
                key_to_check.joy_ball=event.jball.ball;
                ///I don't know if this is correct.
                ///I am assuming that on the x-axis, negative relative motion means left and positive relative motion means right.
                ///Ditto for the y-axis.
                if(event.jball.xrel<0 && event.jball.yrel<0){
                    key_to_check.joy_ball_direction=LEFT_UP;
                }
                else if(event.jball.xrel<0 && event.jball.yrel>0){
                    key_to_check.joy_ball_direction=LEFT_DOWN;
                }
                else if(event.jball.xrel>0 && event.jball.yrel<0){
                    key_to_check.joy_ball_direction=RIGHT_UP;
                }
                else if(event.jball.xrel>0 && event.jball.yrel>0){
                    key_to_check.joy_ball_direction=RIGHT_DOWN;
                }
                else if(event.jball.xrel<0){
                    key_to_check.joy_ball_direction=LEFT;
                }
                else if(event.jball.xrel>0){
                    key_to_check.joy_ball_direction=RIGHT;
                }
                else if(event.jball.yrel<0){
                    key_to_check.joy_ball_direction=UP;
                }
                else if(event.jball.yrel>0){
                    key_to_check.joy_ball_direction=DOWN;
                }

                //If the joystick ball is not already bound to a command.
                if(!check_key(key_to_check)){
                    keys[command_to_set].type=key_to_check.type;
                    keys[command_to_set].which_joystick=key_to_check.which_joystick;
                    keys[command_to_set].joy_ball=key_to_check.joy_ball;
                    keys[command_to_set].joy_ball_direction=key_to_check.joy_ball_direction;
                    command_to_set=-1;
                    options_save();
                }
                break;
        }
    }
}

short Player::return_input_already_bound(){
    return input_already_bound;
}

short Player::current_command_to_set(){
    return command_to_set;
}
