        /**
        @brief Evaluates situation, if particular UI is displayed passes keystroke to that UI and returns true.
        @author Katherine Mann
        @version 01
        @date 13/5/2016
        */

#ifndef UIKEYSHANDLER_H
#define UIKEYSHANDLER_H

#include <stdbool.h>
#include "globalState.h"
#include "uiSettings.h"
#include "uiOptions.h"
#include "uiQuit.h"

bool uiKeysHandler(unsigned char key, int x, int y);

#endif //UIKEYSHANDLER_H
