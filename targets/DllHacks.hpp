#pragma once

#ifdef STEAM_VER
#include "DllAsmHacks_ST.hpp"
#else
#include "DllAsmHacks.hpp"
#endif


namespace DllHacks
{

extern void *windowHandle;

// Hacks to apply before entry into the main game loop
void initializePreLoad();

// Hacks to apply after entry into the main game loop
void initializePostLoad();

// Disable all hacks
void deinitialize();

}
