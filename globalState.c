#include "globalState.h"

struct globalState gState = {

    // setting defaults
    .uiSettings = false,
    .uiOptions = false,
    .uiQuit = false,

    .colour = {1,0,0},
    .colourRand = false,
    .angle = 5, // central preset
    .angleRand = false
};

struct globalState tmpState;
