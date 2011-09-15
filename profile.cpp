#include "profile.h"
#include "world.h"
#include "render.h"
#include "version.h"
#include "string_stuff.h"
#include "options.h"

#include <fstream>
#include <boost/filesystem.hpp>

using namespace std;

Profile::Profile(){
}

void Profile::make_directories(){
    string temp="";

    boost::filesystem::create_directory("profiles");

    //If there is a profile.
    if(player.name!="\x1F"){
        temp="profiles/";
        temp+=player.name;
        boost::filesystem::create_directory(temp.c_str());

        temp="profiles/";
        temp+=player.name;
        temp+="/screenshots";
        boost::filesystem::create_directory(temp.c_str());
    }
}

void Profile::delete_profile(int profile_to_delete){
    //As long as the profile to be deleted is not the current profile.
    if(!(profile_list[profile_to_delete]==player.name)){
        string temp="profiles/";
        temp+=profile_list[profile_to_delete];
        boost::filesystem::remove_all(temp.c_str());

        profile_list.erase(profile_list.begin()+profile_to_delete);

        save_profile_list();

        set_change_profile_buttons();
    }
}

void Profile::select_profile(int profile_to_select){
    options_save();
    save_profile_global_data();
    save_current_profile();
    save_profile_list();

    //If a game was in progress with the previous profile, it should be stopped.
    if(player.game_in_progress){
        player.save_highscore();
    }
    player.stop_game();

    //Set the profile name.
    player.name=profile_list[profile_to_select];

    //Load this new profile.
    options_load();
    load_profile_global_data();

    save_current_profile();
}

void Profile::create_profile(){
    if(creating_profile.length()>0 && !does_profile_exist(creating_profile)){
        options_save();
        save_profile_global_data();
        save_current_profile();
        save_profile_list();

        //If a game was in progress with the previous profile, it should be stopped.
        if(player.game_in_progress){
            player.save_highscore();
        }
        player.stop_game();

        //Set the profile name.
        player.name=creating_profile;

        //Create a new entry in the profile list for this profile.
        profile_list.push_back(string(creating_profile));

        //Save this new profile.
        options_save();
        save_profile_global_data();
        save_current_profile();
        save_profile_list();

        window_manager.close_windows(0);
        vector_windows[WINDOW_MAIN_MENU].turn_on();
    }
}

bool Profile::does_profile_exist(string profile_name){
    string profile_string=make_string_lower_case(profile_name);
    string temp="";

    for(int i=0;i<profile_list.size();i++){
        temp=make_string_lower_case(profile_list[i]);
        if(temp==profile_string){
            return true;
        }
    }

    return false;
}

bool Profile::load_profile_list(){
    string temp="";
    int number_of_profiles=0;

    profile_list.clear();

    ifstream load;
    string file_to_load="profiles/profile_list.cfg";
    load.open(file_to_load.c_str(),ifstream::in);

    if(load!=NULL){
        load>>number_of_profiles;

        for(int i=0;i<number_of_profiles;i++){
            load>>temp;

            profile_list.push_back(string(temp));
        }

        load.close();
        load.clear();
    }
    else{
        if(!save_profile_list()){
            return false;
        }
    }

    return true;
}

bool Profile::save_profile_list(){
    make_directories();
    ofstream save;
    string save_name="profiles/profile_list.cfg";
    save.open(save_name.c_str());

    if(save!=NULL){

        save<<profile_list.size();
        save<<"\n";

        for(int i=0;i<profile_list.size();i++){
            save<<profile_list[i];
            save<<"\n";
        }

        save.close();
        save.clear();
    }
    else{
        return false;
    }

    return true;
}

bool Profile::load_profile_global_data(){
    ///if(!load_inventory()){
        ///return false;
    ///}

    return true;
}

bool Profile::save_profile_global_data(){
    ///if(!save_inventory()){
        ///return false;
    ///}

    return true;
}
