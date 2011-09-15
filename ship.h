/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef ship_h
#define ship_h

#include "sprite_sheets.h"

#include <string>

class Ship{
    private:

    //Virtual AI "keystates".
    bool ai_keystates[256];

    public:

    bool is_player;

    bool alive;

    double x,y;

    double health;

    double angle;

    double move_speed_x;
    double move_speed_y;

    short move_type;

    short accel_type;

    short type;

    short collision_size;

    //If true, the engines are idle.
    bool idle;

    //Is the current direction currently being held?
    bool directions[4];

    //Animation frames and frame counters.
    short frame;
    short frame_counter;

    short frame_idle;
    short frame_counter_idle;

    short counter_engines;

    Ship(double get_x,double get_y,short get_type,bool get_is_player);

    double return_w(bool collision_adjust=false);
    double return_h(bool collision_adjust=false);

    void handle_ai();

    void handle_input_ai();

    void handle_input_player();

    void move();

    void handle_events();

    void handle_damage(double object_speed);

    void animate();

    void render();
};

#endif
