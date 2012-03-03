#include "mouseemu.h"


Display *dpy;
int scr;
Window win;
unsigned int width, height;

char oldButtons = 0;


bool initMouseEmu() {
    Window ret_win;
    int dX, dY;
    unsigned int border_width, depth;

    //Preparing X control

    dpy = XOpenDisplay("");
    if (!dpy)
    {
        printf("Cannot connect to display ...\n");
        return false;
    }
    scr = DefaultScreen(dpy);
    win = RootWindow(dpy, scr);
    XGetGeometry(dpy, win, &ret_win, &dX, &dY, &width, &height, &border_width, &depth);

    int ev, er, ma, mi;
    if(!XTestQueryExtension(dpy, &ev, &er, &ma, &mi))
    {
        printf("XTest extension not supported on server.\n");
        return false;
    }

}


void mouseButton(char buttons)
{
    for (int i = 0; i < 8; i++)
    {
        int button = i + 1;

        if (buttons & (0x01<<i) && !(oldButtons&(0x01<<i)))
        {
            printf("button %d click\n", button);
            XTestFakeButtonEvent(dpy, button, True, CurrentTime);
        }
        if (!(buttons & (0x01<<i)) && oldButtons&(0x01<<i))
        {
            printf("button %d unclick\n",button);
            XTestFakeButtonEvent(dpy, button, False, CurrentTime);
        }
    }

    XFlush(dpy);
    oldButtons = buttons;
}

void mouseMove(char x, char y) {
    if (x != 0 || y != 0) {
        int dX = x;
        int dY = y;
        XTestFakeRelativeMotionEvent(dpy, dX, dY, 0);
    }
}
