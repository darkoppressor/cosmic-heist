/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef world_h
#define world_h

#include <string>
#include <sstream>
#include <vector>

#include "level.h"
#include "font.h"
#include "game_window.h"
#include "random_number_generator.h"
#include "image.h"
#include "tooltip.h"
#include "enumerations.h"
#include "music.h"
#include "player.h"
#include "profile.h"
#include "window.h"
#include "window_manager.h"
#include "window_highscores.h"
#include "ship.h"
#include "exit.h"
#include "explosion.h"

extern Game_Window main_window;

extern std::vector<Window> vector_windows;

extern std::vector<Window_Highscores> window_highscores;

extern Window_Manager window_manager;

extern std::vector<Button> hud_buttons;

extern Tooltip tooltip;

extern SDL_Event event;

struct joy_stick{
    SDL_Joystick* joy;
};
//Vector containing all of the joysticks.
extern std::vector<joy_stick> joystick;

//The number of joysticks connected to the system when the joysticks were loaded up.
//I don't know if SDL_NumJoysticks() might change during runtime if the player disconnects one or more joysticks (or connects one or more new ones).
//Thus, it is saved here right after the joysticks are loaded, and this number is used when unloading them in quit().
extern int number_of_joysticks;

extern BitmapFont font;

extern Random_Number_Generator rng;

extern std::stringstream ss;
extern std::string msg;

extern double UPDATE_RATE;
extern double SKIP_TICKS;

extern Player player;

extern Profile profile;

extern std::string creating_profile;

extern std::vector<Level> levels;

extern std::vector<Ship> ships;

extern std::vector<Exit> exits;

extern std::vector<Explosion> explosions;

extern std::vector<Ship> player_ship;

extern Image image;

extern Music music;

extern Mix_Chunk* sound_pause;
extern Mix_Chunk* sound_unpause;
extern Mix_Chunk* sound_explosion_1;
extern Mix_Chunk* sound_explosion_2;
extern Mix_Chunk* sound_explosion_3;
extern Mix_Chunk* sound_level_start;
extern Mix_Chunk* sound_game_over;
extern Mix_Chunk* sound_engines;

extern std::vector<Mix_Chunk*> sound_button_mouse_over;
extern std::vector<Mix_Chunk*> sound_button_event_fire;

void load_world_initial_1();
void load_world_initial_2();

void load_world();

void unload_world();

void set_change_profile_buttons();

void create_windows();

void create_hud_buttons();

#endif
