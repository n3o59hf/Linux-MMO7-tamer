#include "appconfig.h"


bool init()
{

}

int getFunction(int key)
{
    if (key > 0 && key < 4) return FUNCTION_MOUSE_CLICK;
    if (key > 8 && key < 13) return FUNCTION_KEY_CODE;
    return FUNCTION_NONE;
}

int getKeyCode(int key) {
    if (key == 1) return 1;
    if (key == 2) return 3;
    if (key == 3) return 2;
    if (key == 4) return 4;
    if (key == 5) return 5;
    return 0;
}
int getKeySym(int key)
{
    if (key == 1) return 1;
    if (key == 2) return 3;
    if (key == 3) return 2;


    if (key == 11) return XK_W;
    if (key == 12) return XK_S;
    if (key == 9) return XK_D;
    if (key == 10) return XK_A;

    return 0;
}
