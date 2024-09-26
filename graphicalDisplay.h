#ifndef __GRAPHICAL_DISPLAY_H__
#define __GRAPHICAL_DISPLAY_H__
#include <memory>
#include "window.h"
#include "square.h"
#include "observer.h"

class GraphicalDisplay : public Observer {
    std::unique_ptr<Xwindow> window;
  public:
    GraphicalDisplay();  // Default ctor

    void notify(Square &s) override;
    ~GraphicalDisplay();
};

#endif
