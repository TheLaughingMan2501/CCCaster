#pragma once

#include <cstdint>
#include <iostream>

#include "Controller.hpp"

// For Steam Version
// Number of frames of inputs to send per message
#define NUM_INPUTS                  ( 30 )

// Max allow rollback frames
#define MAX_ROLLBACK                ( 15 )

// Number of rollback states to allocate
#ifdef RELEASE
#define NUM_ROLLBACK_STATES         ( 60 )
#else
#define NUM_ROLLBACK_STATES         ( 256 )
#endif


// Game constants and addresses are prefixed CC
#define CC_VERSION                  ""
#define CC_TITLE                    "MELTY BLOOD Actress Again Current Code" CC_VERSION
#define CC_STARTUP_TITLE            CC_TITLE " "
#define CC_STARTUP_BUTTON           "OK"
#define CC_NETWORK_CONFIG_FILE      "System\\NetConnect.dat"
#define CC_NETWORK_USERNAME_KEY     "UserName"
#define CC_APP_CONFIG_FILE          "System\\_App.ini"
#define CC_APP_WINDOW_MODE_KEY      "Windowed"

// Location of the keyboard config in the binary
#define CC_KEYBOARD_CONFIG_OFFSET   ( 0x1B3540 )

#define CC_WINDOW_PROC_ADDR         ( ( char * )     0x438EE0 ) // Location of WindowProc
#define CC_LOOP_START_ADDR          ( ( char * )     0x438CD7 ) // Start of the main event loop
#define CC_SCREEN_WIDTH_ADDR        ( ( uint32_t * ) 0x5B4D34 ) // The actual width of the main viewport
#define CC_DAMAGE_LEVEL_ADDR        ( ( uint32_t * ) 0x5BAF8C ) // Damage level: default 2
#define CC_WIN_COUNT_VS_ADDR        ( ( uint32_t * ) 0x5BAF9C ) // Win count: default 2
#define CC_TIMER_SPEED_ADDR         ( ( uint32_t * ) 0x5BAF90 ) // Timer speed: default 2
#define CC_AUTO_REPLAY_SAVE_ADDR    ( ( uint32_t * ) 0x5BAFA8 ) // Auto replay saving: 0 to disable, 1 to enable
#define CC_WORLD_TIMER_ADDR         ( ( uint32_t * ) 0x5C4418 ) // Frame step timer, always counting up
#define CC_PAUSE_FLAG_ADDR          ( ( uint8_t * )  0x5C441F ) // 1 when paused
#define CC_SKIP_FRAMES_ADDR         ( ( uint32_t * ) 0x5C5100 ) // Set to N to skip rendering for N frames
#define CC_ROUND_TIMER_ADDR         ( ( uint32_t * ) 0x5C9C58 ) // Counts down from 4752, may stop
#define CC_REAL_TIMER_ADDR          ( ( uint32_t * ) 0x5C4414 ) // Counts up from 0 after round start
#define CC_TRAINING_PAUSE_ADDR      ( ( uint32_t * ) 0x5C9C7C ) // 1 when paused
#define CC_VERSUS_PAUSE_ADDR        ( ( uint32_t * ) 0x5CB660 ) // 0xFFFFFFFF when paused
#define CC_P1_GAME_POINT_FLAG_ADDR  ( ( uint32_t * ) 0x5C07A8 ) // P1 game point flag
#define CC_P2_GAME_POINT_FLAG_ADDR  ( ( uint32_t * ) 0x5C07D8 ) // P2 game point flag
#define CC_P1_WINS_ADDR             ( ( uint32_t * ) 0x7D7104 ) // P1 number of wins
#define CC_P2_WINS_ADDR             ( ( uint32_t * ) 0x7D83A4 ) // P2 number of wins
#define CC_ROUND_COUNT_ADDR         ( ( uint32_t * ) 0x5B4DD4 ) // Round count
#define CC_STAGE_ANIMATION_OFF_ADDR ( ( uint32_t * ) 0x5BB0E4 ) // 1 if stage animations are off
#define CC_INTRO_STATE_ADDR         ( ( uint8_t * )  0x5CA79A ) // 2 (character intros), 1 (pre-game), 0 (in-game)
#define CC_HIT_SPARKS_ADDR          ( ( uint32_t * ) 0x684CB4 ) // Number of hit sparks?

#define CC_STAGE_SELECTOR_ADDR      ( ( uint32_t * ) 0x7B6228 ) // Currently selected stage, can be assigned to directly
#define CC_FPS_COUNTER_ADDR         ( ( uint32_t * ) 0x7DC2B0 ) // Value of the displayed FPS counter
#define CC_PERF_FREQ_ADDR           ( ( uint64_t * ) 0x7DC2C0 ) // Value of QueryPerformanceFrequency for game FPS
#define CC_SKIPPABLE_FLAG_ADDR      ( ( uint32_t * ) 0x7B450C ) // Flag that indicates a skippable state when in-game
#define CC_ALIVE_FLAG_ADDR          ( ( uint8_t * )  0x5BAB93 ) // Flag that indicates the game is alive

// Some menu state counter, incremented for each open menu, decremented when menu closes
#define CC_MENU_STATE_COUNTER_ADDR  ( ( uint32_t * ) 0x7CDAB4 )

#define CC_DUMMY_STATUS_ADDR        ( ( int32_t * )  0x7B4324 ) // Training mode dummy status
#define CC_DUMMY_STATUS_STAND       ( 0 )
#define CC_DUMMY_STATUS_JUMP        ( 1 )
#define CC_DUMMY_STATUS_CROUCH      ( 2 )
#define CC_DUMMY_STATUS_CPU         ( 3 )
#define CC_DUMMY_STATUS_MANUAL      ( 4 )
#define CC_DUMMY_STATUS_DUMMY       ( 5 )
#define CC_DUMMY_STATUS_RECORD      ( -1 )

#define CC_PTR_TO_WRITE_INPUT_ADDR  ( ( char * )     0x5CA9B8 ) // Pointer to the location to write game input
#define CC_P1_OFFSET_DIRECTION      ( 0x18 )                    // Offset to write P1 direction input
#define CC_P1_OFFSET_BUTTONS        ( 0x24 )                    // Offset to write P1 buttons input
#define CC_P2_OFFSET_DIRECTION      ( 0x2C )                    // Offset to write P2 direction input
#define CC_P2_OFFSET_BUTTONS        ( 0x38 )                    // Offset to write P2 buttons input

// Directions are just written in numpad format, EXCEPT neutral is 0
#define CC_BUTTON_A                 ( 0x0010 )
#define CC_BUTTON_B                 ( 0x0020 )
#define CC_BUTTON_C                 ( 0x0008 )
#define CC_BUTTON_D                 ( 0x0004 )
#define CC_BUTTON_E                 ( 0x0080 )
#define CC_BUTTON_AB                ( 0x0040 )
#define CC_BUTTON_START             ( 0x0001 )
#define CC_BUTTON_FN1               ( 0x0100 ) // Control dummy
#define CC_BUTTON_FN2               ( 0x0200 ) // Training reset
#define CC_BUTTON_CONFIRM           ( 0x0400 )
#define CC_BUTTON_CANCEL            ( 0x0800 )

#define CC_GAME_MODE_ADDR           ( ( uint32_t * ) 0x5B391C ) // Current game mode, constants below

// List of game modes relevant to netplay
#define CC_GAME_MODE_STARTUP        ( 65535 )
#define CC_GAME_MODE_OPENING        ( 3 )
#define CC_GAME_MODE_TITLE          ( 2 )
#define CC_GAME_MODE_LOADING_DEMO   ( 13 )
#define CC_GAME_MODE_HIGH_SCORES    ( 11 )
#define CC_GAME_MODE_MAIN           ( 25 )
#define CC_GAME_MODE_CHARA_SELECT   ( 20 )
#define CC_GAME_MODE_LOADING        ( 8 )
#define CC_GAME_MODE_IN_GAME        ( 1 )
#define CC_GAME_MODE_RETRY          ( 5 )

// Character select data, can be assigned to directly at the character select screen
#define CC_P1_SELECTOR_MODE_ADDR    ( ( uint32_t * ) 0x7B4424 )
#define CC_P1_CHARA_SELECTOR_ADDR   ( ( uint32_t * ) 0x7B4430 )
#define CC_P1_CHARACTER_ADDR        ( ( uint32_t * ) 0x7B4434 )
#define CC_P1_MOON_SELECTOR_ADDR    ( ( uint32_t * ) 0x7B4438 )
#define CC_P1_COLOR_SELECTOR_ADDR   ( ( uint32_t * ) 0x7B443C )
#define CC_P1_RANDOM_COLOR_ADDR     ( ( uint8_t * ) ( ( * ( uint32_t * ) 0x7B433C ) + 0 * 0x1DC + 0x2C + 0x0C ) )
#define CC_P2_SELECTOR_MODE_ADDR    ( ( uint32_t * ) 0x7B4448 )
#define CC_P2_CHARA_SELECTOR_ADDR   ( ( uint32_t * ) 0x7B4454 )
#define CC_P2_CHARACTER_ADDR        ( ( uint32_t * ) 0x7B4458 )
#define CC_P2_MOON_SELECTOR_ADDR    ( ( uint32_t * ) 0x7B445C )
#define CC_P2_COLOR_SELECTOR_ADDR   ( ( uint32_t * ) 0x7B4461 )
#define CC_P2_RANDOM_COLOR_ADDR     ( ( uint8_t * ) ( ( * ( uint32_t * ) 0x7B433C ) + 1 * 0x1DC + 0x2C + 0x0C ) )

// Complete RngState
#define CC_RNG_STATE0_ADDR          ( ( uint32_t * ) 0x5CA9BC )
#define CC_RNG_STATE1_ADDR          ( ( uint32_t * ) 0x5CA9C0 )
#define CC_RNG_STATE2_ADDR          ( ( uint32_t * ) 0x5CB2B8 )
#define CC_RNG_STATE3_ADDR          ( ( char * )     0x5CB2C0 )
#define CC_RNG_STATE3_SIZE          ( 220 )

// Character select selection mode
#define CC_SELECT_CHARA             ( 0 )
#define CC_SELECT_MOON              ( 1 )
#define CC_SELECT_COLOR             ( 2 )

// Total size of a single player structure.
// Note: there are FOUR player structs in memory, due to the puppet characters.
#define CC_PLR_STRUCT_SIZE          ( 0xAFC )

// Player addresses
// P1 start from 0x5BC370
// P2 start from 0x5BCE6C
#define CC_P1_ENABLED_FLAG_ADDR     ( ( uint8_t * )  0x5BC370 )
#define CC_P1_SEQUENCE_ADDR         ( ( uint32_t * ) 0x5BC380 )
#define CC_P1_SEQ_STATE_ADDR        ( ( uint32_t * ) 0x5BC384 )
#define CC_P1_HEALTH_ADDR           ( ( uint32_t * ) 0x5BC42C )
#define CC_P1_RED_HEALTH_ADDR       ( ( uint32_t * ) 0x5BC430 )
#define CC_P1_GUARD_BAR_ADDR        ( ( float * )    0x5BC434 )
#define CC_P1_GUARD_QUALITY_ADDR    ( ( float * )    0x5BC448 )
#define CC_P1_METER_ADDR            ( ( uint32_t * ) 0x5BC450 )
#define CC_P1_HEAT_ADDR             ( ( uint32_t * ) 0x5BC454 )
#define CC_P1_NO_INPUT_FLAG_ADDR    ( ( uint8_t * )  0x5BC4E7 ) // Indicates when input is disabled, ie KO or time over
#define CC_P1_PUPPET_STATE_ADDR     ( ( uint8_t * )  0x5BC4E8 ) // 0 is not puppet, 1 is puppet, 2 is puppet w/ hurtbox?

#define CC_P1_X_POSITION_ADDR       ( ( int32_t * )  0x5BC478 )
#define CC_P1_Y_POSITION_ADDR       ( ( int32_t * )  0x5BC47C )
#define CC_P1_X_PREV_POS_ADDR       ( ( int32_t * )  0x5BC484 )
#define CC_P1_Y_PREV_POS_ADDR       ( ( int32_t * )  0x5BC488 )
#define CC_P1_X_VELOCITY_ADDR       ( ( int32_t * )  0x5BC48C )
#define CC_P1_Y_VELOCITY_ADDR       ( ( int32_t * )  0x5BC490 )
#define CC_P1_X_ACCELERATION_ADDR   ( ( int16_t * )  0x5BC494 )
#define CC_P1_Y_ACCELERATION_ADDR   ( ( int16_t * )  0x5BC496 )
#define CC_P1_SPRITE_ANGLE_ADDR     ( ( uint32_t * ) 0x5BC670 )
#define CC_P1_FACING_FLAG_ADDR      ( ( uint8_t * )  0x5BC684 ) // 0 facing left, 1 facing right

#define CC_P2_ENABLED_FLAG_ADDR     ( ( uint8_t * )  ( ( ( char * ) CC_P1_ENABLED_FLAG_ADDR  ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_SEQUENCE_ADDR         ( ( uint32_t * ) ( ( ( char * ) CC_P1_SEQUENCE_ADDR      ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_SEQ_STATE_ADDR        ( ( uint32_t * ) ( ( ( char * ) CC_P1_SEQ_STATE_ADDR     ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_HEALTH_ADDR           ( ( uint32_t * ) ( ( ( char * ) CC_P1_HEALTH_ADDR        ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_RED_HEALTH_ADDR       ( ( uint32_t * ) ( ( ( char * ) CC_P1_RED_HEALTH_ADDR    ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_GUARD_BAR_ADDR        ( ( float * )    ( ( ( char * ) CC_P1_GUARD_BAR_ADDR     ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_GUARD_QUALITY_ADDR    ( ( float * )    ( ( ( char * ) CC_P1_GUARD_QUALITY_ADDR ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_METER_ADDR            ( ( uint32_t * ) ( ( ( char * ) CC_P1_METER_ADDR         ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_HEAT_ADDR             ( ( uint32_t * ) ( ( ( char * ) CC_P1_HEAT_ADDR          ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_NO_INPUT_FLAG_ADDR    ( ( uint8_t * )  ( ( ( char * ) CC_P1_NO_INPUT_FLAG_ADDR ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_PUPPET_STATE_ADDR     ( ( uint8_t * )  ( ( ( char * ) CC_P1_PUPPET_STATE_ADDR  ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_X_POSITION_ADDR       ( ( int32_t * )  ( ( ( char * ) CC_P1_X_POSITION_ADDR    ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_Y_POSITION_ADDR       ( ( int32_t * )  ( ( ( char * ) CC_P1_Y_POSITION_ADDR    ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P2_FACING_FLAG_ADDR      ( ( uint8_t * )  ( ( ( char * ) CC_P1_FACING_FLAG_ADDR   ) + CC_PLR_STRUCT_SIZE ) )

#define CC_P3_ENABLED_FLAG_ADDR     ( ( uint8_t * )  ( ( ( char * ) CC_P2_ENABLED_FLAG_ADDR  ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P3_NO_INPUT_FLAG_ADDR    ( ( uint8_t * )  ( ( ( char * ) CC_P2_NO_INPUT_FLAG_ADDR ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P3_PUPPET_STATE_ADDR     ( ( uint8_t * )  ( ( ( char * ) CC_P2_PUPPET_STATE_ADDR  ) + CC_PLR_STRUCT_SIZE ) )

#define CC_P4_ENABLED_FLAG_ADDR     ( ( uint8_t * )  ( ( ( char * ) CC_P3_ENABLED_FLAG_ADDR  ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P4_NO_INPUT_FLAG_ADDR    ( ( uint8_t * )  ( ( ( char * ) CC_P3_NO_INPUT_FLAG_ADDR ) + CC_PLR_STRUCT_SIZE ) )
#define CC_P4_PUPPET_STATE_ADDR     ( ( uint8_t * )  ( ( ( char * ) CC_P3_PUPPET_STATE_ADDR  ) + CC_PLR_STRUCT_SIZE ) )

// Camera addresses
#define CC_CAMERA_X_ADDR            ( ( int * )      0x5CB63C )
#define CC_CAMERA_Y_ADDR            ( ( int * )      0x5CB63F )

// Array of sound effect flags, each byte corresponds to a specific SFX, set to 1 to start
#define CC_SFX_ARRAY_ADDR           ( ( uint8_t * )  0x7D47B0 )
#define CC_SFX_ARRAY_LEN            ( 1500 )
#define DX_MUTED_VOLUME             ( 0xFFFFD8F0u )

// Number of frames in the initial movement only phase
#define CC_PRE_GAME_INTRO_FRAMES    ( 224 )

// Asm hacks are prefixed MM (for modified memory), they should be written to safe locations
#define MM_HOOK_CALL1_ADDR          ( ( char * )     0x439431 )
#define MM_HOOK_CALL2_ADDR          ( ( char * )     0x434D14 )


union IndexedFrame
{
    struct { uint32_t frame, index; } parts;
    uint64_t value;
};

const IndexedFrame MaxIndexedFrame = {{ UINT_MAX, UINT_MAX }};

inline std::ostream& operator<< ( std::ostream& os, const IndexedFrame& indexedFrame )
{
    return ( os << indexedFrame.parts.index << ':' << indexedFrame.parts.frame );
}


inline const char *gameModeStr ( uint32_t gameMode )
{
    switch ( gameMode )
    {
        case CC_GAME_MODE_STARTUP:
            return "Startup";

        case CC_GAME_MODE_OPENING:
            return "Opening";

        case CC_GAME_MODE_TITLE:
            return "Title";

        case CC_GAME_MODE_LOADING_DEMO:
            return "Loading-demo";

        case CC_GAME_MODE_HIGH_SCORES:
            return "High-scores";

        case CC_GAME_MODE_MAIN:
            return "Main";

        case CC_GAME_MODE_CHARA_SELECT:
            return "Character-select";

        case CC_GAME_MODE_LOADING:
            return "Loading";

        case CC_GAME_MODE_IN_GAME:
            return "In-game";

        case CC_GAME_MODE_RETRY:
            return "Retry";

        default:
            break;
    }

    return "Unknown game mode!";
}


const std::vector<std::pair<std::string, uint32_t>> gameInputBits =
{
    { "Up",          BIT_UP },
    { "Down",        BIT_DOWN },
    { "Left",        BIT_LEFT },
    { "Right",       BIT_RIGHT },
    { "A (confirm)", ( CC_BUTTON_A | CC_BUTTON_CONFIRM ) << 8 },
    { "B (cancel)",  ( CC_BUTTON_B | CC_BUTTON_CANCEL ) << 8 },
    { "C",           CC_BUTTON_C << 8 },
    { "D",           CC_BUTTON_D << 8 },
    { "E",           CC_BUTTON_E << 8 },
    { "Start",       CC_BUTTON_START << 8 },
    { "FN1",         CC_BUTTON_FN1 << 8 },
    { "FN2",         CC_BUTTON_FN2 << 8 },
    { "A+B",         CC_BUTTON_AB << 8 },
};
