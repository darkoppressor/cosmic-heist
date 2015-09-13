/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef update_h
#define update_h

#include <SDL.h>

void input();

void movement();

void events();

void animation();

void camera(int frame_rate,double ms_per_frame,int logic_frame_rate);

void render(int frame_rate,double ms_per_frame,int logic_frame_rate);

#endif
