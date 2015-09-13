/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef window_highscores_h
#define window_highscores_h

#include "window.h"
#include "highscore.h"

class Window_Highscores: public Window{
    private:
    public:

    std::vector<Highscore> highscores;

    Window_Highscores(short get_x,short get_y,short get_w,short get_h);

    void update_highscores();

    void toggle_on();

    void turn_off();

    void handle_input_states();

    void handle_input_events();

    void render();
};

#endif
