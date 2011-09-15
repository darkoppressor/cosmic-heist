#include "world.h"

#include "sprite_sheets.h"
#include "button_events.h"

using namespace std;

//Create the main window.
Game_Window main_window;

vector<Window> vector_windows;

vector<Window_Highscores> window_highscores;

Window_Manager window_manager;

vector<Button> hud_buttons;

Tooltip tooltip;

Random_Number_Generator rng;

//All of the other variables that are not defined in their own classes are defined here.

//We use this to poll for events when getting input.
SDL_Event event;

vector<joy_stick> joystick;

int number_of_joysticks=0;

//The game's bitmap font.
BitmapFont font;

//String stream used to easily render ints and stuff as strings.
stringstream ss("");

//ss passes its information into this string for output.
string msg="";

double UPDATE_RATE=60.0;
double SKIP_TICKS=1000.0/UPDATE_RATE;

Player player;

Profile profile;

string creating_profile="";

vector<Level> levels;

vector<Ship> ships;

vector<Exit> exits;

vector<Explosion> explosions;

vector<Ship> player_ship;

//Here we define the surface and texture variables:
Image image;

Music music;

//Here we define the sound files.
Mix_Chunk* sound_pause=NULL;
Mix_Chunk* sound_unpause=NULL;
Mix_Chunk* sound_explosion_1=NULL;
Mix_Chunk* sound_explosion_2=NULL;
Mix_Chunk* sound_explosion_3=NULL;
Mix_Chunk* sound_level_start=NULL;
Mix_Chunk* sound_game_over=NULL;
Mix_Chunk* sound_engines=NULL;

vector<Mix_Chunk*> sound_button_mouse_over;
vector<Mix_Chunk*> sound_button_event_fire;

void load_world_initial_1(){
    //Create the background layers.
    levels[0].background_layers.push_back(Background_Layer(player.option_resolution_x,player.option_resolution_y,2.0,2.0));
    levels[0].background_layers.push_back(Background_Layer(player.option_resolution_x,player.option_resolution_y,4.0,4.0));
    levels[0].background_layers.push_back(Background_Layer(player.option_resolution_x,player.option_resolution_y,6.0,6.0));
    levels[0].background_layers.push_back(Background_Layer(player.option_resolution_x,player.option_resolution_y,8.0,8.0));
    levels[0].background_layers.push_back(Background_Layer(player.option_resolution_x,player.option_resolution_y,10.0,10.0));

    levels[0].menu_backgrounds.push_back(Menu_Background(player.option_resolution_x,player.option_resolution_y,2.0,2.0));
    levels[0].menu_backgrounds.push_back(Menu_Background(player.option_resolution_x,player.option_resolution_y,4.0,4.0));
    levels[0].menu_backgrounds.push_back(Menu_Background(player.option_resolution_x,player.option_resolution_y,6.0,6.0));
    levels[0].menu_backgrounds.push_back(Menu_Background(player.option_resolution_x,player.option_resolution_y,8.0,8.0));
    levels[0].menu_backgrounds.push_back(Menu_Background(player.option_resolution_x,player.option_resolution_y,10.0,10.0));
}

void load_world_initial_2(){
    prepare_ship_sizes();

    prepare_ship_sprite_counts();

    generate_sprite_sheets();

    //Create all of the windows and hud buttons.
    create_windows();
    create_hud_buttons();
}

//This function is called at the beginning of the game.
void load_world(){
    //Generate the font.
    font.build_font("data/images/bitmap_font.png",4096,16,11,24);

    //Load the surfaces and textures.
    image.load_images_global();

    //Load the sound effects.
    sound_pause=Mix_LoadWAV("data/sounds/pause.wav");
    sound_unpause=Mix_LoadWAV("data/sounds/unpause.wav");
    sound_explosion_1=Mix_LoadWAV("data/sounds/explosion_1.wav");
    sound_explosion_2=Mix_LoadWAV("data/sounds/explosion_2.wav");
    sound_explosion_3=Mix_LoadWAV("data/sounds/explosion_3.wav");
    sound_level_start=Mix_LoadWAV("data/sounds/level_start.wav");
    sound_game_over=Mix_LoadWAV("data/sounds/game_over.wav");
    sound_engines=Mix_LoadWAV("data/sounds/engines.wav");

    for(int i=0;i<12;i++){
        sound_button_mouse_over.push_back((Mix_Chunk*)&Mix_Chunk());
        msg="data/sounds/buttons/";
        ///Here, I can add different button sound themes.
        msg+="normal";
        msg+="/mouse_over/";
        ss.clear();ss.str("");ss<<i+1;msg+=ss.str();
        msg+=".wav";
        sound_button_mouse_over[i]=Mix_LoadWAV(msg.c_str());
    }
    for(int i=0;i<12;i++){
        sound_button_event_fire.push_back((Mix_Chunk*)&Mix_Chunk());
        msg="data/sounds/buttons/";
        ///Here, I can add different button sound themes.
        msg+="normal";
        msg+="/event_fire/";
        ss.clear();ss.str("");ss<<i+1;msg+=ss.str();
        msg+=".wav";
        sound_button_event_fire[i]=Mix_LoadWAV(msg.c_str());
    }
}

//This is called in quit_game(). It frees all of the memory allocated in load_world().
void unload_world(){
    //Unload the font.
    font.unload_font();

    //Delete the surfaces and textures:
    image.unload_images_global();

    //Free the sound effects.
    Mix_FreeChunk(sound_pause);
    Mix_FreeChunk(sound_unpause);
    Mix_FreeChunk(sound_explosion_1);
    Mix_FreeChunk(sound_explosion_2);
    Mix_FreeChunk(sound_explosion_3);
    Mix_FreeChunk(sound_level_start);
    Mix_FreeChunk(sound_game_over);
    Mix_FreeChunk(sound_engines);

    for(int i=0;i<12;i++){
        Mix_FreeChunk(sound_button_mouse_over[i]);
    }
    for(int i=0;i<12;i++){
        Mix_FreeChunk(sound_button_event_fire[i]);
    }
    sound_button_mouse_over.clear();
    sound_button_event_fire.clear();
}

void set_change_profile_buttons(){
    short button_sound=0;

    vector_windows[WINDOW_CHANGE_PROFILE].clear_buttons();
    vector_windows[WINDOW_CHANGE_PROFILE].clear_information();

    //Create the close button.
    vector_windows[WINDOW_CHANGE_PROFILE].create_button(360-23,5,"","X",&button_event_close_window,0,0,BUTTON_VISIBLE,-1);

    vector_windows[WINDOW_CHANGE_PROFILE].create_information((360-(16*12))/2,40,"","",0,SPECIAL_INFO_CURRENT_PROFILE);

    for(int i=0;i<profile.profile_list.size();i++){
        vector_windows[WINDOW_CHANGE_PROFILE].create_information(15,105+55*i,"","",0,SPECIAL_INFO_PROFILE_1+i);

        msg="Select Profile";
        button_sound++;
        if(button_sound>11){
            button_sound=0;
        }
        vector_windows[WINDOW_CHANGE_PROFILE].create_button(15,130+55*i,"",msg,&button_event_select_profile,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE,i);

        msg="Delete Profile";
        button_sound++;
        if(button_sound>11){
            button_sound=0;
        }
        vector_windows[WINDOW_CHANGE_PROFILE].create_button(185,130+55*i,"",msg,&button_event_delete_profile,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE,i);
    }

    msg="Create New Profile";
    button_sound++;
    if(button_sound>11){
        button_sound=0;
    }
    vector_windows[WINDOW_CHANGE_PROFILE].create_button((360-(msg.length()*12))/2,526,"",msg,&button_event_open_window_create_profile,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound++;
    if(button_sound>11){
        button_sound=0;
    }
    vector_windows[WINDOW_CHANGE_PROFILE].create_button((360-(msg.length()*12))/2,vector_windows[WINDOW_CHANGE_PROFILE].return_h()-37,"",msg,&button_event_open_window_main_menu,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
}

void create_hud_buttons(){
    short button_sound=0;

    hud_buttons.clear();

    ///button_sound=7-1;
    ///hud_buttons.push_back(Button(376,10,"","    \xA    ",&button_event_open_window_crop_to_plant,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE));
}

void create_windows(){
    short button_sound=0;

    window_highscores.clear();
    window_highscores.push_back(Window_Highscores((player.option_resolution_x-375)/2,(player.option_resolution_y-455)/2,375,455));

    msg="Back to Main Menu";
    button_sound=11-1;
    window_highscores[0].create_button((375-(msg.length()*12))/2,window_highscores[0].return_h()-37,"",msg,&button_event_open_window_main_menu,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);

    vector_windows.clear();

    //Used to position stuff in windows more easily.
    short y_position=0;

    //************//
    // Main Menu: //
    //************//

    vector_windows.push_back(Window((player.option_resolution_x-300)/2,(player.option_resolution_y-350)/2,300,350,"Main Menu",NO_SPECIAL_INPUT));

    vector_windows[vector_windows.size()-1].create_information((300-(16*12))/2,40,"","",0,SPECIAL_INFO_CURRENT_PROFILE);

    msg="Change Profile";
    button_sound=7-1;
    y_position=90;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,y_position,"",msg,&button_event_open_window_change_profile,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="Play";
    button_sound=11-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,y_position,"",msg,&button_event_start_game,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="Highscores";
    button_sound=9-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,y_position,"",msg,&button_event_open_window_highscores,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="Options";
    button_sound=7-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,y_position,"",msg,&button_event_open_window_options,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="Help";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,y_position,"",msg,&button_event_open_window_help,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="About";
    button_sound=12-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,y_position,"",msg,&button_event_open_window_about,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="Credits";
    button_sound=10-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,y_position,"",msg,&button_event_open_window_credits,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="Quit";
    button_sound=8-1;
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,y_position,"",msg,&button_event_quit,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);

    //*****************//
    // Create Profile: //
    //*****************//

    vector_windows.push_back(Window((player.option_resolution_x-300)/2,(player.option_resolution_y-400)/2,300,400,"Create Profile",SPECIAL_INPUT_CREATE_PROFILE));

    vector_windows[vector_windows.size()-1].create_information(15,40,"","",0,SPECIAL_INFO_NO_PROFILE);

    vector_windows[vector_windows.size()-1].create_information(15,65,"","",0,SPECIAL_INFO_PROFILE_TAKEN);

    vector_windows[vector_windows.size()-1].create_information((300-16*12)/2,140,"","Enter your name:",0,NO_SPECIAL_INFO);

    vector_windows[vector_windows.size()-1].create_information(15,165,"","",0,SPECIAL_INFO_CREATING_PROFILE);

    msg="Done";
    button_sound=7-1;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,240,"",msg,&button_event_create_profile,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Profile Menu";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,vector_windows[vector_windows.size()-1].return_h()-37,"",msg,&button_event_open_window_change_profile,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);

    //*****************//
    // Change Profile: //
    //*****************//

    vector_windows.push_back(Window((player.option_resolution_x-360)/2,(player.option_resolution_y-600)/2,360,600,"Change Profile",NO_SPECIAL_INPUT));

    set_change_profile_buttons();

    //**********//
    // Options: //
    //**********//

    vector_windows.push_back(Window((player.option_resolution_x-300)/2,(player.option_resolution_y-520)/2,300,520,"Options",NO_SPECIAL_INPUT));

    msg="Display Options";
    vector_windows[vector_windows.size()-1].create_information((300-msg.length()*12)/2,35,"",msg,0,NO_SPECIAL_INFO);

    msg="Fullscreen:";
    button_sound=7-1;
    vector_windows[vector_windows.size()-1].create_information(15,65,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,67,"","",0,SPECIAL_INFO_CHECKBOX_FULLSCREEN);
    vector_windows[vector_windows.size()-1].create_button(145-1,67,"","  ",&button_event_option_fullscreen,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Show FPS:";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_information(15,95,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,97,"","",0,SPECIAL_INFO_CHECKBOX_FPS);
    vector_windows[vector_windows.size()-1].create_button(145-1,97,"","  ",&button_event_option_fps,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Audio Options";
    vector_windows[vector_windows.size()-1].create_information((300-msg.length()*12)/2,135,"",msg,0,NO_SPECIAL_INFO);

    msg="Global Volume";
    vector_windows[vector_windows.size()-1].create_information(15,165,"",msg,0,NO_SPECIAL_INFO);
    msg="Mute:";
    button_sound=9-1;
    vector_windows[vector_windows.size()-1].create_information(15,195,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(75,197,"","",0,SPECIAL_INFO_CHECKBOX_MUTE_GLOBAL);
    vector_windows[vector_windows.size()-1].create_button(75-1,197,"","  ",&button_event_option_mute_global,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);
    msg="-";
    button_sound=7-1;
    vector_windows[vector_windows.size()-1].create_button(15,225,"",msg,&button_event_option_volume_down_global,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="+";
    button_sound=8-1;
    vector_windows[vector_windows.size()-1].create_button(258,225,"",msg,&button_event_option_volume_up_global,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    vector_windows[vector_windows.size()-1].create_information(45,234,"","",0,SPECIAL_INFO_VOLUME_GLOBAL);

    msg="Sound Effects Volume";
    vector_windows[vector_windows.size()-1].create_information(15,260,"",msg,0,NO_SPECIAL_INFO);
    msg="Mute:";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_information(15,290,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(75,292,"","",0,SPECIAL_INFO_CHECKBOX_MUTE_SOUND);
    vector_windows[vector_windows.size()-1].create_button(75-1,292,"","  ",&button_event_option_mute_sound,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);
    msg="-";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_button(15,320,"",msg,&button_event_option_volume_down_sound,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="+";
    button_sound=8-1;
    vector_windows[vector_windows.size()-1].create_button(258,320,"",msg,&button_event_option_volume_up_sound,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    vector_windows[vector_windows.size()-1].create_information(45,329,"","",0,SPECIAL_INFO_VOLUME_SOUND);

    msg="Music Volume";
    vector_windows[vector_windows.size()-1].create_information(15,355,"",msg,0,NO_SPECIAL_INFO);
    msg="Mute:";
    button_sound=10-1;
    vector_windows[vector_windows.size()-1].create_information(15,385,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(75,387,"","",0,SPECIAL_INFO_CHECKBOX_MUTE_MUSIC);
    vector_windows[vector_windows.size()-1].create_button(75-1,387,"","  ",&button_event_option_mute_music,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);
    msg="-";
    button_sound=12-1;
    vector_windows[vector_windows.size()-1].create_button(15,415,"",msg,&button_event_option_volume_down_music,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    msg="+";
    button_sound=8-1;
    vector_windows[vector_windows.size()-1].create_button(258,415,"",msg,&button_event_option_volume_up_music,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
    vector_windows[vector_windows.size()-1].create_information(45,424,"","",0,SPECIAL_INFO_VOLUME_MUSIC);

    msg="Configure Controls";
    button_sound=7-1;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,448,"",msg,&button_event_open_window_options_keys,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);

    msg="Back to Main Menu";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,vector_windows[vector_windows.size()-1].return_h()-37,"",msg,&button_event_open_window_main_menu,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);

    //********************//
    // Key Configuration: //
    //********************//

    vector_windows.push_back(Window((player.option_resolution_x-600)/2,(player.option_resolution_y-475)/2,600,275,"Controls",NO_SPECIAL_INPUT));

    msg="Left:";
    button_sound=7-1;
    y_position=30;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_LEFT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_left,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Up:";
    button_sound=11-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_UP);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_up,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Right:";
    button_sound=9-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_RIGHT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_right,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Down:";
    button_sound=7-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_DOWN);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_down,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Pause:";
    button_sound=8-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_PAUSE);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_pause,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Toggle Main Menu:";
    button_sound=12-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_TOGGLE_MAIN_MENU);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_toggle_main_menu,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Take Screenshot:";
    button_sound=10-1;
    y_position+=25;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);
    vector_windows[vector_windows.size()-1].create_information(145,y_position,"","",0,SPECIAL_INFO_KEY_SCREENSHOT);
    vector_windows[vector_windows.size()-1].create_button(15,y_position,"","                                                   ",&button_event_key_screenshot,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_INVISIBLE);

    msg="Back to Options";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button((600-(msg.length()*12))/2,vector_windows[vector_windows.size()-1].return_h()-37,"",msg,&button_event_open_window_options,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);

    //********//
    // About: //
    //********//

    vector_windows.push_back(Window((player.option_resolution_x-300)/2,(player.option_resolution_y-400)/2,300,400,"About",NO_SPECIAL_INPUT));

    msg="Cosmic Heist";
    vector_windows[vector_windows.size()-1].create_information((300-(msg.length()*12))/2,40,"",msg,0,NO_SPECIAL_INFO);

    vector_windows[vector_windows.size()-1].create_information(15,90,"","",0,SPECIAL_INFO_VERSION);

    msg="Back to Main Menu";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button((300-(msg.length()*12))/2,vector_windows[vector_windows.size()-1].return_h()-37,"",msg,&button_event_open_window_main_menu,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);

    //**********//
    // Credits: //
    //**********//

    vector_windows.push_back(Window((player.option_resolution_x-800)/2,(player.option_resolution_y-600)/2,800,600,"Credits",NO_SPECIAL_INPUT));

    msg="Cheese and Bacon Games";
    vector_windows[vector_windows.size()-1].create_information((800-msg.length()*12)/2,40,"",msg,0,NO_SPECIAL_INFO);

    msg="Design:";
    vector_windows[vector_windows.size()-1].create_information(15,90,"",msg,0,NO_SPECIAL_INFO);

    msg="Kevin Wells";
    vector_windows[vector_windows.size()-1].create_information(15,115,"",msg,0,NO_SPECIAL_INFO);

    msg="Programming:";
    vector_windows[vector_windows.size()-1].create_information(15,165,"",msg,0,NO_SPECIAL_INFO);

    msg="Kevin Wells";
    vector_windows[vector_windows.size()-1].create_information(15,190,"",msg,0,NO_SPECIAL_INFO);

    msg="Artwork:";
    vector_windows[vector_windows.size()-1].create_information(15,240,"",msg,0,NO_SPECIAL_INFO);

    msg="Kevin Wells";
    vector_windows[vector_windows.size()-1].create_information(15,265,"",msg,0,NO_SPECIAL_INFO);

    msg="Sound:";
    vector_windows[vector_windows.size()-1].create_information(15,315,"",msg,0,NO_SPECIAL_INFO);

    msg="Kevin Wells";
    vector_windows[vector_windows.size()-1].create_information(15,340,"",msg,0,NO_SPECIAL_INFO);

    msg="Music:";
    vector_windows[vector_windows.size()-1].create_information(15,390,"",msg,0,NO_SPECIAL_INFO);

    msg="Kevin Wells";
    vector_windows[vector_windows.size()-1].create_information(15,415,"",msg,0,NO_SPECIAL_INFO);

    msg="Special thanks to:";
    vector_windows[vector_windows.size()-1].create_information(275,90,"",msg,0,NO_SPECIAL_INFO);

    msg="Sam Lantinga and the SDL contributors";
    vector_windows[vector_windows.size()-1].create_information(275,115,"",msg,0,NO_SPECIAL_INFO);

    msg="The GLEW team";
    vector_windows[vector_windows.size()-1].create_information(275,140,"",msg,0,NO_SPECIAL_INFO);

    msg="The Boost team";
    vector_windows[vector_windows.size()-1].create_information(275,165,"",msg,0,NO_SPECIAL_INFO);

    msg="The free software community";
    vector_windows[vector_windows.size()-1].create_information(275,190,"",msg,0,NO_SPECIAL_INFO);

    msg="Additional thanks to:";
    vector_windows[vector_windows.size()-1].create_information(275,240,"",msg,0,NO_SPECIAL_INFO);

    msg="Aneissa Wells";
    vector_windows[vector_windows.size()-1].create_information(275,265,"",msg,0,NO_SPECIAL_INFO);

    msg="Back to Main Menu";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button((800-(msg.length()*12))/2,vector_windows[vector_windows.size()-1].return_h()-37,"",msg,&button_event_open_window_main_menu,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);

    //*******//
    // Help: //
    //*******//

    vector_windows.push_back(Window((player.option_resolution_x-665)/2,(player.option_resolution_y-600)/2,665,600,"Help",NO_SPECIAL_INPUT));

    msg="Welcome to Cosmic Heist!";
    y_position=35;
    vector_windows[vector_windows.size()-1].create_information((vector_windows[vector_windows.size()-1].return_w()-msg.length()*font.spacing_x)/2.0,y_position,"",msg,0,NO_SPECIAL_INFO);

    ///Blu Corporation -> Team Fortress 2.
    ///HPL Industries  -> Howard Phillips Lovecraft.
    ///Good luck -> Star Fox 64

    msg="You work for the Blu Corporation, the galaxy's\xAleading manufacturer of custom cruisers.\xA";
    msg+="HPL Industries, a recent newcomer to the industry,\xAhas just unveiled their own line of custom ships,\xA";
    msg+="rivaling Blu Corporation's latest offerings.\xA";
    msg+="\xAYour mission is a simple one:\xAInfiltrate enemy space, obtain a top secret X-class ship,\xA";
    msg+="and escape from enemy space with this ship.\xAThen return to enemy space, and take another.\xA\xA";
    msg+="In each level, you start with a liberated ship.\xA";
    msg+="You must then proceed to an extraction point.\xA";
    msg+="Once you reach an extraction point, you will be\xAsent back into enemy space.\xA";
    msg+="Enemy ships will attempt to stop you at any cost.\xA";
    msg+="You will need to watch out for these ships,\xA";
    msg+="as well as various bits of space debris.";
    y_position+=30;
    vector_windows[vector_windows.size()-1].create_information(15,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Good luck!";
    y_position+=30*15;
    vector_windows[vector_windows.size()-1].create_information((vector_windows[vector_windows.size()-1].return_w()-msg.length()*font.spacing_x)/2.0,y_position,"",msg,0,NO_SPECIAL_INFO);

    msg="Back to Main Menu";
    button_sound=11-1;
    vector_windows[vector_windows.size()-1].create_button((665-(msg.length()*12))/2,vector_windows[vector_windows.size()-1].return_h()-37,"",msg,&button_event_open_window_main_menu,&sound_button_mouse_over[button_sound],&sound_button_event_fire[button_sound],BUTTON_VISIBLE);
}
