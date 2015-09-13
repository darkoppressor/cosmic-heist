/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "input_data.h"
#include "world.h"

using namespace std;

Input_Data::Input_Data(SDLKey get_key){
    type=INPUT_TYPE_KEYBOARD;

    key=get_key;

    which_joystick=NULL;

    joy_button=NULL;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=NULL;

    joy_ball_direction=NONE;
}

Input_Data::Input_Data(){
    type=INPUT_TYPE_KEYBOARD;

    key=SDLK_FIRST;

    which_joystick=NULL;

    joy_button=NULL;

    joy_axis=NULL;

    joy_axis_direction=false;

    joy_hat=NULL;

    joy_hat_direction=SDL_HAT_CENTERED;

    joy_ball=NULL;

    joy_ball_direction=NONE;
}
