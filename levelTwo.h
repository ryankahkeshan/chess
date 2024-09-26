#ifndef __LEVEL_TWO_H__
#define __LEVEL_TWO_H__
#include "computer.h"

class LevelTwo : public Computer {
  public:
    LevelTwo(Colour c, Colour start); // Constructor
    std::vector<std::string> cpuMove(const std::unique_ptr<Board>& b, 
      const std::unique_ptr<Player>& next) override;
};

#endif
