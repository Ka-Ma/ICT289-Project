        /**
        @brief Global Variables for fireworks and UI display states
        @author Katherine Mann
        @version 01
        @date 13/5/2016
        */

#ifndef STATE_H
#define STATE_H

#include <stdbool.h>

typedef struct globalState{
        /// display states
    bool uiSettings;
    bool uiOptions;
    bool uiQuit;

        /// firework settings
    float colourR;
    float colourG;
    float colourB;
    bool colourRand;
    int angle; // 1-9 to represent presets
    bool angleRand;
    float fuseMin;
    float fuseMax;
    float fuseCh;
    float velocityMin;
    float velocityMax;
    float velocityCh;
};

extern struct globalState gState;

#endif // STATE_H
