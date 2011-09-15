/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef button_events_h
#define button_events_h

#include "window.h"

//Open the corresponding window.
void button_event_open_window_main_menu(Window* parent_window,int special_data);
void button_event_open_window_create_profile(Window* parent_window,int special_data);
void button_event_open_window_change_profile(Window* parent_window,int special_data);
void button_event_open_window_options(Window* parent_window,int special_data);
void button_event_open_window_options_keys(Window* parent_window,int special_data);
void button_event_open_window_about(Window* parent_window,int special_data);
void button_event_open_window_credits(Window* parent_window,int special_data);
void button_event_open_window_help(Window* parent_window,int special_data);

void button_event_open_window_highscores(Window* parent_window,int special_data);

//Change the corresponding option.
void button_event_option_fullscreen(Window* parent_window,int special_data);
void button_event_option_fps(Window* parent_window,int special_data);
void button_event_option_mute_global(Window* parent_window,int special_data);
void button_event_option_mute_sound(Window* parent_window,int special_data);
void button_event_option_mute_music(Window* parent_window,int special_data);
void button_event_option_volume_up_global(Window* parent_window,int special_data);
void button_event_option_volume_down_global(Window* parent_window,int special_data);
void button_event_option_volume_up_sound(Window* parent_window,int special_data);
void button_event_option_volume_down_sound(Window* parent_window,int special_data);
void button_event_option_volume_up_music(Window* parent_window,int special_data);
void button_event_option_volume_down_music(Window* parent_window,int special_data);

//Change key.
void button_event_key_toggle_main_menu(Window* parent_window,int special_data);
void button_event_key_screenshot(Window* parent_window,int special_data);
void button_event_key_left(Window* parent_window,int special_data);
void button_event_key_up(Window* parent_window,int special_data);
void button_event_key_right(Window* parent_window,int special_data);
void button_event_key_down(Window* parent_window,int special_data);
void button_event_key_pause(Window* parent_window,int special_data);

//Create profile.
void button_event_create_profile(Window* parent_window,int special_data);

//Select this profile.
void button_event_select_profile(Window* parent_window,int special_data);

//Delete this profile.
void button_event_delete_profile(Window* parent_window,int special_data);

//Start or resume the game.
void button_event_start_game(Window* parent_window,int special_data);

//Quit the game.
void button_event_quit(Window* parent_window,int special_data);

//Close the parent window.
void button_event_close_window(Window* parent_window,int special_data);

//Do nothing.
//This is just a placeholder function.
void button_event_do_nothing(Window* parent_window,int special_data);

#endif
