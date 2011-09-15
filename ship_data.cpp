#include "ship_data.h"
#include "enumerations.h"

using namespace std;

vector<ship_sprite_count_data> SHIP_SPRITE_COUNTS;

void prepare_ship_sprite_counts(){
    SHIP_SPRITE_COUNTS.clear();

    for(short i=SHIP_TYPE_BEGIN;i<SHIP_TYPE_END;i++){
        SHIP_SPRITE_COUNTS.push_back(ship_sprite_count_data());
    }

    SHIP_SPRITE_COUNTS[SHIP_TYPE_PLAYER].walk_sprites=2;
    SHIP_SPRITE_COUNTS[SHIP_TYPE_PLAYER].idle_sprites=1;

    SHIP_SPRITE_COUNTS[SHIP_TYPE_SPECK].walk_sprites=2;
    SHIP_SPRITE_COUNTS[SHIP_TYPE_SPECK].idle_sprites=1;

    SHIP_SPRITE_COUNTS[SHIP_TYPE_BULLET].walk_sprites=2;
    SHIP_SPRITE_COUNTS[SHIP_TYPE_BULLET].idle_sprites=1;

    SHIP_SPRITE_COUNTS[SHIP_TYPE_TORP].walk_sprites=2;
    SHIP_SPRITE_COUNTS[SHIP_TYPE_TORP].idle_sprites=1;
}
