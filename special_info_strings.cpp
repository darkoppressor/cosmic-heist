#include "special_info_strings.h"
#include "world.h"
#include "version.h"

using namespace std;

special_info_return_data set_special_info_string(short special_info){
    string text="";

    short command=0;

    switch(special_info){

    case SPECIAL_INFO_CURRENT_PROFILE:
        ss.clear();ss.str("");ss<<"Current profile:\xA";ss<<player.name;text=ss.str();
        break;

    case SPECIAL_INFO_NO_PROFILE:
        if(player.name=="\x1F"){
            text="No profiles exist yet,\nso you must create one.";
        }
        else{
            text="";
        }
        break;

    case SPECIAL_INFO_PROFILE_TAKEN:
        if(profile.profile_list.size()==7){
            text="Maximum number of\nprofiles reached.";
        }
        else if(profile.does_profile_exist(creating_profile)){
            text="That profile name\nalready exists.";
        }
        else{
            text="";
        }
        break;

    case SPECIAL_INFO_CREATING_PROFILE:
        text=creating_profile;
        break;

    case SPECIAL_INFO_KEY_TOGGLE_MAIN_MENU:
        command=COMMAND_TOGGLE_MAIN_MENU;
        break;

    case SPECIAL_INFO_KEY_SCREENSHOT:
        command=COMMAND_SCREENSHOT;
        break;

    case SPECIAL_INFO_KEY_LEFT:
        command=COMMAND_LEFT;
        break;

    case SPECIAL_INFO_KEY_UP:
        command=COMMAND_UP;
        break;

    case SPECIAL_INFO_KEY_RIGHT:
        command=COMMAND_RIGHT;
        break;

    case SPECIAL_INFO_KEY_DOWN:
        command=COMMAND_DOWN;
        break;

    case SPECIAL_INFO_KEY_PAUSE:
        command=COMMAND_PAUSE;
        break;

    case SPECIAL_INFO_PROFILE_1:
        text=profile.profile_list[0];
        break;

    case SPECIAL_INFO_PROFILE_2:
        text=profile.profile_list[1];
        break;

    case SPECIAL_INFO_PROFILE_3:
        text=profile.profile_list[2];
        break;

    case SPECIAL_INFO_PROFILE_4:
        text=profile.profile_list[3];
        break;

    case SPECIAL_INFO_PROFILE_5:
        text=profile.profile_list[4];
        break;

    case SPECIAL_INFO_PROFILE_6:
        text=profile.profile_list[5];
        break;

    case SPECIAL_INFO_PROFILE_7:
        text=profile.profile_list[6];
        break;

    case SPECIAL_INFO_VERSION:
        ss.clear();ss.str("");ss<<"Version: ";ss<<AutoVersion::MAJOR;ss<<".";ss<<AutoVersion::MINOR;ss<<" ";ss<<AutoVersion::STATUS;ss<<"\xA";text=ss.str();
        ss.clear();ss.str("");ss<<"Build date: ";ss<<AutoVersion::MONTH;ss<<"-";ss<<AutoVersion::DATE;ss<<"-";ss<<AutoVersion::YEAR;text+=ss.str();
        break;

    }

    special_info_return_data return_data;
    return_data.command=command;
    return_data.text=text;

    return return_data;
}
