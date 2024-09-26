#ifndef __TEXT_DISPLAY_H__
#define __TEXT_DISPLAY_H__
#include <vector>
#include <iostream>
#include "observer.h"
#include "game.h"
#include "square.h"

class TextDisplay: public Observer {
    std::vector<std::vector<char>> display;
  public:
    TextDisplay();  // Default constructor

    void notify(Square &s) override;
    ~TextDisplay();  // Clears display

    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
