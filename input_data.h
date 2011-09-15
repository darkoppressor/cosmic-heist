/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef input_data_h
#define input_data_h

#include <SDL.h>

class Input_Data{
    private:
    public:
    Input_Data(SDLKey get_key);
    Input_Data();

    short type;
    SDLKey key;
    Uint8 which_joystick;
    Uint8 joy_button;
    Uint8 joy_axis;
    bool joy_axis_direction;
    Uint8 joy_hat;
    short joy_hat_direction;
    Uint8 joy_ball;
    short joy_ball_direction;
};

#endif
