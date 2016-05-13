#include "uiKeysHandler.h"
#include "state.h"
#include "uiSettings.h"
#include "uiOptions.h"
#include "uiQuit.h"

bool uiKeysHandler(unsigned char key, int x, int y)
{
    if(state.uiSettings)
    {
        keysUISettings(key, x, y);
        return true;
    }
    else if(state.uiOptions)
    {
        if(state.uiQuit)
        {
            keysUIQuit(key,x,y);
            return true;
        }
        else
        {
            keysUIOptions(key,x,y);
            return true;
        }
    }
    else
        return false;
}
