#ifndef __MOUSEEMU_H__
#define __MOUSEEMU_H__

#include <stdio.h>

#include "mouseconfig.h"
#include "appconfig.h"

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>



bool initMouseEmu();
void mouseButton(char * buttons);
void mouseMove(char x, char y);

#endif
