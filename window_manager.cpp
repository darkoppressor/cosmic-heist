/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "window_manager.h"
#include "world.h"

using namespace std;

void Window_Manager::close_windows(Window* window_to_keep){
    //Clear the profile creation string.
    creating_profile.clear();

    //The player's input events should no longer be blocked.
    player.block_input_events=false;

    //As long as the window is not to be kept, close it.
    if(window_to_keep!=&window_highscores[0]){
        window_highscores[0].turn_off();
    }

    for(int i=0;i<vector_windows.size();i++){
        //As long as the current window is not to be kept, close it.
        if(window_to_keep!=&vector_windows[i]){
            vector_windows[i].turn_off();
        }
    }
}

short Window_Manager::which_window_open(){
    if(window_highscores[0].return_on()){
        return WHICH_WINDOW_HIGHSCORES;
    }

    for(int i=WINDOW_BEGIN;i<WINDOW_END;i++){
        if(vector_windows[i].return_on()){
            return WHICH_WINDOW_OTHER+i;
        }
    }

    return -1;
}
