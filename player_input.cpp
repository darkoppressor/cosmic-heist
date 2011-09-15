/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "player.h"
#include "world.h"
#include "collision.h"
#include "options.h"
#include "quit.h"
#include "positional_audio.h"

using namespace std;

void Player::prepare_for_input(){
    //Get the SDL keystates and store them in the keystates variable for evaluation.
    keystates=SDL_GetKeyState(NULL);
}

void Player::handle_command_event(short command){
    switch(command){

    case COMMAND_TOGGLE_MAIN_MENU:
        //If the game is in progress.
        if(game_in_progress){
            //Close all windows.
            window_manager.close_windows(&vector_windows[WINDOW_MAIN_MENU]);

            vector_windows[WINDOW_MAIN_MENU].toggle_on();
        }
        break;

    case COMMAND_SCREENSHOT:
        main_window.screenshot();
        break;

    case COMMAND_PAUSE:
        if(window_manager.which_window_open()==-1){
            toggle_pause(!pause);
            if(pause){
                play_positional_sound(sound_pause);
            }
            else{
                play_positional_sound(sound_unpause);
            }
        }
        break;

    }
}

void Player::handle_input_states(){
    handle_input_states_always();

    handle_input_states_during_play();
}

void Player::handle_input_states_always(){
    //Toggle fullscreen.
    if((keystates[SDLK_LALT] || keystates[SDLK_RALT]) && keystates[SDLK_RETURN]){
        toggle_fullscreen();

        if(keystates[SDLK_LALT]){
            keystates[SDLK_LALT]=NULL;
        }
        if(keystates[SDLK_RALT]){
            keystates[SDLK_RALT]=NULL;
        }
        keystates[SDLK_RETURN]=NULL;
    }

    //Quit the game.
    if((keystates[SDLK_LALT] || keystates[SDLK_RALT]) && keystates[SDLK_F4]){
        quit_game();

        if(keystates[SDLK_LALT]){
            keystates[SDLK_LALT]=NULL;
        }
        if(keystates[SDLK_RALT]){
            keystates[SDLK_RALT]=NULL;
        }
        keystates[SDLK_F4]=NULL;
    }

    //Toggle main menu.
    if(keystates[SDLK_ESCAPE]){
        //If the escape key is pressed, stop setting the command.
        if(command_to_set!=-1){
            command_to_set=-1;
        }
        //If the game is in progress.
        else if(game_in_progress && command_to_set==-1){
            handle_command_event(COMMAND_TOGGLE_MAIN_MENU);
        }
        //If the game is not in progress and there is not command being set.
        else if(!game_in_progress && command_to_set==-1){
            //Close all windows.
            window_manager.close_windows(0);

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

        keystates[SDLK_ESCAPE]=NULL;
    }

    ///Should dev mode toggling be disabled in any release builds?
    //Toggle dev mode.
    if(keystates[SDLK_d] && keystates[SDLK_e] && keystates[SDLK_v]){
        option_dev=!option_dev;
        options_save();

        keystates[SDLK_d]=NULL;
        keystates[SDLK_e]=NULL;
        keystates[SDLK_v]=NULL;
    }
}

void Player::handle_input_states_during_play(){
    player_ship[0].handle_input_player();

    //******************//
    // Camera controls: //
    //******************//

    //If numpad 0 is pressed, toggle the camera's stickiness and play the appropriate sound.
    /**if(keystates[SDLK_KP0]){
        if(cam_state==CAM_STICKY){
            cam_state=0;
            play_positional_sound(sound_camera_unlock);
        }
        else{
            cam_state=CAM_STICKY;
            play_positional_sound(sound_camera_lock);
        }

        //Once the toggle camera stickiness key has been hit, the player must release it for it to function again.
        keystates[SDLK_KP0]=NULL;
    }*/

    //If the camera is unsticky, check for camera inputs.
    if(cam_state!=CAM_STICKY){
        //Handle camera directional keys being pressed.
        if(command_state(COMMAND_LEFT)){
            cam_state=LEFT;
        }
        if(command_state(COMMAND_UP)){
            cam_state=UP;
        }
        if(command_state(COMMAND_RIGHT)){
            cam_state=RIGHT;
        }
        if(command_state(COMMAND_DOWN)){
            cam_state=DOWN;
        }

        //Handle multiple camera directional keys being pressed at once.
        if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP)){
            cam_state=LEFT_UP;
        }
        if(command_state(COMMAND_UP) && command_state(COMMAND_RIGHT)){
            cam_state=RIGHT_UP;
        }
        if(command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
            cam_state=RIGHT_DOWN;
        }
        if(command_state(COMMAND_DOWN) && command_state(COMMAND_LEFT)){
            cam_state=LEFT_DOWN;
        }
        if(command_state(COMMAND_LEFT) && command_state(COMMAND_RIGHT)){
            cam_state=LEFT;
        }
        if(command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
            cam_state=UP;
        }
        if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT)){
            cam_state=LEFT_UP;
        }
        if(command_state(COMMAND_LEFT) && command_state(COMMAND_DOWN) && command_state(COMMAND_RIGHT)){
            cam_state=LEFT_DOWN;
        }
        if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_DOWN)){
            cam_state=LEFT_UP;
        }
        if(command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
            cam_state=RIGHT_UP;
        }
        if(command_state(COMMAND_LEFT) && command_state(COMMAND_UP) && command_state(COMMAND_RIGHT) && command_state(COMMAND_DOWN)){
            cam_state=LEFT_UP;
        }

        //If no camera directional keys are pressed, stop the camera.
        if(!command_state(COMMAND_LEFT) && !command_state(COMMAND_UP) && !command_state(COMMAND_RIGHT) && !command_state(COMMAND_DOWN)){
            cam_state=NONE;
        }
    }

    ///******************///
    /// Debug/Test Code. ///
    ///******************///

    //If developer mode is enabled and the dev key is pressed.
    if(option_dev && keystates[SDLK_F1]){
        if(keystates[SDLK_s]){
            if(UPDATE_RATE==60.0){
                UPDATE_RATE=30.0;
            }
            else{
                UPDATE_RATE=60.0;
            }
            SKIP_TICKS=1000.0/UPDATE_RATE;

            keystates[SDLK_s]=NULL;
        }
    }

    ///*************************///
    /// End of Debug/Test Code. ///
    ///*************************///
}

bool Player::command_state(short command){
    Sint16 axis_value=0;
    Uint8 hat_value=SDL_HAT_CENTERED;
    short ball_direction=NONE;
    int ball_delta_x=0;
    int ball_delta_y=0;

    //If this command's bound input is a keyboard key, and the key is being pressed.
    if(keys[command].type==INPUT_TYPE_KEYBOARD && keystates[keys[command].key]){
        return true;
    }

    //Check all available joysticks for input.
    for(int i=0;i<number_of_joysticks;i++){
        //As long as this joystick is opened properly, and is the joystick associated with this command.
        if(SDL_JoystickOpened(i) && joystick[i].joy!=NULL && i==keys[command].which_joystick){
            //If this command's bound input is a joystick button, and the button is being pressed.
            if(keys[command].type==INPUT_TYPE_JOYSTICK_BUTTON && SDL_JoystickGetButton(joystick[i].joy,keys[command].joy_button)){
                return true;
            }

            //If this command's bound input is a joystick axis.
            else if(keys[command].type==INPUT_TYPE_JOYSTICK_AXIS){
                //Get the current value of this command's bound axis.
                axis_value=SDL_JoystickGetAxis(joystick[i].joy,keys[command].joy_axis);

                //If the axis is outside of the neutral zone and held in the correct direction.
                if(!keys[command].joy_axis_direction && axis_value<JOYSTICK_NEUTRAL_NEGATIVE){
                    return true;
                }
                else if(keys[command].joy_axis_direction && axis_value>JOYSTICK_NEUTRAL_POSITIVE){
                    return true;
                }
            }

            //If this command's bound input is a joystick hat.
            else if(keys[command].type==INPUT_TYPE_JOYSTICK_HAT){
                //Get the current value of this command's bound hat.
                hat_value=SDL_JoystickGetHat(joystick[i].joy,keys[command].joy_hat);

                //If the hat is held in the correct direction.
                if(hat_value==keys[command].joy_hat_direction){
                    return true;
                }
            }

            //If this command's bound input is a joystick ball.
            else if(keys[command].type==INPUT_TYPE_JOYSTICK_BALL){
                //Get the current delta movement of this command's bound ball.
                SDL_JoystickGetBall(joystick[i].joy,keys[command].joy_ball,&ball_delta_x,&ball_delta_y);

                //Set the ball's direction using the deltas.
                if(ball_delta_x<0 && ball_delta_y<0){
                    ball_direction=LEFT_UP;
                }
                else if(ball_delta_x<0 && ball_delta_y>0){
                    ball_direction=LEFT_DOWN;
                }
                else if(ball_delta_x>0 && ball_delta_y<0){
                    ball_direction=RIGHT_UP;
                }
                else if(ball_delta_x>0 && ball_delta_y>0){
                    ball_direction=RIGHT_DOWN;
                }
                else if(ball_delta_x<0){
                    ball_direction=LEFT;
                }
                else if(ball_delta_x>0){
                    ball_direction=RIGHT;
                }
                else if(ball_delta_y<0){
                    ball_direction=UP;
                }
                else if(ball_delta_y>0){
                    ball_direction=DOWN;
                }

                //If the ball has moved in the correct direction.
                if(ball_direction==keys[command].joy_ball_direction){
                    return true;
                }
            }
        }
    }

    //If neither the key nor the joystick input bound to this command are active,
    //return false to let the game know this command is not currently being given.
    return false;
}

string Player::command_bound_input(short command){
    string string_to_return="None";

    //If this command's bound input is a keyboard key.
    if(keys[command].type==INPUT_TYPE_KEYBOARD){
        ss.clear();ss.str("");ss<<SDL_GetKeyName(keys[command].key);string_to_return=ss.str();
    }

    //If this command's bound input is a joystick button.
    else if(keys[command].type==INPUT_TYPE_JOYSTICK_BUTTON){
        ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)keys[command].which_joystick;ss<<" Button ";ss<<(int)keys[command].joy_button;string_to_return=ss.str();
    }

    //If this command's bound input is a joystick axis.
    else if(keys[command].type==INPUT_TYPE_JOYSTICK_AXIS){
        ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)keys[command].which_joystick;ss<<" Axis ";ss<<(int)keys[command].joy_axis;ss<<" ";ss<<keys[command].joy_axis_direction;string_to_return=ss.str();
    }

    //If this command's bound input is a joystick hat.
    else if(keys[command].type==INPUT_TYPE_JOYSTICK_HAT){
        ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)keys[command].which_joystick;ss<<" Hat ";ss<<(int)keys[command].joy_hat;ss<<" ";string_to_return=ss.str();
        if(keys[command].joy_hat_direction==SDL_HAT_LEFT){
            string_to_return+="Left";
        }
        else if(keys[command].joy_hat_direction==SDL_HAT_UP){
            string_to_return+="Up";
        }
        else if(keys[command].joy_hat_direction==SDL_HAT_RIGHT){
            string_to_return+="Right";
        }
        else if(keys[command].joy_hat_direction==SDL_HAT_DOWN){
            string_to_return+="Down";
        }
        else if(keys[command].joy_hat_direction==SDL_HAT_LEFTUP){
            string_to_return+="Left Up";
        }
        else if(keys[command].joy_hat_direction==SDL_HAT_LEFTDOWN){
            string_to_return+="Left Down";
        }
        else if(keys[command].joy_hat_direction==SDL_HAT_RIGHTUP){
            string_to_return+="Right Up";
        }
        else if(keys[command].joy_hat_direction==SDL_HAT_RIGHTDOWN){
            string_to_return+="Right Down";
        }
    }

    //If this command's bound input is a joystick ball.
    else if(keys[command].type==INPUT_TYPE_JOYSTICK_BALL){
        ss.clear();ss.str("");ss<<"Joystick ";ss<<(int)keys[command].which_joystick;ss<<" Ball ";ss<<(int)keys[command].joy_ball;ss<<" ";string_to_return=ss.str();
        if(keys[command].joy_ball_direction==LEFT){
            string_to_return+="Left";
        }
        else if(keys[command].joy_ball_direction==UP){
            string_to_return+="Up";
        }
        else if(keys[command].joy_ball_direction==RIGHT){
            string_to_return+="Right";
        }
        else if(keys[command].joy_ball_direction==DOWN){
            string_to_return+="Down";
        }
        else if(keys[command].joy_ball_direction==LEFT_UP){
            string_to_return+="Left Up";
        }
        else if(keys[command].joy_ball_direction==LEFT_DOWN){
            string_to_return+="Left Down";
        }
        else if(keys[command].joy_ball_direction==RIGHT_UP){
            string_to_return+="Right Up";
        }
        else if(keys[command].joy_ball_direction==RIGHT_DOWN){
            string_to_return+="Right Down";
        }
    }

    return string_to_return;
}

void Player::handle_input_events(){
    if(event.type==SDL_QUIT){
        quit_game();
        return;
    }

    check_for_command_set();

    //As long as the player's input events are not being blocked.
    if(!block_input_events){
        //Look through all commands, and see if any should be triggered by this input event.
        for(int i=0;i<keys.size();i++){
            //If the input type is a keyboard keypress, and the event is a keyboard keypress.
            if(keys[i].type==INPUT_TYPE_KEYBOARD && event.type==SDL_KEYDOWN){
                if(event.key.keysym.sym==keys[i].key){
                    handle_command_event(i);
                    return;
                }
            }
            //If the input type is a joystick button press, and the event is a joystick button press, and the event joystick is the command's bound joystick.
            else if(keys[i].type==INPUT_TYPE_JOYSTICK_BUTTON && event.type==SDL_JOYBUTTONDOWN && event.jbutton.which==keys[i].which_joystick){
                if(event.jbutton.button==keys[i].joy_button){
                    handle_command_event(i);
                    return;
                }
            }
            //If the input type is a joystick axis motion, and the event is a joystick axis motion, and the event joystick is the command's bound joystick.
            else if(keys[i].type==INPUT_TYPE_JOYSTICK_AXIS && event.type==SDL_JOYAXISMOTION && event.jaxis.which==keys[i].which_joystick){
                if(event.jaxis.axis==keys[i].joy_axis){
                    if(!keys[i].joy_axis_direction && event.jaxis.value<JOYSTICK_NEUTRAL_NEGATIVE){
                        handle_command_event(i);
                        return;
                    }
                    else if(keys[i].joy_axis_direction && event.jaxis.value>JOYSTICK_NEUTRAL_POSITIVE){
                        handle_command_event(i);
                        return;
                    }
                }
            }
            //If the input type is a joystick hat motion, and the event is a joystick hat motion, and the event joystick is the command's bound joystick.
            else if(keys[i].type==INPUT_TYPE_JOYSTICK_HAT && event.type==SDL_JOYHATMOTION && event.jhat.which==keys[i].which_joystick){
                if(event.jhat.hat==keys[i].joy_hat){
                    if(keys[i].joy_hat_direction==event.jhat.value){
                        handle_command_event(i);
                        return;
                    }
                }
            }
            //If the input type is a joystick ball motion, and the event is a joystick ball motion, and the event joystick is the command's bound joystick.
            else if(keys[i].type==INPUT_TYPE_JOYSTICK_BALL && event.type==SDL_JOYBALLMOTION && event.jball.which==keys[i].which_joystick){
                if(event.jball.ball==keys[i].joy_ball){
                    short ball_direction=NONE;

                    //Set the ball's direction using the deltas.
                    if(event.jball.xrel<0 && event.jball.yrel<0){
                        ball_direction=LEFT_UP;
                    }
                    else if(event.jball.xrel<0 && event.jball.yrel>0){
                        ball_direction=LEFT_DOWN;
                    }
                    else if(event.jball.xrel>0 && event.jball.yrel<0){
                        ball_direction=RIGHT_UP;
                    }
                    else if(event.jball.xrel>0 && event.jball.yrel>0){
                        ball_direction=RIGHT_DOWN;
                    }
                    else if(event.jball.xrel<0){
                        ball_direction=LEFT;
                    }
                    else if(event.jball.xrel>0){
                        ball_direction=RIGHT;
                    }
                    else if(event.jball.yrel<0){
                        ball_direction=UP;
                    }
                    else if(event.jball.yrel>0){
                        ball_direction=DOWN;
                    }

                    //If the ball has moved in the correct direction.
                    if(ball_direction==keys[i].joy_ball_direction){
                        handle_command_event(i);
                        return;
                    }
                }
            }
        }
    }
}
