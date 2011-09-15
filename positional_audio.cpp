/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "positional_audio.h"
#include "world.h"

using namespace std;

int modify_sound_volume(int volume){
    //If either the global mute or the sound effects mute is enabled.
    if(player.option_mute_global || player.option_mute_sound){
        return 0;
    }

    double new_volume=volume;

    new_volume*=player.option_volume_sound;

    new_volume*=player.option_volume_global;

    return (int)new_volume;
}

int modify_music_volume(int volume){
    //If either the global mute or the music mute is enabled.
    if(player.option_mute_global || player.option_mute_music){
        return 0;
    }

    double new_volume=volume;

    new_volume*=player.option_volume_music;

    new_volume*=player.option_volume_global;

    return (int)new_volume;
}

int get_free_channel(){
    int allocated_channels=Mix_AllocateChannels(-1);

    for(int i=0;i<allocated_channels;i++){
        //Check through all music tracks currently loaded.
        bool channel_taken_by_music_track=false;
        for(int n=0;n<music.tracks.size();n++){
            //If this channel is equal to this track's channel, then this channel is taken.
            if(i==music.tracks[n].channel){
                channel_taken_by_music_track=true;
            }
        }

        //If the channel is already playing.
        if(Mix_Playing(i)==1){
        }
        //If this channel is already reserved by a music track.
        else if(channel_taken_by_music_track){
        }
        //All tests were passed, and this channel is currently free, so return it.
        else{
            return i;
        }
    }

    return -1;
}

void play_positional_sound(Mix_Chunk *sound,double sound_x,double sound_y,double falloff){
    int volume=128;

    if(sound_x!=-1){
        double distance_x=fabs((player.camera_x+player.camera_w/2.0)-sound_x);
        double distance_y=fabs((player.camera_y+player.camera_h/2.0)-sound_y);
        double average=(distance_x+distance_y)/2;
        volume=(int)((falloff/average)*falloff);
        if(volume>128){
            volume=128;
        }
    }

    volume=modify_sound_volume(volume);

    int channel=get_free_channel();

    //
    if(channel!=-1){
        Mix_Volume(channel,volume);
        Mix_PlayChannel(channel,sound,0);
    }
}
