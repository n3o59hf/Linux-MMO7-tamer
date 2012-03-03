#ifndef __APPCONFIG_H__
#define __APPCONFIG_H__

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#define FUNCTION_NONE 0
#define FUNCTION_MOUSE_CLICK 128
#define FUNCTION_KEY_CODE 256

bool init();
int getFunction(int key);
int getKeyCode(int key);
int getKeySym(int key);

#endif
