#include "globalState.h"

struct globalState gState = {

    // setting defaults
    .uiSettings = false,
    .uiOptions = false,
    .uiQuit = false,

    .colour = {1,0,0},
    .colourRand = false,
    .angle = 5, // central preset
    .angleRand = false,
    .velocityMin = 4.1,
    .velocityMax = 12.3,
    .velocityCh = 8.6,
    .fuseMin = 500,
    .fuseMax = 2000,
    .fuseCh = 900,
    .trackFW = false
};

struct globalState tmpState; // for the settings UI before accepting changes
