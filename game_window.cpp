/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "game_window.h"
#include "world.h"
#include "quit.h"
#include "pixels.h"

#include <time.h>
#include <string>

using namespace std;

Game_Window::Game_Window(){
    SCREEN_BPP=32;

    screen=NULL;
}

bool Game_Window::initialize_opengl(){
    //Set OpenGL attributes:
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    //Set up the screen:
    if(player.option_fullscreen){
        screen=SDL_SetVideoMode(player.option_resolution_x,player.option_resolution_y,SCREEN_BPP,SDL_OPENGL|SDL_HWSURFACE|SDL_FULLSCREEN);
    }
    else{
        screen=SDL_SetVideoMode(player.option_resolution_x,player.option_resolution_y,SCREEN_BPP,SDL_OPENGL|SDL_HWSURFACE);
    }

    //Initialize OpenGL.
    //I am still a beginner at using OpenGL, so I'll put forth my best explanation for how things work, exactly.

    //Enable 2D textures.
    glEnable(GL_TEXTURE_2D);

    //Enable blending.
    glEnable(GL_BLEND);

    //
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    //Set clear color. When the screen is cleared, it will be filled with this color.
    glClearColor(0,0,0,1);

    //Set the OpenGL viewport to be the same size as our screen.
    glViewport(0,0,player.option_resolution_x,player.option_resolution_y);

    //Set projection:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,player.option_resolution_x,player.option_resolution_y,0,-1,1);

    //Initialize modelview matrix:
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Initialize GLEW.
    GLenum glew_err=glewInit();
    if(glew_err!=GLEW_OK){
        fprintf(stderr,"GLEW Error: %s\n",glewGetErrorString(glew_err));
        return false;
    }

    //If OpenGL version 1.5 is not supported, check for the needed extension.
    //If that is also unsupported, the game cannot run.
    /**if(!GLEW_VERSION_1_5){
        if(!GLEW_ARB_vertex_buffer_object){
            fprintf(stderr,"GLEW Error: Extension ARB_vertex_buffer_object is not supported by your video card drivers.\n");
            return false;
        }
    }*/

    return true;
}

bool Game_Window::init(){
    //Initialize all of the SDL stuff:

    //Initialize all of the standard SDL stuff, and return false if it did not initialize properly.
    if(SDL_Init(SDL_INIT_EVERYTHING)==-1){
        fprintf(stderr,"Unable to init SDL: %s\n",SDL_GetError());
        return false;
    }

    //Attempt to center the window on the screen.
    SDL_putenv("SDL_VIDEO_CENTERED=center");

    if(!initialize_opengl()){
        return false;
    }

    //If the screen could not be set up.
    if(!screen){
        fprintf(stderr,"Unable to set video mode: %s\n",SDL_GetError());
        return false;
    }

    //If there were any errors.
    if(glGetError()!=GL_NO_ERROR){
        fprintf(stderr,"OpenGL initialization failed.\n");
        return false;
    }

    //Start up the audio system.
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)==-1){
        fprintf(stderr,"SDL_Mixer initialization failed.\n");
        return false;
    }

    Mix_AllocateChannels(2000);

    //Open all available joysticks.
    for(int i=0;i<SDL_NumJoysticks();i++){
        joystick.push_back(joy_stick());
        joystick[i].joy=NULL;
        joystick[i].joy=SDL_JoystickOpen(i);
    }

    //Check for any joystick open failures.
    for(int i=0;i<SDL_NumJoysticks();i++){
        //If the joystick could not be opened.
        if(joystick[i].joy==NULL){
            string error_string="";
            ss.clear();ss.str("");ss<<"Failed to open joystick ";ss<<i;ss<<".\n";error_string=ss.str();
            fprintf(stderr,error_string.c_str());
        }
    }

    number_of_joysticks=SDL_NumJoysticks();

    //Set the window caption.
    SDL_WM_SetCaption("Cosmic Heist","Cosmic Heist");

    //Show the mouse cursor.
    SDL_ShowCursor(1);

    //Everything initialized fine, so return true.
    return true;
}

void Game_Window::toggle_fullscreen(){
    //Remember which window is currently open, if any.
    short which_window_open=window_manager.which_window_open();

    //Close all windows.
    window_manager.close_windows(0);

    //Pause the game.
    player.toggle_pause(true);

    //To toggle fullscreen with OpenGL, we must reinitialize all of the OpenGL stuff and reload the textures.
    initialize_opengl();

    unload_world();
    load_world();

    //If a window was open before.
    if(which_window_open!=-1){
        if(which_window_open==WHICH_WINDOW_HIGHSCORES){
            window_highscores[0].turn_on();
        }
        else if(which_window_open>=WHICH_WINDOW_OTHER){
            vector_windows[which_window_open-WHICH_WINDOW_OTHER].turn_on();
        }
    }
    else{
        //Unpause the game.
        player.toggle_pause(false);
    }
}

void Game_Window::screenshot(){
    //First, we setup the filename for the screenshot.

    //Determine the date and time.
    time_t now;
    struct tm *tm_now;
    char buff[BUFSIZ];
    now=time(NULL);
    tm_now=localtime(&now);

    //Store the date and time in buff.
    strftime(buff,sizeof buff,"%Y-%A_%B_%d-%H%M.%S",tm_now);

    //Copy buff's data into the date string for use with screenshot_name below.
    string date=buff;

    //Used to store the filename of the screenshot.
    string screenshot_name;

    //Set the filename.
    screenshot_name="profiles/";
    screenshot_name+=player.name;
    screenshot_name+="/screenshots/";
    screenshot_name+=date;
    screenshot_name+=".bmp";

    //Create temporary surfaces.
    SDL_Surface *surface=NULL;
    SDL_Surface *flipped_surface=NULL;

    //This will hold the pixel data from the frame buffer.
    unsigned char pixel_data[4*player.option_resolution_x*player.option_resolution_y];

    //Read the pixels from the frame buffer and store them in pixel_data.
    glReadPixels(0,0,player.option_resolution_x,player.option_resolution_y,GL_BGRA,GL_UNSIGNED_BYTE,pixel_data);

    //Create an SDL surface from this pixel data.
    surface=SDL_CreateRGBSurfaceFrom(pixel_data,player.option_resolution_x,player.option_resolution_y,screen->format->BitsPerPixel,player.option_resolution_x*screen->format->BytesPerPixel,screen->format->Rmask,screen->format->Gmask,screen->format->Bmask,screen->format->Amask);

    //Create a SDL surface for holding the flipped image.
    flipped_surface=SDL_CreateRGBSurface(SDL_SWSURFACE,surface->w,surface->h,surface->format->BitsPerPixel,surface->format->Rmask,surface->format->Gmask,surface->format->Bmask,surface->format->Amask);

    //If the surface must be locked.
    if(SDL_MUSTLOCK(surface)){
        //Lock the surface.
        SDL_LockSurface(surface);
    }

    //Read the pixel data from surface and store it (flipped vertically) in flipped_surface.
    for(int x=0,rx=surface->w-1;x<surface->w;x++,rx--){
        for(int y=0,ry=surface->h-1;y<surface->h;y++,ry--){
            Uint32 pixel=surface_get_pixel(surface,x,y);
            surface_put_pixel(flipped_surface,x,ry,pixel);
        }
    }

    //If the surface had to be locked.
    if(SDL_MUSTLOCK(surface)){
        //Unlock the surface.
        SDL_UnlockSurface(surface);
    }

    //Save the flipped surface to the screenshot file.
    SDL_SaveBMP(flipped_surface,screenshot_name.c_str());

    //Free the surfaces.
    SDL_FreeSurface(flipped_surface);
    SDL_FreeSurface(surface);
}
