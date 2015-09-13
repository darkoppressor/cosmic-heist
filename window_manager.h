/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef window_manager_h
#define window_manager_h

#include "window.h"

class Window_Manager{
    public:

    void close_windows(Window* window_to_keep=0);

    //Returns -1 if no window is open.
    //Returns a number representing the window that is open otherwise.
    short which_window_open();
};

#endif
