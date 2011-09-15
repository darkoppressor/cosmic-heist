/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef music_h
#define music_h

#include <vector>
#include <string>

#include <SDL_mixer.h>

class Music{
private:

    struct music_data{
        Mix_Chunk* track;
        bool playing;
        int channel;
        double volume;
    };

    std::vector<music_data> tracks;

public:

    friend int get_free_channel();

    //
    void set_track_volumes();

    //Handle the fading in and out of tracks.
    void fadein_tracks();
    void fadeout_tracks();

    //Turn off the currently playing track, if any, and start playing track.
    void play_track(int track);

    //
    void initialize_tracks();

    //Load a single track.
    void load_track(std::string track);

    //Unload all tracks.
    void unload_tracks();
};

#endif
