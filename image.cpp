/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "image.h"
#include "world.h"
#include "render.h"

using namespace std;

void Image::load_images_global(){
    //Set texture dimensions.

    checkbox_unchecked.w=24;
    checkbox_unchecked.h=24;

    checkbox_checked.w=24;
    checkbox_checked.h=24;

    ships.w=69;
    ships.h=92;

    exits.w=512;
    exits.h=128;

    tiles.w=32;
    tiles.h=192;

    explosion.w=23;
    explosion.h=69;

    //Load the global images.
    checkbox_unchecked.texture=load_texture("data/images/checkbox_unchecked.png");
    checkbox_checked.texture=load_texture("data/images/checkbox_checked.png");
    ships.texture=load_texture("data/images/ships.png");
    exits.texture=load_texture("data/images/exit.png");
    tiles.texture=load_texture("data/images/tiles.png");
    explosion.texture=load_texture("data/images/explosion.png");

    //Generate the background layer textures.
    for(int i=0;i<levels[0].background_layers.size();i++){
        background_layer_images.push_back(image_data());

        background_layer_images[i].w=levels[0].background_layers[i].width;
        background_layer_images[i].h=levels[0].background_layers[i].height;
        background_layer_images[i].texture=levels[0].background_layers[i].generate_texture_background(background_layer_images[i].w,background_layer_images[i].h,i);
    }

    for(int i=0;i<levels[0].menu_backgrounds.size();i++){
        menu_background_images.push_back(image_data());

        menu_background_images[i].w=levels[0].menu_backgrounds[i].width;
        menu_background_images[i].h=levels[0].menu_backgrounds[i].height;

        string file_name="data/images/menu_background_";
        ss.clear();ss.str("");ss<<i+1;file_name+=ss.str();
        file_name+=".png";
        menu_background_images[i].texture=load_texture(file_name);
    }
}

void Image::unload_images_global(){
    //Free up the global images.
    glDeleteTextures(1,&checkbox_unchecked.texture);
    glDeleteTextures(1,&checkbox_checked.texture);
    glDeleteTextures(1,&ships.texture);
    glDeleteTextures(1,&exits.texture);
    glDeleteTextures(1,&tiles.texture);
    glDeleteTextures(1,&explosion.texture);

    for(int i=0;i<background_layer_images.size();i++){
        glDeleteTextures(1,&background_layer_images[i].texture);
    }
    background_layer_images.clear();

    for(int i=0;i<menu_background_images.size();i++){
        glDeleteTextures(1,&menu_background_images[i].texture);
    }
    menu_background_images.clear();
}
