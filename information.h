/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef information_h
#define information_h

#include <string>

#include "image_data.h"

class Information{
    private:

    //The coordinate location.
    //This location, as used by a window, is an offset from the window, not the actual screen.
    short x,y;

    //Dimensions.
    short w,h;

    std::string text;

    std::string tooltip_text;

    image_data* info_image;

    //The special info to display, if any.
    //Any non-zero value specifies special info.
    short special_info;

    void set_dimensions();

    //If there is special info, and it is a string, this function sets that string.
    //This is used to create strings that take some variable(s) into account.
    void set_special_string();

    //If there is special info, and it is an image, this function sets that image.
    //This is used to create images that take some variable(s) into account.
    void set_special_image();

    public:

    //get_text, get_info_image, and get_special are a special case.
    //Only one should be used per Information object.
    Information(short get_x,short get_y,std::string get_tooltip_text,std::string get_text,image_data* get_info_image,short get_special_info);

    void handle_input(int mouse_x,int mouse_y,short x_offset,short y_offset);

    //Render the information, offset by x_offset and y_offset.
    void render(short x_offset,short y_offset);
};

#endif
