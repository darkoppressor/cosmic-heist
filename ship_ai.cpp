#include "ship.h"
#include "world.h"
#include "collision.h"
#include "ai_keys.h"

using namespace std;

void Ship::handle_ai(){
    if(alive){
        //Reset AI keystates.
        for(short i=0;i<256;i++){
            ai_keystates[i]=false;
        }

        double follow_range=0.0;

        if(type==SHIP_TYPE_PLAYER){
            follow_range=2500;
        }
        else if(type==SHIP_TYPE_SPECK){
            follow_range=2000;
        }
        else if(type==SHIP_TYPE_BULLET){
            follow_range=1500;
        }
        else if(type==SHIP_TYPE_TORP){
            follow_range=1000;
        }

        //If the player is in range.
        if(collision_check(x-follow_range,y-follow_range,return_w()+follow_range*2,return_h()+follow_range*2,player_ship[0].x,player_ship[0].y,player_ship[0].return_w(),player_ship[0].return_h())){
            double target_x=player_ship[0].x;
            double target_y=player_ship[0].y;
            double target_w=player_ship[0].return_w();
            double target_h=player_ship[0].return_h();

            double this_width_box_x=(x+return_w()/2.0)-2;
            double this_width_box_y=(x+return_w()/2.0)-2;
            double this_width_box_w=5;
            double this_width_box_h=5;

            double this_height_box_x=(y+return_h()/2.0)-2;
            double this_height_box_y=(y+return_h()/2.0)-2;
            double this_height_box_w=5;
            double this_height_box_h=5;

            double target_width_box_x=(target_x+target_w/2.0)-2;
            double target_width_box_y=(target_x+target_w/2.0)-2;
            double target_width_box_w=5;
            double target_width_box_h=5;

            double target_height_box_x=(target_y+target_h/2.0)-2;
            double target_height_box_y=(target_y+target_h/2.0)-2;
            double target_height_box_w=5;
            double target_height_box_h=5;

            if(!collision_check(this_width_box_x,this_width_box_y,this_width_box_w,this_width_box_h,target_width_box_x,target_width_box_y,target_width_box_w,target_width_box_h)){
                if(x+return_w()/2.0>target_x+target_w/2.0){
                    ai_keystates[AIK_LEFT]=true;
                }
                else if(x+return_w()/2.0<target_x+target_w/2.0){
                    ai_keystates[AIK_RIGHT]=true;
                }
            }
            if(!collision_check(this_height_box_x,this_height_box_y,this_height_box_w,this_height_box_h,target_height_box_x,target_height_box_y,target_height_box_w,target_height_box_h)){
                if(y+return_h()/2.0>target_y+target_h/2.0){
                    ai_keystates[AIK_UP]=true;
                }
                else if(y+return_h()/2.0<target_y+target_h/2.0){
                    ai_keystates[AIK_DOWN]=true;
                }
            }

            if(move_type==SHIP_MOVE_TYPE_LIMITED){
                if(ai_keystates[AIK_LEFT] && move_speed_y!=0.0){
                    ai_keystates[AIK_LEFT]=false;
                }
                if(ai_keystates[AIK_RIGHT] && move_speed_y!=0.0){
                    ai_keystates[AIK_RIGHT]=false;
                }
                if(ai_keystates[AIK_UP] && move_speed_x!=0.0){
                    ai_keystates[AIK_UP]=false;
                }
                if(ai_keystates[AIK_DOWN] && move_speed_x!=0.0){
                    ai_keystates[AIK_DOWN]=false;
                }
            }
        }
    }
}
