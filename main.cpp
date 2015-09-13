/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include <vector>
#include <ctime>

#include <SDL.h>
#include <SDL_mixer.h>

#include "main.h"
#include "timer.h"
#include "world.h"
#include "update.h"
#include "quit.h"
#include "options.h"

using namespace std;

void game_loop(){
    //Turn off key repeating.
    SDL_EnableKeyRepeat(0,0);
    SDL_EnableUNICODE(SDL_ENABLE);

    music.load_track("data/music/menu.ogg");
    music.load_track("data/music/level_1.ogg");
    music.initialize_tracks();
    music.play_track(MUSIC_MENU);

    levels.push_back(Level());

    load_world_initial_1();
    load_world();
    load_world_initial_2();

    player.load_data();

    //If no profile exists.
    if(player.name=="\x1F"){
        vector_windows[WINDOW_CREATE_PROFILE].turn_on();
    }
    //If a profile already exists.
    else{
        profile.load_profile_global_data();
        vector_windows[WINDOW_MAIN_MENU].turn_on();
    }

    //The maximum number of frames to be skipped.
    const double MAX_FRAMESKIP=5;

    //In our logic update while() loop, we consume SKIP_TICKS sized chunks of time, which are added to next_game_tick.
    Uint32 next_game_tick=SDL_GetTicks();

    //The number of logic updates that have occured since the last render.
    int number_of_updates=0;

    //Declare and start the frame rate timer. This keeps track of the frame rate, as well as milliseconds spent per frame.
    //See the explanation at the beginning of the game loop for more details.
    Timer timer_frame_rate;
    timer_frame_rate.start();

    //The following three variables are used for displaying the FPS to the player.
    //This integer keeps track of the number of frame each second. Every time a second has passed, frame_count's value is given to frame_rate, and frame_count is reset to 0.
    int frame_count=0;

    //At any given time (after the first second of runtime), frame_rate represents the number of frames that were rendered in the past second.
    int frame_rate=0;

    //For added performance information, ms_per_frame takes the FPS value each second and converts it to the number of milliseconds spent on each frame in the past second.
    double ms_per_frame=0;

    //Keeps track of the logic frames each second.
    int logic_frame_count=0;
    ////At any given time (after the first second of runtime), logic_frame_rate represents the number of logic frames that were processed in the past second.
    int logic_frame_rate=0;

    //Here we have the game loop. Only when loop==false will this while() loop end. This should never happen, as when the game is closed, quit_game() should be called.
    while(true){
        //If at least a second(preferably exactly one second) has passed since the last restart of the frame rate timer, we do several things.
        //First, we set the frame rate to the current frame count, which has been counting up since the last timer restart.
        //Second, we reset the frame count to 0, to count the number of frames succesfully completed in the next second.
        //Third, we set the milliseconds per frame to 1000/our current frame rate. Since our frame rate is in seconds, this gives us the number of milliseconds being spent on
        //each frame. Finally, we restart the frame rate timer.
        if(timer_frame_rate.get_ticks()>=1000){
            logic_frame_rate=logic_frame_count;
            logic_frame_count=0;

            frame_rate=frame_count;
            frame_count=0;
            ms_per_frame=1000.0/frame_rate;
            timer_frame_rate.start();
        }

        //Here we are just incrementing the frame counter, which gives us the frames per second in the above if() statement.
        frame_count++;

        //For our game loop, we first update game logic, and then render. The two are kept separate and independent.

        //First we reset our updates counter to 0.
        number_of_updates=0;

        //We consume SKIP_TICKS sized chunks of time, which ultimately translates to updating the logic UPDATE_LIMIT times a second.
        //We also check to see if we've updated logic MAX_FRAMESKIP times without rendering, at which point we render.
        while(SDL_GetTicks()>next_game_tick && number_of_updates<MAX_FRAMESKIP){
            //We are doing another game logic update.
            //If this reaches MAX_FRAMESKIP, we will render.
            number_of_updates++;

            //Consume another SKIP_TICKS sized chunk of time.
            next_game_tick+=SKIP_TICKS;

            //Increment the logic frame counter.
            logic_frame_count++;

            //Now we update the game logic:

            //First, we check for input from the player.
            input();

            //Then, we move all of the objects.
            movement();

            //Once everything has had its chance to move, we handle events (collision detection).
            events();

            //Handle camera movement.
            camera(frame_rate,ms_per_frame,logic_frame_rate);

            //Animate everything. This entails first making sure that the appropriate animation state is being displayed, and then incrementing the frame for each sprite.
            animation();
        }

        //Now that we've handled logic updates, we do our rendering.

        //Finally, we render everything that is currently within the camera bounds to the screen.
        //We pass frame_rate and ms_per_frame so they can be displayed onscreen.
        render(frame_rate,ms_per_frame,logic_frame_rate);
    }
}

//Apparently, SDL likes main() to take these arguments, so that is what we will do.
int main(int argc, char* args[]){
    time_t seconds;
    uint32_t random_seed=(uint32_t)time(&seconds);
    rng.mrand_main.seed(random_seed);
    rng.mrand_render.seed(random_seed);

    image.current_texture=0;

    if(load_current_profile()){
        if(!options_load()){
            return 1;
        }
        if(!profile.load_profile_list()){
            return 1;
        }
    }

    //Initialize the various subsystems.
    if(!main_window.init()){
        fprintf(stderr,"Failed to initialize the window.\n");
        return 1;
    }

    player.save_initial_highscores();

    //Begin the game by entering the game loop.
    game_loop();
    return 0;
}
