#include "quit.h"

#include <SDL.h>
#include <SDL_mixer.h>

#include "world.h"
#include "options.h"

using namespace std;

void quit_game(){
    if(player.game_in_progress){
        player.save_highscore();
    }

    profile.save_profile_global_data();

    options_save();

    music.unload_tracks();

    //Disable unicode.
    SDL_EnableUNICODE(SDL_DISABLE);

    //This function frees any dynamically allocated memory that was loaded in load_world().
    unload_world();

    //Close any opened joysticks.
    for(int i=0;i<number_of_joysticks;i++){
        //Close the joystick, if it is opened.
        if(SDL_JoystickOpened(i)){
            SDL_JoystickClose(joystick[i].joy);
        }
    }

    //Close the audio subsystem.
    Mix_CloseAudio();

    //Shutdown SDL.
    SDL_Quit();

    //Print a success message to stdout.txt.
    printf("Exited successfully!\n");

    //Tell the program to exit successfully.
    exit(EXIT_SUCCESS);
}
