/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef background_layer_h
#define background_layer_h

#define GLEW_STATIC
#include <glew.h>

class Background_Layer{
    private:
    public:

    Background_Layer(double get_width,double get_height,double get_parallax_speed_x,double get_parallax_speed_y);

    GLuint generate_texture_background(double width,double height,short layer_number);

    void update(double camera_delta_x,double camera_delta_y);

    void render(short layer_number);

    //Every layer uses two identical images.

    //The x and y positions of the first layer image.
    double position_x_a;
    double position_y_a;

    //The x and y positions of the second layer image.
    double position_x_b;
    double position_y_b;

    //The dimensions of layer images.
    double width;
    double height;

    //The rate at which the background scrolls.
    //Lower numbers scroll the background faster.
    double parallax_speed_x,parallax_speed_y;
};

#endif
