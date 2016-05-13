#include "uiMouseHandler.h"

bool uiMouseHandler(int button, int st, int x, int y)
{
    if(gState.uiSettings)
    {
        mouseUISettings(button, st, x, y);
        return true;
    }
    else if(gState.uiOptions)
    {
        if(gState.uiQuit)
        {
            mouseUIQuit(button, st, x, y);
            return true;
        }
        else
        {
            mouseUIOptions(button, st, x, y);
            return true;
        }
    }
    else
        return false;
}

