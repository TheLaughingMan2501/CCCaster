#pragma once

#ifdef STEAM_VER
#include "Constants_ST.hpp"
#else
#include "Constants.hpp"
#endif
#include "Exceptions.hpp"

#include <vector>
#include <array>


#define WRITE_ASM_HACK(ASM_HACK)                                                                                    \
    do {                                                                                                            \
        const int error = ASM_HACK.write();                                                                         \
        if ( error != 0 ) {                                                                                         \
            LOG ( "[%d] %s; %s failed; addr=%08x",                                                                  \
                  error, WinException::getAsString ( error ), #ASM_HACK, ASM_HACK.addr );                           \
            exit ( -1 );                                                                                            \
        }                                                                                                           \
    } while ( 0 )


#define INLINE_DWORD(X)                                                         \
    static_cast<unsigned char> ( unsigned ( X ) & 0xFF ),                       \
    static_cast<unsigned char> ( ( unsigned ( X ) >> 8 ) & 0xFF ),              \
    static_cast<unsigned char> ( ( unsigned ( X ) >> 16 ) & 0xFF ),             \
    static_cast<unsigned char> ( ( unsigned ( X ) >> 24 ) & 0xFF )

#define INLINE_DWORD_FF { 0xFF, 0x00, 0x00, 0x00 }

#define INLINE_FF_12_BYTES { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }

#define INLINE_NOP_TWO_TIMES { 0x90, 0x90 }

#define INLINE_NOP_THREE_TIMES { 0x90, 0x90, 0x90 }

#define INLINE_NOP_FOUR_TIMES { 0x90, 0x90, 0x90, 0x90 }

#define INLINE_NOP_SIX_TIMES { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }

#define INLINE_NOP_SEVEN_TIMES { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }

#define INLINE_NOP_TEN_TIMES { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }


namespace AsmHacks
{

// DLL callback function
extern "C" void callback();

// Position of the current menu's cursor, this gets updated by ASM hacks
extern uint32_t currentMenuIndex;

// The value of menuConfirmState is set to 1 if a menu confirm happens.
// A menu confirm will only go through if menuConfirmState is greater than 1.
extern uint32_t menuConfirmState;

// Round start counter, this gets incremented whenever players can start moving
extern uint32_t roundStartCounter;

// Auto replay save state, is set to 1 (loading), then 100 (saving), finally 255 (saved)
extern uint32_t *autoReplaySaveStatePtr;

// Flag to enable / disable the Escape key function that exits game (initially true)
extern uint8_t enableEscapeToExit;

// Array of sound effects that were played last frame, set any SFX to 1 to prevent playback.
extern uint8_t sfxFilterArray[CC_SFX_ARRAY_LEN];

// Indicates the next playback of the sound effect should be muted.
// Can mute and play a sound effect at the same time to effectively cancel an existing playback.
extern uint8_t sfxMuteArray[CC_SFX_ARRAY_LEN];

// The pointer to the current color table being loaded
extern uint32_t *currentColorTablePtr;

// The number of colors loaded, used to determine P1/P2/team colors
extern uint32_t numLoadedColors;


// Color loading callback functions
void colorLoadCallback ( uint32_t player, uint32_t chara, uint32_t *paletteData );
void colorLoadCallback ( uint32_t player, uint32_t chara, uint32_t palette, uint32_t *singlePaletteData );


// Struct for storing assembly code
struct Asm
{
    void *const addr;
    const std::vector<uint8_t> bytes;
    mutable std::vector<uint8_t> backup;

    int write() const;
    int revert() const;
};

typedef std::vector<Asm> AsmList;

// Add a call to the callback function just before the beginning of the game's main message loop.
// Note the message loop can get run multiple times per frame step, so be sure to check the world timer.
static const AsmList hookMainLoop =
{
    { MM_HOOK_CALL1_ADDR, {
        0xE8, INLINE_DWORD ( ( ( char * ) &callback ) - MM_HOOK_CALL1_ADDR - 5 ),   // call callback
        0xE9, INLINE_DWORD ( MM_HOOK_CALL2_ADDR - MM_HOOK_CALL1_ADDR - 10 )         // jmp MM_HOOK_CALL2_ADDR
    } },
    { MM_HOOK_CALL2_ADDR,{
        0x80, 0x3D, 0x93, 0xAB, 0x5B, 0x00, 0x00,									// CMP BYTE PTR DS:[5BAB93],0
        0xE9, INLINE_DWORD(CC_LOOP_START_ADDR - MM_HOOK_CALL2_ADDR - 5)				// jmp CC_LOOP_START_ADDR+6 (AFTER)
    } },
    // Write the jump location last, due to dependencies on the callback hook code
    { CC_LOOP_START_ADDR, {
        0xE9, INLINE_DWORD ( MM_HOOK_CALL1_ADDR - CC_LOOP_START_ADDR - 5 ),         // jmp MM_HOOK_CALL1_ADDR
        0x90, 0x90                                                                  // nop
                                                                                    // AFTER:
    } },
    // Skip GameMode to TITLE
    { (void *)0x475007,{
        0x02
    } },
};

// Enable disabled stages and fix Ryougi stage music looping
static const AsmList enableDisabledStages =
{
    // Enable disabled stages
    { ( void * ) 0x5B4E00, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E04, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E08, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E30, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E34, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E38, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E3C, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E40, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E44, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E48, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E4C, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E50, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E54, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E58, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E5C, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E60, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E64, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E68, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E6C, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E70, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E74, INLINE_DWORD_FF },
    { ( void * ) 0x5B4E78, INLINE_DWORD_FF },

    // Fix Ryougi stage music looping
    { ( void * ) 0x7CFC86, { 0x35, 0x00, 0x00, 0x00 } },
    { ( void * ) 0x7CFC7C, { 0xAA, 0xCC, 0x1E, 0x40 } },
};

// Disable the FPS limit by setting the game's perceived perf freq to 1
static const Asm disableFpsLimit = { CC_PERF_FREQ_ADDR, { INLINE_DWORD ( 1 ), INLINE_DWORD ( 0 ) } };

// Disable the code that updates the FPS counter
static const Asm disableFpsCounter = { ( void * ) 0x48B19C, INLINE_NOP_SIX_TIMES };

// Disable the Reset Win Counter when Character Select
static const Asm disableResetWinCounter = { ( void * ) 0x46E47B, INLINE_NOP_TEN_TIMES };

// Disable normal joystick and keyboard controls
static const AsmList hijackControls =
{
    // Disable joystick controls
    { ( void * ) 0x476219, INLINE_NOP_FOUR_TIMES  },
    { ( void * ) 0x4F0165, INLINE_NOP_TWO_TIMES   },
    { ( void * ) 0x4F018C, INLINE_NOP_THREE_TIMES },
    { ( void * ) 0x4F0196, INLINE_NOP_THREE_TIMES },
    { ( void * ) 0x4F01A0, INLINE_NOP_THREE_TIMES },
    { ( void * ) 0x4F01AA, INLINE_NOP_THREE_TIMES },
    { ( void * ) 0x4F01D7, INLINE_NOP_TWO_TIMES   },
    { ( void * ) 0x4F01DE, INLINE_NOP_THREE_TIMES },

    // Zero all keyboard keys
    { ( void * ) 0x5B4D40, {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    } },
};

// Copy dynamic menu variables and hijack the menu confirms
static const AsmList hijackMenu =
{
    // Copy the value of the current menu index (edi) to a location we control.
    // This hack uses the strategy of jumping around the extra spaces left in between code.
    { ( void * ) 0x51DA92,{
        0x8B, 0x40, 0x40,											// MOV EAX,DWORD PTR DS : [EAX + 40]
        0x89, 0x0D, INLINE_DWORD(&currentMenuIndex),                // MOV [&currentMenuIndex],ECX
        0xE9, 0xD0, 0x02, 0x00, 0x00                                // JMP 0x51DD70 (AFTER)
    } },
    { ( void * ) 0x51DC32,{
        0x8B, 0x48, 0x38,                                           // MOV ECX,DWORD PTR DS : [EAX + 38]
        0xE9, 0x58, 0xFE, 0xFF, 0xFF                                // jmp 0x51DA92
    } },
    // Write this last due to dependencies
    { ( void * ) 0x51DD6A,{
        0xE9, 0xC3, 0xFE, 0xFF, 0xFF, 0x90,                         // jmp 0x51DC32
                                                                    // AFTER:
    } },

    // Code that allows us to selectively override menu confirms.
    // The value of menuConfirmState is set to 1 if a menu confirm happens.
    // A menu confirm will only go through if menuConfirmState is greater than 1.
    { ( void * ) 0x414B43,{
        0x53, 0x51, 0x52,                                           // push ebx,ecx,edx
        0x8D, 0x5C, 0xE4, 0x2C,                                     // lea ebx,[esp+2C]
        0xEB, 0x0C                                                  // jmp 0x414B58
    } },
    { ( void * ) 0x414B58,{ 
        0xB9, INLINE_DWORD(&menuConfirmState),                      // mov ecx,&menuConfirmState
        0xEB, 0x07                                                  // jmp 0x414B66
    } },
    { ( void * ) 0x414B66,{
        0x83, 0x39, 0x01,                                           // cmp dword ptr [ecx],01
        0x8B, 0x13,                                                 // mov edx,[ebx]
        0xEB, 0x08                                                  // jmp 0x414B75
    } },
    { ( void * ) 0x414B75,{
        0x89, 0x11,                                                 // mov [ecx],edx
        0x7F, 0x32,                                                 // jg 0x414BAB (LABEL_B)
        0xEB, 0x2C                                                  // jmp 0x414BA7 (LABEL_A)
    } },
    { ( void * ) 0x414B98,{
        0x5A, 0x59, 0x5B,                                           // pop edx,ecx,ebx
        0x90,                                                       // nop
        0xEB, 0xF7                                                  // jmp 0x414B95 (RETURN)
    } },
    { ( void * ) 0x414BA7,{
                                                                    // LABEL_A:
        0x89, 0x03,                                                 // mov [ebx],eax
        0xEB, 0xED,                                                 // jmp 0x414B98
                                                                    // LABEL_B:
        0x89, 0x01,                                                 // mov [ecx],eax
        0xEB, 0xE9                                                  // jmp 0x414B98
    } },
    // Write this last due to dependencies
    { ( void * ) 0x414B82,{
        0x81, 0x3C, 0xE4, 0xBE, 0x59, 0x43, 0x00,                   // cmp [esp],004359BE (return addr of menu call)
        0x75, 0x0A,                                                 // jne 0x414B95 (RETURN)
        0xEB, 0xB6                                                  // jmp 0x414B43
    } },
    { ( void * ) 0x414B95,{
        // RETURN:
        0xC2, 0x04, 0x00                                            // ret 0004
    } },
};

// Increment a counter at the beginning of the round when players can move
static const AsmList detectRoundStart =
{
    { ( void * ) 0x499678,{
        0xE9, 0xF5, 0xFE, 0xFF, 0xFF								// JMP 0x499572
    } },
    { ( void * ) 0x499572,{
        0xB9, INLINE_DWORD(&roundStartCounter),						// mov ecx,[&roundStartCounter]
        0xE9, 0xFA, 0x03, 0x00, 0x00								// jmp 0x499976
    } },
    { ( void * ) 0x499976,{
        0x8B, 0x31,                                                 // mov esi,[ecx]
        0x46,                                                       // inc esi
        0x89, 0x31,                                                 // mov [ecx],esi
        0x5E,                                                       // pop esi
        0x5B,                                                       // pop ebx
        0xC3                                                        // ret
    } },
    // Write this last due to dependencies
    { ( void * ) 0x499631,{
        0xEB, 0x45                                                  // jmp 0x499678
    } },
};

// This copies an auto replay save flag to a non-dynamic memory location.
// Used to detect when the auto replay save is done and the menu is up.
static const AsmList detectAutoReplaySave =
{
    { ( void * ) 0x4DC734,{
        0x89, 0x0D, INLINE_DWORD(&autoReplaySaveStatePtr),       // mov [&autoReplaySaveStatePtr],ecx
        0xE9, 0x52, 0x10, 0x00, 0x00                             // jmp 0x4DD791 
    } },
    { ( void * ) 0x4DC9E2,{
        0x8D, 0x89, 0xCC, 0x00, 0x00, 0x00,                      // lea ecx,[ECX+0CC]
        0xE9, 0x47, 0xFD, 0xFF, 0xFF                             // jmp 0x4DC734
    } },
    { ( void * ) 0x4DD791,{
        0x8B, 0x4D, 0xF4,                                        // MOV ECX,DWORD PTR SS:[EBP-0C]
        0xE9, 0xB3, 0xF1, 0xFF, 0xFF                             // jmp 0x4DC94C (RETURN)
    } },
    { ( void * ) 0x4DC949,{
        0xEB, 0x0F, 0x90										 // jmp 0x4DC95A
    } },
    { ( void * ) 0x4DC95A,{
        0xE9, 0x83, 0x00, 0x00, 0x00,							 // jmp 0x4DC9E2
    } },
};
// Force the game to go to a certain mode
static const Asm forceGotoVersus    = { (void *)0x4852E6,{ 0xE9, 0x80, 0x00, 0x00, 0x00 } }; // jmp 0048536B
static const Asm forceGotoVersusCPU = { (void *)0x4852E6,{ 0xE9, 0xB9, 0x00, 0x00, 0x00 } }; // jmp 004853A4
static const Asm forceGotoTraining  = { (void *)0x4852E6,{ 0xEB, 0x4A } }; // jmp 00485332
static const Asm forceGotoReplay    = { (void *)0x4852E6,{ 0xE9, 0x41, 0x01, 0x00, 0x00 } }; // jmp 0048542C

// Hijack the game's Escape key so we can control when it exits the game
static const Asm hijackEscapeKey =
    { ( void * ) 0x4A0070, {
        0x80, 0x3D, INLINE_DWORD ( &enableEscapeToExit ), 0x00,     // cmp byte ptr [&enableEscapeToExit],00
        0xA0, INLINE_DWORD ( 0x5544F1 ),                            // mov ax,[005544F1]
        0x75, 0x03,                                                 // jne 0x4A0081 (AFTER)
        0x66, 0x31, 0xC0,                                           // xor ax,ax
                                                                    // AFTER:
        0x24, 0x80,                                                 // and al,80
        // Rest of the code is unchanged, just shifted down
        0x33, 0xC9,                                                 // xor ecx,ecx
        0x3C, 0x80,                                                 // cmp al,80
        0x0F, 0x94, 0xC1,                                           // sete cl
        0x8B, 0xC1,                                                 // mov eax,ecx
        0xC3,                                                       // ret
    } };

// This increments a counter for each sound effect played,
// but only actually plays the sound if its muted or at zero plays.
static const AsmList filterRepeatedSfx =
{
    { ( void * ) 0x520BE1,{
        0xB8, INLINE_DWORD(sfxMuteArray),							// mov eax,sfxMuteArray
        0xEB, 0x79,                                                 // jmp 0x520C61
    } },
    { ( void * ) 0x520C61,{
        0x80, 0x3C, 0x06, 0x00,                                     // cmp byte ptr [eax+esi],00
        0xE9, 0xB9, 0x0A, 0x00, 0x00                                // jmp 0x521723
    } },
    { ( void * ) 0x521723,{
        0x0F, 0x84, 0xFA, 0x00, 0x00, 0x00,                         // je 0x521823
        0x58,                                                       // pop eax
        0xE9, 0x25, 0x03, 0x00, 0x00                                // jmp 0x521A54
    } },
    { ( void * ) 0x521823,{
        0xB8, INLINE_DWORD(sfxFilterArray),                         // mov eax,sfxFilterArray
        0x80, 0x04, 0x06, 0x01,                                     // add byte ptr [eax+esi],01
        0xEB, 0x47                                                  // jmp 0x521875
    } },
    { ( void * ) 0x521875,{
        0xE9, 0x78, 0x13, 0x00, 0x00								// jmp 0x522BF2
    } },
    { ( void * ) 0x522BF2,{
        0x80, 0x3C, 0x30, 0x01,                                     // cmp byte ptr [eax+esi],01
        0x58,                                                       // pop eax
        0x0F, 0x87, 0x73, 0xED, 0xFF, 0xFF,                         // ja 0x521970 (SKIP_SFX)
        0xEB, 0x38                                                  // jmp 0x522C37
    } },
    { ( void * ) 0x522C37,{
        0xE9, 0x18, 0xEE, 0xFF, 0xFF								// jmp 0x521A54
    } },
    { ( void * ) 0x521A54,{
        0x8B, 0x0C, 0xB5, INLINE_DWORD(0x7D2D90),                   // MOV ECX,DWORD PTR DS:[ESI*4+7D2D90]
        0xE9, 0x06, 0xFF, 0xFF, 0xFF                                // jmp 0x521966 (PLAY_SFX)
    } },
    // Write this last due to dependencies
    { ( void * ) 0x52195F,{
        0x50,                                                       // push eax
        0xE9, 0x7C, 0xF2, 0xFF, 0xFF,                               // jmp 0x520BE1
        0x90                                                        // nop
                                                                    // PLAY_SFX:
                                                                    // test ECX,ECX
                                                                    // je 0x52196F
                                                                    // call 0x466C60
                                                                    // INC EDI
                                                                    // INC ESI
                                                                    // SKIP_SFX:
    } },
};

// Mutes the next playback of a specific sound effect
static const AsmList muteSpecificSfx =
{
    { ( void * ) 0x466452, {
        0x8B, 0x14, 0x24,                                           // mov edx,[esp]
        0x81, 0xFA, INLINE_DWORD ( CC_SFX_ARRAY_LEN ),              // cmp edx,CC_SFX_ARRAY_LEN
        0xE9, 0x51, 0x04, 0x00, 0x00                                // jmp 0x4668B1
    } },
    { ( void * ) 0x4668B1, {
        0x0F, 0x8D, 0xFF, 0x02, 0x00, 0x00,                         // jnl 0x466BB6 (AFTER)
        0xE9, 0x67, 0x01, 0x00, 0x00                                // jmp 0x466A23
    } },
    { ( void * ) 0x466A23, {
        0x0F, 0x85, 0xE9, 0x04, 0x00, 0x00,                         // jne 0x466F12
        0xE9, 0x88, 0x01, 0x00, 0x00                                // jmp 0x466BB6 (AFTER)
    } },
    { ( void * ) 0x466BB6, {                                        // AFTER:
        0xFF, 0x77, 0x18,                                           // PUSH DWORD PTR DS:[EDI+18]
        0x8B, 0x06,                                                 // MOV EAX,DWORD PTR DS:[ESI]
        0x56,                                                       // PUSH ESI
        0xE9, 0xD2, 0x00, 0x00, 0x00                                // jmp 0x466C92
    } },
    { ( void * ) 0x466F12, {
        0x8D, 0x92, INLINE_DWORD ( sfxMuteArray ),                  // lea edx,[edx+sfxMuteArray]
        0xE9, 0x9B, 0x00, 0x00, 0x00                                // jmp 0x466FB2
    } },
    { ( void * ) 0x466FB2, {
        0x80, 0x3A, 0x00,                                           // cmp byte ptr [edx],00
        0xC6, 0x02, 0x00,                                           // mov byte ptr [edx],00
        0xEB, 0x27                                                  // jmp 0x466FE1
    } },
    { ( void * ) 0x466FE1, {
        0x74, 0x05,                                                 // je 0x466FE8 (DONE_MUTE)
        0xB9, INLINE_DWORD ( DX_MUTED_VOLUME ),                     // mov ecx,DX_MUTED_VOLUME
                                                                    // DONE_MUTE:
        0xE9, 0xC9, 0xFB, 0xFF, 0xFF                                // jmp 0x466BB6 (AFTER)
    } },
    // Write this last due to dependencies
    { ( void * ) 0x466C92, {
        0xE9, 0xBB, 0xF7, 0xFF, 0xFF, 0x90                          // jmp 0x466452
    } },
};

// Disables the code that sets the intro state to 0. This is so we can manually set it during rollback
static const Asm hijackIntroState = { ( void * ) 0x4B2F1A, INLINE_NOP_SEVEN_TIMES };

// Prevent training mode music from reseting
static const Asm disableTrainingMusicReset = { ( void * ) 0x4CB533, { 0xEB, 0x05 } }; // jmp 004CB53A

// Fix the super flash overlays on the two boss stages.
// This is done by changing the string that the games searches for in the stage config .ini.
// Search for 'IsGiantStage' in bg/BgList.ini inside 0000.p.
static const Asm fixBossStageSuperFlashOverlay = { ( void * ) 0x53B3C8, INLINE_FF_12_BYTES };

// The color load callback during character select
extern "C" void charaSelectColorCb();

// Inserts a callback just after the color data is loaded into memory, but before it is read into the sprites.
// The color values can be effectively overridden here. This is only effective during character select.
static const Asm hijackCharaSelectColors =
    { ( void * ) 0x489CD1, {
        0xE8, INLINE_DWORD ( ( ( char * ) &charaSelectColorCb ) - 0x489CD1 - 5 ),       // call charaSelectColorCb
        0x90, 0x90, 0x90,                                                               // nops
    } };

// The color load callback during loading state
extern "C" void loadingStateColorCb();

// Inserts a callback just after the color data is loaded into memory, but before it is read into the sprites.
// The color values can be effectively overridden here. This is only effective during the loading state.
static const AsmList hijackLoadingStateColors =
{
    { ( void * ) 0x448202, {
        0x50,                                                                           // push eax
        0xE8, INLINE_DWORD ( ( ( char * ) &loadingStateColorCb ) - 0x448202 - 1 - 5 ),  // call loadingStateColorCb
        0x58,                                                                           // pop eax
        0x85, 0xC0,                                                                     // test eax,eax
        0xEB, 0x38,                                                                     // jmp 0x448245
    } },
    { ( void * ) 0x448245, {
        0x89, 0x44, 0x24, 0x20,                                                         // mov [esp+20],eax
        0xE9, 0x81, 0x09, 0x00, 0x00,                                                   // jmp 0x448BCF (AFTER)
    } },
    // Write this last due to dependencies
    { ( void * ) 0x448BC9, {
        0xE9, 0x34, 0xF6, 0xFF, 0xFF,                                                   // jmp 0x448202
        0x90,                                                                           // nop
                                                                                        // AFTER:
    } },
};

} // namespace AsmHacks
