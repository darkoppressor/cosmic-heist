#ifndef exit_h
#define exit_h

#include "sprite_sheets.h"

#include <string>

class Exit{
    private:
    public:

    double x,y;

    short frame;
    short frame_counter;

    Exit(double get_x,double get_y);

    void animate();

    void render();
};

#endif
