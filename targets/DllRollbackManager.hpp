#pragma once

#include "DllNetplayManager.hpp"
#ifdef STEAM_VER
#include "Constants_ST.hpp"
#else
#include "Constants.hpp"
#endif

#include <memory>
#include <stack>
#include <list>
#include <array>
#include <cfenv>


class DllRollbackManager
{
public:

    // Allocate / deallocate memory for saving game states
    void allocateStates();
    void deallocateStates();

    // Save / load current game state
    void saveState ( const NetplayManager& netMan );
    bool loadState ( IndexedFrame indexedFrame, NetplayManager& netMan );

    // Save sounds during rollback re-run
    void saveRerunSounds ( uint32_t frame );

    // Finalize rollback sound effects
    void finishedRerunSounds();

private:

    struct GameState
    {
        // Each game state is uniquely identified by (netplayState, startWorldTime, indexedFrame).
        // They are chronologically ordered by index and then frame.
        NetplayState netplayState;
        uint32_t startWorldTime;
        IndexedFrame indexedFrame;
        std::fenv_t fp_env;

        // The pointer to the raw bytes in the state pool
        char *rawBytes;

        // Save / load the game state
        void save();
        void load();
    };

    // Memory pool to allocate game states
    std::shared_ptr<char> _memoryPool;

    // Unused indices in the memory pool, each game state has the same size
    std::stack<size_t> _freeStack;

    // List of saved game states in chronological order
    std::list<GameState> _statesList;

    // History of sound effect playbacks
    std::array<std::array<uint8_t, CC_SFX_ARRAY_LEN>, NUM_ROLLBACK_STATES> _sfxHistory;
};
