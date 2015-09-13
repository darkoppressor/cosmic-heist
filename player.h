/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef player_h
#define player_h

#include <SDL.h>

#include "timer.h"
#include "sprite_sheets.h"
#include "input_data.h"
#include "enumerations.h"

#include <string>
#include <vector>

struct tile_coords{
    int x;
    int y;
};

class Player{
    private:
    //We will use this to hold the keystates, which will be given to us by SDL.
    Uint8 *keystates;

    //Current movement state of the camera.
    short cam_state;

    public:

    //The id of a command that a checked input is already bound to.
    //When command_to_set is not -1 (currently being set), the player might
    //try to bind an input that is already bound to some other command.
    //If this happens, that command's id is stored in input_already_bound.
    //The corresponding Information object will use this information to let the player
    //know that this input is already bound to something.
    //This will be -1 whenever it is not being used.
    short input_already_bound;

    //If command_to_set is -1, no key is being set.
    //If anything other than -1, the player has clicked on a command in the key config window, and the next key hit should be set to that command.
    short command_to_set;

    //The coordinates and dimensions of the camera.
    double camera_x,camera_y;
    double camera_w,camera_h;

    //The speed at which the camera moves when unstuck.
    short camera_speed;

    double camera_delta_x;
    double camera_delta_y;

    //Menu background stuff
    short menu_background_counter_x;
    short menu_background_counter_y;

    short menu_background_counter_max_x;
    short menu_background_counter_max_y;

    short menu_background_x;
    short menu_background_y;

    double menu_speed_x;
    double menu_speed_y;

    //Menu title stuff
    double menu_title_opacity;
    bool menu_title_direction;

    //If true, the game is paused.
    //If false, the game is not paused.
    bool pause;

    //If true, a game is currently in progress.
    //If false, the game is on the main menu.
    bool game_in_progress;

    //If true, some window is open that needs special input events,
    //so we ignore player input events.
    bool block_input_events;

    //The player's profile name.
    std::string name;

    uint64_t score;

    int current_highscore;

    int difficulty;

    uint64_t existing_explosions;

    //Options:
    bool option_fullscreen;
    bool option_fps;
    bool option_dev;
    int option_effect_limit;
    double option_volume_global;
    bool option_mute_global;
    double option_volume_sound;
    bool option_mute_sound;
    double option_volume_music;
    bool option_mute_music;
    short option_resolution_x;
    short option_resolution_y;

    //Inputs.
    std::vector<Input_Data> keys;

    Player();

    void reset();

    void load_data();

    //Pause or unpause the game.
    //If get_pause is true, pause the game.
    //If get_pause is false, unpause the game.
    void toggle_pause(bool get_pause);

    //Prepare to check for input.
    void prepare_for_input();

    //Input here is handled at all times.
    void handle_input_states_always();

    //Input here is handled any time the game is in progress, even if it is paused.
    void handle_input_states_during_play();

    //Get input states.
    //Only processed when the game is unpaused.
    void handle_input_states();

    //Get input events.
    //Always processed. If an event is supposed to only happen when the game is unpaused, in progress, etc.,
    //this will be handled in handle_command_event().
    void handle_input_events();

    //When a command event occurs, it calls this function to actually execute the command.
    void handle_command_event(short command);

    void animate_menu_title();

    void save_highscore();

    void save_initial_highscores();

    //Handle camera movement.
    void set_camera();

    void update_window_caption(int frame_rate,double ms_per_frame,int logic_frame_rate);

    void update_background();

    //Render the background.
    void render_background();

    void render_menu_background();

    //Render the foreground.
    void render_foreground();

    //Stops the current in-progress game, if any,
    //and resets the player's variables.
    void stop_game();

    void start_game();

    //Option functions:

    //Toggles the fullscreen option.
    void toggle_fullscreen();

    //Toggles the show fps option.
    void toggle_fps();

    //Toggles global mute.
    void toggle_mute_global();

    //Toggles sound effect mute.
    void toggle_mute_sound();

    //Toggles music mute.
    void toggle_mute_music();

    //Increment or decrement the corresponding volume.
    void volume_up_global();
    void volume_down_global();
    void volume_up_sound();
    void volume_down_sound();
    void volume_up_music();
    void volume_down_music();

    //Pass the identifier for the key you want to set.
    //Pass -1 to stop setting any command.
    void begin_setting_command(short key_to_set);

    //Check through all commands to see if the passed input is already bound to something.
    //Returns true if the input is already bound.
    //Returns false if the input is not bound.
    bool check_key(Input_Data checked_key);

    //Handle setting command input events.
    void check_for_command_set();

    //Returns the current value of command_to_set.
    short current_command_to_set();

    //Returns whether or not the passed command is currently being given.
    bool command_state(short command);

    //Returns a string with the name of the input bound to the passed command.
    std::string command_bound_input(short command);

    //Returns the current value of input_already_bound.
    short return_input_already_bound();
};

#endif
