/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef highscore_h
#define highscore_h

#include <string>
#include <stdint.h>

class Highscore{
    private:
    public:

    uint64_t score;

    std::string name;

    Highscore();
};

#endif
