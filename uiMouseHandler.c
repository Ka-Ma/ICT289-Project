#include "uiMouseHandler.h"
#include "state.h"
#include "uiSettings.h"
#include "uiOptions.h"
#include "uiQuit.h"

bool uiMouseHandler(int button, int st, int x, int y)
{
    if(state.uiSettings)
    {
        mouseUISettings(button, st, x, y);
        return true;
    }
    else if(state.uiOptions)
    {
        if(state.uiQuit)
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

