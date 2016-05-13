#include "globalState.h"

struct globalState gState = {

    // setting defaults
    .uiSettings = false,
    .uiOptions = false,
    .uiQuit = false,

    .colourR = 1,
    .colourG = 0,
    .colourB = 0,
    .colourRand = false,
    .angle = 5, // central preset
    .angleRand = false
};
