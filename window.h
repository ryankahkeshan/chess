#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <X11/Xlib.h>
#include <iostream>
#include <string>
#include "constants.h"

class Xwindow {
  Display *d;
  Window w;
  int s;
  GC gc;
  unsigned long colours[10];

 public:
  // Constructor; displays the window.
  Xwindow(int width=GRAPHICAL_DISPLAY_WIDTH, int height=GRAPHICAL_DISPLAY_HEIGHT);
  ~Xwindow();  // Destructor; destroys the window.

  enum {White=0, Black, Red, Green, Blue}; // Available colours.

  // Draws a rectangle
  void fillRectangle(int x, int y, int width, int height, int colour=Black);

  // Draws a string
  void drawString(int x, int y, std::string msg);

};

#endif
