/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "button_events.h"
#include "world.h"
#include "quit.h"

using namespace std;

//****************//
// Open a window: //
//****************//

void button_event_open_window_main_menu(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_MAIN_MENU].turn_on();
}

void button_event_open_window_create_profile(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    //Begin blocking the player's input events.
    //This will prevent the player from triggering some input event (such as opening a window) when
    //typing the new profile name.
    player.block_input_events=true;

    vector_windows[WINDOW_CREATE_PROFILE].turn_on();
}

void button_event_open_window_change_profile(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    set_change_profile_buttons();

    //If there is a profile.
    if(player.name!="\x1F"){
        vector_windows[WINDOW_CHANGE_PROFILE].turn_on();
    }
    else{
        vector_windows[WINDOW_CREATE_PROFILE].turn_on();
    }
}

void button_event_open_window_options(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_OPTIONS].turn_on();
}

void button_event_open_window_options_keys(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    //Begin blocking the player's input events.
    //This will prevent the player from triggering some input event (such as opening a window) when
    //setting an input.
    player.block_input_events=true;

    vector_windows[WINDOW_OPTIONS_KEYS].turn_on();
}

void button_event_open_window_about(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_ABOUT].turn_on();
}

void button_event_open_window_credits(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_CREDITS].turn_on();
}

void button_event_open_window_help(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    vector_windows[WINDOW_HELP].turn_on();
}

void button_event_open_window_highscores(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    window_highscores[0].update_highscores();

    window_highscores[0].turn_on();
}

//*******************//
// Change an option: //
//*******************//

void button_event_option_fullscreen(Window* parent_window,int special_data){
    player.toggle_fullscreen();
}

void button_event_option_fps(Window* parent_window,int special_data){
    player.toggle_fps();
}

void button_event_option_mute_global(Window* parent_window,int special_data){
    player.toggle_mute_global();
    music.set_track_volumes();
}

void button_event_option_mute_sound(Window* parent_window,int special_data){
    player.toggle_mute_sound();
}

void button_event_option_mute_music(Window* parent_window,int special_data){
    player.toggle_mute_music();
    music.set_track_volumes();
}

void button_event_option_volume_up_global(Window* parent_window,int special_data){
    player.volume_up_global();
    music.set_track_volumes();
}

void button_event_option_volume_down_global(Window* parent_window,int special_data){
    player.volume_down_global();
    music.set_track_volumes();
}

void button_event_option_volume_up_sound(Window* parent_window,int special_data){
    player.volume_up_sound();
}

void button_event_option_volume_down_sound(Window* parent_window,int special_data){
    player.volume_down_sound();
}

void button_event_option_volume_up_music(Window* parent_window,int special_data){
    player.volume_up_music();
    music.set_track_volumes();
}

void button_event_option_volume_down_music(Window* parent_window,int special_data){
    player.volume_down_music();
    music.set_track_volumes();
}

//************************************//
// Change the key bound to a command: //
//************************************//

void button_event_key_toggle_main_menu(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_TOGGLE_MAIN_MENU);
}

void button_event_key_screenshot(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_SCREENSHOT);
}

void button_event_key_left(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_LEFT);
}

void button_event_key_up(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_UP);
}

void button_event_key_right(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_RIGHT);
}

void button_event_key_down(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_DOWN);
}

void button_event_key_pause(Window* parent_window,int special_data){
    player.begin_setting_command(COMMAND_PAUSE);
}

//**************//
// Other Stuff: //
//**************//

void button_event_create_profile(Window* parent_window,int special_data){
    profile.create_profile();
}

void button_event_select_profile(Window* parent_window,int special_data){
    profile.select_profile(special_data);
}

void button_event_delete_profile(Window* parent_window,int special_data){
    profile.delete_profile(special_data);
}

void button_event_start_game(Window* parent_window,int special_data){
    //Close windows.
    window_manager.close_windows(0);

    //Start/resume the game.
    player.start_game();
}

void button_event_quit(Window* parent_window,int special_data){
    quit_game();
}

void button_event_close_window(Window* parent_window,int special_data){
    parent_window->turn_off();

    //If a game is not in progress.
    if(!player.game_in_progress){
        //If a profile exists.
        if(player.name!="\x1F"){
            vector_windows[WINDOW_MAIN_MENU].turn_on();
        }
        //If no profile exists.
        else{
            //Keep the Create Profile window open.
            vector_windows[WINDOW_CREATE_PROFILE].turn_on();
        }
    }
}

void button_event_do_nothing(Window* parent_window,int special_data){
}
