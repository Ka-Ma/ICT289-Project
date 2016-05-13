#include "uiKeysHandler.h"

bool uiKeysHandler(unsigned char key, int x, int y)
{
    if(gState.uiSettings)
    {
        keysUISettings(key, x, y);
        return true;
    }
    else if(gState.uiOptions)
    {
        if(gState.uiQuit)
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
