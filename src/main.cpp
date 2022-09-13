//
// Created by m4tex on 9/13/22.
//
#include <iostream>
#include <X11/Xlib.h>

enum {
    RECT_X = 20,
    RECT_Y = 20,
    RECT_WIDTH = 10,
    RECT_HEIGHT = 10,

    WIN_X = 10,
    WIN_Y = 10,
    WIN_WIDTH = 100,
    WIN_HEIGHT = 100,
    WIN_BORDER = 1
};

int main() {
    Display *display;
    Window window;
    XEvent event;
    int screen;

    display = XOpenDisplay(NULL);
    if (display == NULL) exit(1);

    screen = DefaultScreen(display);

    window = XCreateSimpleWindow(display, RootWindow(display, screen), WIN_X, WIN_Y, WIN_WIDTH, WIN_HEIGHT, WIN_BORDER,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    Atom del_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &del_window, 1);

    XMapWindow(display, window);


    //heeeeeeeell naaw... I saw more errors than I could handle. Sure, I could fix it, but I need to spend my time on the more important stuff

    return 0;
}