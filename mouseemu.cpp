#include "mouseemu.h"


Display *dpy;
int scr;
Window win;
unsigned int width, height;

char * oldButtons;


bool initMouseEmu()
{
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

    oldButtons = new char[BUTTON_COUNT / 8 + (BUTTON_COUNT % 8 > 0?1:0)];
    for (int i = 0; i < BUTTON_COUNT; i+=8)
    {
        oldButtons[i/8]=0;
    }
}


void mouseButton(char * buttons)
{
    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        int button = i + 1;

        if (buttons[i/8] & (0x01<<(i%8)) && !(oldButtons[i/8]&(0x01<<(i%8))))
        {
            printf("button %d click\n", button);
            switch (getFunction(button))
            {
            case FUNCTION_MOUSE_CLICK:
                XTestFakeButtonEvent(dpy, getKeyCode(button), True, CurrentTime);
                break;
            case FUNCTION_KEY_CODE:
                int keycode = XKeysymToKeycode(dpy,getKeySym(button));
                XTestFakeKeyEvent(dpy, keycode, True, CurrentTime);
                break;
            }
        }
        if (!(buttons[i/8] & (0x01<<(i%8) )) && oldButtons[i/8]&(0x01<<(i%8)))
        {
            printf("button %d unclick\n",button);
            switch (getFunction(button))
            {
            case FUNCTION_MOUSE_CLICK:
                XTestFakeButtonEvent(dpy, getKeyCode(button), False, CurrentTime);
                break;
            case FUNCTION_KEY_CODE:
                int keycode = XKeysymToKeycode(dpy,getKeySym(button));
                XTestFakeKeyEvent(dpy, keycode, False, CurrentTime);
                break;
            }
        }
    }

    XFlush(dpy);

    for (int i = 0; i < BUTTON_COUNT; i+=8)
    {
        oldButtons[i/8]=buttons[i/8];
    }
}

void mouseMove(char x, char y)
{
    if (x != 0 || y != 0)
    {
        int dX = x;
        int dY = y;
        XTestFakeRelativeMotionEvent(dpy, dX, dY, 0);
    }
}
