/* Copyright (c) 2011 Kevin Wells */
/* Cosmic Heist may be freely redistributed.  See license for details. */

#ifndef enumerations_h
#define enumerations_h

//******************//
// Explosion types: //
//******************//

enum{
    EXPLOSION_BEGIN,
    EXPLOSION_1=EXPLOSION_BEGIN,
    EXPLOSION_2,
    EXPLOSION_3,
    EXPLOSION_END
};

//*************//
// Ship types: //
//*************//

enum{
    SHIP_TYPE_BEGIN,
    SHIP_TYPE_PLAYER=SHIP_TYPE_BEGIN,
    SHIP_TYPE_SPECK,
    SHIP_TYPE_BULLET,
    SHIP_TYPE_TORP,
    SHIP_TYPE_END
};

//**********************//
// Ship movement types: //
//**********************//

enum{
    SHIP_MOVE_TYPE_LIMITED,
    SHIP_MOVE_TYPE_FREE
};

//*******************//
// Ship accel types: //
//*******************//

enum{
    SHIP_ACCEL_TYPE_INSTANT,
    SHIP_ACCEL_TYPE_GRADUAL
};

//***************//
// Tile sprites: //
//***************//

enum{
    TILE_SPRITE_BEGIN,
    TILE_SPRITE_EMPTY=TILE_SPRITE_BEGIN,
    TILE_SPRITE_ROCK_1,
    TILE_SPRITE_ROCK_2,
    TILE_SPRITE_ROCK_3,
    TILE_SPRITE_METAL_1,
    TILE_SPRITE_METAL_2,
    TILE_SPRITE_METAL_3,
    TILE_SPRITE_END
};

//********//
// Music: //
//********//

enum{
    MUSIC_MENU,
    MUSIC_LEVEL_1
};

//*******************//
// HUD Button names: //
//*******************//

//These need to be in the exact order that the hud buttons are created. See create_hud_buttons() in world.cpp.
enum{
    HUD_BUTTON_BEGIN,
    ///HUD_BUTTON_=HUD_BUTTON_BEGIN,
    HUD_BUTTON_END
};

//***************//
// Window names: //
//***************//

//These need to be in the exact order that the windows are created. See create_windows() in world.cpp.
enum{
    WINDOW_BEGIN,
    WINDOW_MAIN_MENU=WINDOW_BEGIN,
    WINDOW_CREATE_PROFILE,
    WINDOW_CHANGE_PROFILE,
    WINDOW_OPTIONS,
    WINDOW_OPTIONS_KEYS,
    WINDOW_ABOUT,
    WINDOW_CREDITS,
    WINDOW_HELP,
    WINDOW_END
};

//******************************//
// Special information strings: //
//******************************//

enum{
    NO_SPECIAL_INFO,
    SPECIAL_INFO_CURRENT_PROFILE,
    SPECIAL_INFO_NO_PROFILE,
    SPECIAL_INFO_PROFILE_TAKEN,
    SPECIAL_INFO_CREATING_PROFILE,
    SPECIAL_INFO_CHECKBOX_FULLSCREEN,
    SPECIAL_INFO_CHECKBOX_FPS,
    SPECIAL_INFO_CHECKBOX_MUTE_GLOBAL,
    SPECIAL_INFO_CHECKBOX_MUTE_SOUND,
    SPECIAL_INFO_CHECKBOX_MUTE_MUSIC,
    SPECIAL_INFO_VOLUME_GLOBAL,
    SPECIAL_INFO_VOLUME_SOUND,
    SPECIAL_INFO_VOLUME_MUSIC,
    SPECIAL_INFO_KEY_TOGGLE_MAIN_MENU,
    SPECIAL_INFO_KEY_SCREENSHOT,
    SPECIAL_INFO_KEY_LEFT,
    SPECIAL_INFO_KEY_UP,
    SPECIAL_INFO_KEY_RIGHT,
    SPECIAL_INFO_KEY_DOWN,
    SPECIAL_INFO_KEY_PAUSE,
    SPECIAL_INFO_PROFILE_1,
    SPECIAL_INFO_PROFILE_2,
    SPECIAL_INFO_PROFILE_3,
    SPECIAL_INFO_PROFILE_4,
    SPECIAL_INFO_PROFILE_5,
    SPECIAL_INFO_PROFILE_6,
    SPECIAL_INFO_PROFILE_7,
    SPECIAL_INFO_VERSION
};

//****************//
// Special input: //
//****************//

enum{
    NO_SPECIAL_INPUT,
    SPECIAL_INPUT_CREATE_PROFILE
};

//*********//
// Colors: //
//*********//

enum{
    COLOR_RAINBOW,

    COLOR_BLACK,

    COLOR_GRAY,
    COLOR_LIGHT_GRAY,
    COLOR_DARK_GRAY,
    COLOR_PAYNES_GRAY,
    COLOR_DARKER_GRAY,

    COLOR_WHITE,

    COLOR_BROWN,
    COLOR_DARK_BROWN,
    COLOR_COPPER,
    COLOR_BRONZE,
    COLOR_SEPIA,
    COLOR_SANDY_BROWN,
    COLOR_WHEAT,
    COLOR_TAN,
    COLOR_PALE_BROWN,

    COLOR_YELLOW,
    COLOR_SYSTEM,
    COLOR_GOLD,

    COLOR_ORANGE,
    COLOR_PUMPKIN,

    COLOR_RED,
    COLOR_DARK_RED,
    COLOR_FLAME,

    COLOR_PINK,
    COLOR_ROSE,
    COLOR_SHOCKING_PINK,

    COLOR_PURPLE,
    COLOR_VIOLET,
    COLOR_INDIGO,

    COLOR_BLUE,
    COLOR_SKY_BLUE,
    COLOR_UN_BLUE,

    COLOR_GREEN,
    COLOR_JUNGLE,
    COLOR_SPRING
};

//***************//
// Which Window: //
//***************//

enum{
    WHICH_WINDOW_HIGHSCORES,
    WHICH_WINDOW_OTHER
};

//***********//
// Commands: //
//***********//

enum{
    COMMAND_TOGGLE_MAIN_MENU,
    COMMAND_SCREENSHOT,
    COMMAND_LEFT,
    COMMAND_UP,
    COMMAND_RIGHT,
    COMMAND_DOWN,
    COMMAND_PAUSE
};

//**************//
// Input types: //
//**************//

enum{
    INPUT_TYPE_KEYBOARD,
    INPUT_TYPE_JOYSTICK_BUTTON,
    INPUT_TYPE_JOYSTICK_AXIS,
    INPUT_TYPE_JOYSTICK_HAT,
    INPUT_TYPE_JOYSTICK_BALL
};

//**************************//
// Joystick neutral values: //
//**************************//

enum{
    JOYSTICK_NEUTRAL_NEGATIVE=-8000,
    JOYSTICK_NEUTRAL_POSITIVE=8000
};

//*************//
// Directions: //
//*************//

enum{
    NONE,
    LEFT,
    UP,
    RIGHT,
    DOWN,
    LEFT_UP,
    RIGHT_UP,
    RIGHT_DOWN,
    LEFT_DOWN,
    //This constant is used as a cam_state value, to tell the camera whether it is free or attached to a player.
    CAM_STICKY
};

//********************//
// Button Properties: //
//********************//

enum{
    BUTTON_VISIBLE,
    BUTTON_INVISIBLE
};
enum{
    BUTTON_DISABLED,
    BUTTON_ENABLED
};

#endif
