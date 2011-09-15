/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef profile_h
#define profile_h

#include <SDL.h>

#include <vector>
#include <string>

class Profile{
    private:
    public:
    Profile();

    //Create the entire needed directory structure.
    void make_directories();

    //Delete the passed profile.
    void delete_profile(int profile_to_delete);

    //Select the passed profile.
    void select_profile(int profile_to_select);

    //Create the profile currently represented by creating_profile (a string).
    void create_profile();

    //Returns true if the passed profile already exists.
    //Returns false if it does not exist.
    bool does_profile_exist(std::string profile_name);

    //Retrieve the list of all existing profiles.
    bool load_profile_list();

    //Save the list of all existing profiles.
    bool save_profile_list();

    //Load profile data that is not level-specific.
    bool load_profile_global_data();

    //Save profile data that is not level-specific.
    bool save_profile_global_data();

    //A list of all existing profiles.
    std::vector<std::string> profile_list;
};

#endif
