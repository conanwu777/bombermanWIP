//
// Created by Hugo JEANNIN on 5/21/17.
//

#ifndef CONSTANT_HPP
# define CONSTANT_HPP

#ifdef _WIN32
# define _CRTDBG_MAP_ALLOC
# include<iostream>
# include <crtdbg.h>
# ifdef _DEBUG
#  define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#  define new DEBUG_NEW
# endif
#endif

//////////////
//  Config  //
//////////////

//#define FULL_SCREEN (true)
#define FULL_SCREEN (false)

//#define CONFIG_5k
//#define CONFIG_2_5k
//#define CONFIG_1080p
#define CONFIG_DEV

#ifdef CONFIG_5k

#  define SAMPLE_RATE 		9
#  define INITIAL_RES_X		5120
#  define INITIAL_RES_Y		2880

#endif
#ifdef CONFIG_2_5k

#  define SAMPLE_RATE 		1
#  define INITIAL_RES_X		2560
#  define INITIAL_RES_Y		1440

#endif

#ifdef CONFIG_1080p

#  define SAMPLE_RATE       4
#  define INITIAL_RES_X     1920
#  define INITIAL_RES_Y     1080

#endif

#ifdef CONFIG_DEV

#  define SAMPLE_RATE        1
#  define INITIAL_RES_X        1600
#  define INITIAL_RES_Y        900

#endif

//#define CURSOR_MODE		0x00034001		//NORMAL
#define CURSOR_MODE        0x00034002        //HIDDEN
//#define CURSOR_MODE		0x00034003		//DISABLED

#define CAMERA_CFG_AUTO_LOAD    true
//#define CAMERA_CFG_AUTO_LOAD    false

//#define ANIMATION_VERBOSE   true
#define ANIMATION_VERBOSE   false

#define TICK_PER_SECOND     60.0f
#define TPS                 TICK_PER_SECOND

//////////////
//  OpenGL  //
//////////////

# define SHADER_NUM         4
# define TEX_NUM            4

enum textures
{
    DEFAULT_GENERATED_TEXTURE,
    SKYBOX_TEXTURE,
    CHECKER_TEXTURE,
    FONT_TEXTURE,
};

enum shaders
{
    AXES_SHADER,
    SKYBOX_SHADER,
    TEXT_SHADER,
    OBJ_SHADER,
};

enum data_formatting_status
{
    UNDETERMINED,
    INTERLEAVED_FAT_DATA,
    INDEXED_INTERLEAVED_DATA,
};

//////////////
//  OpenCL  //
//////////////

# define KERNELS_NUMBER 1

static char const *CLProgramNames[KERNELS_NUMBER] =
{
        "generateBlock"
};

static char const *CLKernelFiles[KERNELS_NUMBER] =
{
        "../kernels/generateBlock.h"
};

enum kernels
{
    GENERATE_BLOCK
};

////////////////////
//  Game Objects  //
////////////////////

enum objectType
{
    PLAYER_GAME,
    BOMB_GAME,
    WALL_GAME,
    BREAKABLE_WALL_GAME,
    FIRE_GAME,
    BOMB_POWER_UP_GAME,
    FIRE_POWER_UP_GAME,
};

#endif //CONSTANT_HPP
