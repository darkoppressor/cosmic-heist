/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef explosion_h
#define explosion_h

#include "sprite_sheets.h"

#include <string>

class Explosion{
    private:
    public:

    double x,y;

    double scale;

    short frame;
    short frame_counter;

    bool exists;

    Explosion(double get_x,double get_y,double get_scale,short get_type);

    void animate();

    void render();
};

#endif
