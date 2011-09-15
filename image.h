/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef image_h
#define image_h

#include <vector>

#include "image_data.h"

#include <SDL.h>
#include <string>

#define GLEW_STATIC
#include <glew.h>

class Image{
    private:
    public:

    //Load global images.
    void load_images_global();

    //Unload global images.
    void unload_images_global();

    //The currently bound texture.
    GLuint current_texture;

    //Global images.
    std::vector<image_data> background_layer_images;
    std::vector<image_data> menu_background_images;

    image_data checkbox_unchecked;
    image_data checkbox_checked;
    image_data ships;
    image_data exits;
    image_data tiles;
    image_data explosion;
};

#endif
