/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "string_stuff.h"

#include <algorithm>

using namespace std;

string make_string_lower_case(string string_input){
    string string_output="";
    char temp=0;

    for(int i=0;i<string_input.size();i++){
        temp=(char)tolower(string_input[i]);
        string_output+=temp;
    }

    return string_output;
}
