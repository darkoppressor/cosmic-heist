/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "music.h"
#include "world.h"
#include "positional_audio.h"

using namespace std;

void Music::set_track_volumes(){
    for(int i=0;i<tracks.size();i++){
        //Set the track's channel volume.
        Mix_Volume(tracks[i].channel,modify_music_volume((int)(tracks[i].volume*128)));
    }
}

void Music::fadein_tracks(){
    for(int i=0;i<tracks.size();i++){
        //If the track is playing and the track's volume is not already maxed out, fade it in.
        if(tracks[i].playing && tracks[i].volume<1.0){
            tracks[i].volume+=0.005;
            if(tracks[i].volume>1.0){
                tracks[i].volume=1.0;
            }

            //Set the track's channel volume.
            Mix_Volume(tracks[i].channel,modify_music_volume((int)(tracks[i].volume*128)));
        }
    }
}

void Music::fadeout_tracks(){
    for(int i=0;i<tracks.size();i++){
        //If the track is not playing and the track's volume is not already off, fade it out.
        if(!tracks[i].playing && tracks[i].volume>0.0){
            tracks[i].volume-=0.05;
            if(tracks[i].volume<0.0){
                tracks[i].volume=0.0;
            }

            //Set the track's channel volume.
            Mix_Volume(tracks[i].channel,modify_music_volume((int)(tracks[i].volume*128)));
        }
    }
}

void Music::play_track(int track){
    //If the track exists.
    if(track<tracks.size()){
        //Set all tracks to fade out.
        for(int i=0;i<tracks.size();i++){
            //If this track is currently playing.
            if(tracks[i].playing){
                tracks[i].playing=false;
            }
        }

        //Set the new track to fade in.
        tracks[track].playing=true;
    }
}

void Music::initialize_tracks(){
    for(int i=0;i<tracks.size();i++){
        //If the track has a channel associated with it.
        if(tracks[i].channel!=-1){
            //Halt the track's channel.
            Mix_HaltChannel(tracks[i].channel);

            //Set the track's channel volume.
            Mix_Volume(tracks[i].channel,modify_music_volume((int)(tracks[i].volume*128)));

            //Start "playing" the music track.
            Mix_PlayChannel(tracks[i].channel,tracks[i].track,-1);
        }
    }
}

void Music::load_track(string track){
    tracks.push_back(music_data());
    tracks[tracks.size()-1].track=Mix_LoadWAV(track.c_str());
    tracks[tracks.size()-1].playing=false;
    tracks[tracks.size()-1].channel=get_free_channel();
    tracks[tracks.size()-1].volume=0.0;
}

void Music::unload_tracks(){
    //Free all tracks from memory.
    for(int i=0;i<tracks.size();i++){
        Mix_HaltChannel(tracks[i].channel);
        Mix_FreeChunk(tracks[i].track);
        tracks[i].track=NULL;
    }

    //Clear the tracks vector.
    tracks.clear();
}
