#ifndef special_info_strings_h
#define special_info_strings_h

#include <string>

struct special_info_return_data{
    std::string text;
    short command;
};

special_info_return_data set_special_info_string(short special_info);

#endif
