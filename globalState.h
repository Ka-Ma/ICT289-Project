        /**
        @brief Global Variables for fireworks and UI display states, including temporary variables for settings UI
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
    float colour[3];
    bool colourRand;
    int angle; // 1-9 to represent presets
    bool angleRand;
    float velocityMin;
    float velocityMax;
    float velocityCh;
};

extern struct globalState gState;
extern struct globalState tmpState;

#endif // STATE_H
