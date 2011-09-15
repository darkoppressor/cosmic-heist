/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef tooltip_h
#define tooltip_h

#include <SDL.h>

#include <string>

class Tooltip{
    private:
    public:
    Tooltip();

    void setup(std::string get_message,int mouse_x,int mouse_y);

    void set_dimensions();

    void render();

    bool on;

    std::string message;

    short x,y;

    short w,h;
};

#endif
