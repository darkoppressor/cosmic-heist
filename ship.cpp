/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#include "ship.h"
#include "render.h"
#include "world.h"
#include "collision.h"
#include "sizes.h"
#include "ai_keys.h"
#include "options.h"
#include "positional_audio.h"
#include "counters.h"

using namespace std;

Ship::Ship(double get_x,double get_y,short get_type,bool get_is_player){
    x=get_x;
    y=get_y;
    type=get_type;
    is_player=get_is_player;

    collision_size=0;

    if(type==SHIP_TYPE_PLAYER){
        move_type=SHIP_MOVE_TYPE_FREE;
        accel_type=SHIP_ACCEL_TYPE_GRADUAL;
        health=500.0;
        collision_size=3;
    }
    else if(type==SHIP_TYPE_SPECK){
        move_type=SHIP_MOVE_TYPE_LIMITED;
        accel_type=SHIP_ACCEL_TYPE_INSTANT;
        health=10.0;
        collision_size=3;
    }
    else if(type==SHIP_TYPE_BULLET){
        move_type=SHIP_MOVE_TYPE_FREE;
        accel_type=SHIP_ACCEL_TYPE_GRADUAL;
        health=25.0;
        collision_size=3;
    }
    else if(type==SHIP_TYPE_TORP){
        move_type=SHIP_MOVE_TYPE_FREE;
        accel_type=SHIP_ACCEL_TYPE_GRADUAL;
        health=50.0;
        collision_size=3;
    }

    alive=true;

    angle=0.0;

    move_speed_x=0.0;
    move_speed_y=0.0;

    idle=true;

    for(int i=0;i<4;i++){
        directions[i]=false;
    }

    frame=0;
    frame_counter=0;
    frame_idle=0;
    frame_counter_idle=0;

    counter_engines=COUNTER_ENGINES;
}

double Ship::return_w(bool collision_adjust){
    if(!collision_adjust){
        return ship_sizes[type].w;
    }
    else{
        return ship_sizes[type].w-collision_size*2;
    }
}
double Ship::return_h(bool collision_adjust){
    if(!collision_adjust){
        return ship_sizes[type].h;
    }
    else{
        return ship_sizes[type].h-collision_size*2;
    }
}

void Ship::handle_input_ai(){
    if(alive){
        idle=true;

        for(int i=0;i<4;i++){
            directions[i]=false;
        }

        if(ai_keystates[AIK_LEFT]){
            idle=false;

            directions[LEFT-1]=true;
        }

        if(ai_keystates[AIK_UP]){
            idle=false;

            directions[UP-1]=true;
        }

        if(ai_keystates[AIK_RIGHT]){
            idle=false;

            directions[RIGHT-1]=true;
        }

        if(ai_keystates[AIK_DOWN]){
            idle=false;

            directions[DOWN-1]=true;
        }
    }
}

void Ship::handle_input_player(){
    if(alive){
        idle=true;

        for(int i=0;i<4;i++){
            directions[i]=false;
        }

        if(player.command_state(COMMAND_LEFT)){
            idle=false;

            directions[LEFT-1]=true;
        }

        if(player.command_state(COMMAND_UP)){
            idle=false;

            directions[UP-1]=true;
        }

        if(player.command_state(COMMAND_RIGHT)){
            idle=false;

            directions[RIGHT-1]=true;
        }

        if(player.command_state(COMMAND_DOWN)){
            idle=false;

            directions[DOWN-1]=true;
        }
    }
}

void Ship::move(){
    if(alive){
        double acceleration=0.0;
        double deceleration=0.0;
        double max_speed=0.0;
        double handling=0.0;

        if(type==SHIP_TYPE_PLAYER){
            max_speed=6.0;
            acceleration=0.1;
            handling=2.0;
        }
        else if(type==SHIP_TYPE_SPECK){
            max_speed=2.0;
            acceleration=0.1;
            handling=1.0;
        }
        else if(type==SHIP_TYPE_BULLET){
            max_speed=3.0;
            acceleration=0.2;
            handling=1.0;
        }
        else if(type==SHIP_TYPE_TORP){
            max_speed=4.0;
            acceleration=0.1;
            handling=0.5;
        }

        deceleration=acceleration/2.0;

        if(directions[LEFT-1]){
            idle=false;

            if(accel_type==SHIP_ACCEL_TYPE_GRADUAL){
                if(move_speed_x>0.0){
                    move_speed_x-=acceleration*handling;
                }
                else{
                    move_speed_x-=acceleration;
                }
            }
            else{
                move_speed_x=-max_speed;
            }
        }

        if(directions[UP-1]){
            idle=false;

            if(accel_type==SHIP_ACCEL_TYPE_GRADUAL){
                if(move_speed_y>0.0){
                    move_speed_y-=acceleration*handling;
                }
                else{
                    move_speed_y-=acceleration;
                }
            }
            else{
                move_speed_y=-max_speed;
            }
        }

        if(directions[RIGHT-1]){
            idle=false;

            if(accel_type==SHIP_ACCEL_TYPE_GRADUAL){
                if(move_speed_x<0.0){
                    move_speed_x+=acceleration*handling;
                }
                else{
                    move_speed_x+=acceleration;
                }
            }
            else{
                move_speed_x=max_speed;
            }
        }

        if(directions[DOWN-1]){
            idle=false;

            if(accel_type==SHIP_ACCEL_TYPE_GRADUAL){
                if(move_speed_y<0.0){
                    move_speed_y+=acceleration*handling;
                }
                else{
                    move_speed_y+=acceleration;
                }
            }
            else{
                move_speed_y=max_speed;
            }
        }

        if(move_speed_x>max_speed){
            move_speed_x=max_speed;
        }
        if(move_speed_x<-max_speed){
            move_speed_x=-max_speed;
        }
        if(move_speed_y>max_speed){
            move_speed_y=max_speed;
        }
        if(move_speed_y<-max_speed){
            move_speed_y=-max_speed;
        }

        if(!directions[LEFT-1] && !directions[RIGHT-1]){
            if(accel_type==SHIP_ACCEL_TYPE_INSTANT){
                move_speed_x=0.0;
            }
            else if(accel_type==SHIP_ACCEL_TYPE_GRADUAL){
                if(move_speed_x>0.0){
                    move_speed_x-=deceleration;
                    if(move_speed_x<0.0){
                        move_speed_x=0.0;
                    }
                }
                else if(move_speed_x<0.0){
                    move_speed_x+=deceleration;
                    if(move_speed_x>0.0){
                        move_speed_x=0.0;
                    }
                }
            }
        }
        if(!directions[UP-1] && !directions[DOWN-1]){
            if(accel_type==SHIP_ACCEL_TYPE_INSTANT){
                move_speed_y=0.0;
            }
            else if(accel_type==SHIP_ACCEL_TYPE_GRADUAL){
                if(move_speed_y>0.0){
                    move_speed_y-=deceleration;
                    if(move_speed_y<0.0){
                        move_speed_y=0.0;
                    }
                }
                else if(move_speed_y<0.0){
                    move_speed_y+=deceleration;
                    if(move_speed_y>0.0){
                        move_speed_y=0.0;
                    }
                }
            }
        }

        //This is the maximum number of pixels that the ship will move before calling handle_events().
        double run_chunk=fabs(1);

        for(double i=run_chunk;i>0;){
            //If we have run_chunk or more pixels left to move,
            //we will move run_chunk pixels, call handle_events(), and loop back up here.

            //Or, if we have less than run_chunk pixels left to move,
            //we will move whatever pixels we have left and call handle_events() once more.
            if(i<run_chunk){
                run_chunk=i;
                i=0;
            }

            //Move.
            x+=move_speed_x;
            y+=move_speed_y;

            //If we still have pixels left to move.
            if(i!=0){
                i-=run_chunk;
            }

            handle_events();
        }
    }
}

void Ship::handle_events(){
    if(alive){
        //The current tile location.
        int current_x=(int)((int)x/TILE_W);
        int current_y=(int)((int)y/TILE_H);

        int check_x_start=current_x-4;
        int check_x_end=current_x+4;
        int check_y_start=current_y-4;
        int check_y_end=current_y+4;

        //First, we check for collisions with tiles.

        for(int int_y=check_y_start;int_y<check_y_end;int_y++){
            for(int int_x=check_x_start;int_x<check_x_end;int_x++){

                //As long as the current tile is within the level's boundaries.
                if(int_x>=0 && int_x<=(levels[0].level_x)-1 && int_y>=0 && int_y<=(levels[0].level_y)-1){

                    if(levels[0].tile_array[int_x][int_y].solid){
                        if(collision_check(x+collision_size,y+collision_size,return_w(true),return_h(true),levels[0].tile_array[int_x][int_y].x+TILE_COLLISION_SIZE,levels[0].tile_array[int_x][int_y].y+TILE_COLLISION_SIZE,TILE_W-TILE_COLLISION_SIZE*2,TILE_H-TILE_COLLISION_SIZE*2)){
                            handle_damage(0.0);
                        }
                    }
                }
            }
        }

        for(int i=0;i<ships.size();i++){
            if(ships[i].alive && (is_player || (!is_player && this!=&ships[i]))){
                if(collision_check(x+collision_size,y+collision_size,return_w(true),return_h(true),ships[i].x+ships[i].collision_size,ships[i].y+ships[i].collision_size,ships[i].return_w(true),ships[i].return_h(true))){
                    handle_damage((fabs(ships[i].move_speed_x)+fabs(ships[i].move_speed_y))/2.0);
                    ships[i].handle_damage((move_speed_x+move_speed_y)/2.0);
                }
            }
        }

        if(!is_player){
            if(player_ship[0].alive && collision_check(x+collision_size,y+collision_size,return_w(true),return_h(true),player_ship[0].x+player_ship[0].collision_size,player_ship[0].y+player_ship[0].collision_size,player_ship[0].return_w(true),player_ship[0].return_h(true))){
                handle_damage((fabs(player_ship[0].move_speed_x)+fabs(player_ship[0].move_speed_y))/2.0);
                player_ship[0].handle_damage((move_speed_x+move_speed_y)/2.0);
            }
        }

        if(is_player){
            for(int i=0;i<exits.size();i++){
                if(collision_check(x+collision_size,y+collision_size,return_w(true),return_h(true),exits[i].x+EXIT_COLLISION_SIZE,exits[i].y+EXIT_COLLISION_SIZE,EXIT_W-EXIT_COLLISION_SIZE*2,EXIT_H-EXIT_COLLISION_SIZE*2)){
                    levels[0].complete_level();
                }
            }
        }

        //Keep the ship in the level map's bounds.
        if(x<0){
            x=0;
            move_speed_x=0.0;
        }
        if(x+return_w()>levels[0].level_x*TILE_W){
            x=levels[0].level_x*TILE_W-return_w();
            move_speed_x=0.0;
        }
        if(y<0){
            y=0;
            move_speed_y=0.0;
        }
        if(y+return_h()>levels[0].level_y*TILE_H){
            y=levels[0].level_y*TILE_H-return_h();
            move_speed_y=0.0;
        }
    }
}

void Ship::handle_damage(double object_speed){
    if(alive){
        int damage=1;

        if(!is_player){
            damage=5;
        }

        double speed_avg=(fabs(move_speed_x)+fabs(move_speed_y))/2.0;

        damage+=speed_avg+object_speed;

        health-=damage;

        if(health<=0){
            alive=false;

            //Create explosion.
            explosions.push_back(Explosion(x,y,(ship_sizes[type].w*ship_sizes[type].h)/(ship_sizes[SHIP_TYPE_PLAYER].w*ship_sizes[SHIP_TYPE_PLAYER].h),random_range(EXPLOSION_BEGIN,EXPLOSION_END)));

            if(!is_player){
                player.score+=10;
            }
            else{
                //End game.
                play_positional_sound(sound_game_over);

                player.save_highscore();
                string temp_name=player.name;

                player.stop_game();

                player.name=temp_name;
                options_load();
                profile.load_profile_global_data();
                save_current_profile();

                window_manager.close_windows(0);
                window_highscores[0].update_highscores();
                window_highscores[0].toggle_on();
            }
        }
        //If the ship did not die.
        else{
            if(is_player){
                explosions.push_back(Explosion(x+random_range(0,20),y+random_range(0,20),0.2,random_range(EXPLOSION_BEGIN,EXPLOSION_END)));
            }
        }
    }
}

void Ship::animate(){
    if(alive){
        if((directions[LEFT-1] || directions[UP-1] || directions[RIGHT-1] || directions[DOWN-1]) && --counter_engines<=0){
            counter_engines=COUNTER_ENGINES;
            if(is_player){
                play_positional_sound(sound_engines,x,y);
            }
            else{
                play_positional_sound(sound_engines,x,y,32);
            }
        }

        //Increment the frame counter.
        frame_counter++;
        frame_counter_idle++;

        if(frame_counter>=5){
            frame_counter=0;

            //Now increment the frame.
            frame++;

            if(frame>SHIP_SPRITE_COUNTS[type].walk_sprites-1){
                frame=0;
            }
        }

        //Handle idle animation.
        if(frame_counter_idle>=10){
            frame_counter_idle=0;

            //Now increment the frame.
            frame_idle++;

            if(frame_idle>SHIP_SPRITE_COUNTS[type].idle_sprites-1){
                frame_idle=0;
            }
        }
    }
}

void Ship::render(){
    if(alive && x>=player.camera_x-return_w() && x<=player.camera_x+player.camera_w && y>=player.camera_y-return_h() && y<=player.camera_y+player.camera_h){
        if(directions[DOWN-1]){
            angle=270;
        }
        if(directions[RIGHT-1]){
            angle=0;
        }
        if(directions[UP-1]){
            angle=90;
        }
        if(directions[LEFT-1]){
            angle=180;
        }

        if(directions[LEFT-1] && directions[UP-1]){
            angle=135;
        }
        if(directions[RIGHT-1] && directions[UP-1]){
            angle=45;
        }
        if(directions[RIGHT-1] && directions[DOWN-1]){
            angle=315;
        }
        if(directions[LEFT-1] && directions[DOWN-1]){
            angle=225;
        }
        if(directions[LEFT-1] && directions[RIGHT-1]){
            angle=180;
        }
        if(directions[UP-1] && directions[DOWN-1]){
            angle=90;
        }
        if(directions[LEFT-1] && directions[UP-1] && directions[RIGHT-1]){
            angle=135;
        }
        if(directions[LEFT-1] && directions[DOWN-1] && directions[RIGHT-1]){
            angle=225;
        }
        if(directions[LEFT-1] && directions[UP-1] && directions[DOWN-1]){
            angle=135;
        }
        if(directions[UP-1] && directions[RIGHT-1] && directions[DOWN-1]){
            angle=45;
        }
        if(directions[LEFT-1] && directions[UP-1] && directions[RIGHT-1] && directions[DOWN-1]){
            angle=135;
        }

        if(idle){
            render_sprite((int)(x-player.camera_x),(int)(y-player.camera_y),image.ships,&SPRITES_SHIPS[type].idle[frame_idle],1.0,1.0,1.0,angle,COLOR_WHITE);
        }
        else{
            render_sprite((int)(x-player.camera_x),(int)(y-player.camera_y),image.ships,&SPRITES_SHIPS[type].walk[frame],1.0,1.0,1.0,angle,COLOR_WHITE);
        }

        if(player.option_dev){
            ///render_rectangle_empty((int)(x-player.camera_x),(int)(y-player.camera_y),return_w(),return_h(),1.0,COLOR_GREEN);
            render_rectangle_empty((int)(x+collision_size-player.camera_x),(int)(y+collision_size-player.camera_y),return_w(true),return_h(true),1.0,COLOR_RED);
        }
    }
}
