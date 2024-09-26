#ifndef __LEVEL_FOUR_H__
#define __LEVEL_FOUR_H__
#include "computer.h"

class LevelFour : public Computer {
  public:
    LevelFour(Colour c, Colour start); // Constructor
    std::vector<std::string> cpuMove(const std::unique_ptr<Board>& b, 
      const std::unique_ptr<Player>& next) override;
};

#endif
